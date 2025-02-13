/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2009 Konstantin Belousov <kib@FreeBSD.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice unmodified, this list of conditions, and the following
 *    disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/proc.h>
#include <sys/rangelock.h>
#include <sys/systm.h>

#include <vm/uma.h>

struct rl_q_entry {
	TAILQ_ENTRY(rl_q_entry) rl_q_link;
	off_t		rl_q_start, rl_q_end;
	int		rl_q_flags;
};

static uma_zone_t rl_entry_zone;

static void
rangelock_sys_init(void *arg __unused)
{

	rl_entry_zone = uma_zcreate("rl_entry", sizeof(struct rl_q_entry),
	    NULL, NULL, NULL, NULL, UMA_ALIGN_PTR, 0);
}
SYSINIT(vfs, SI_SUB_LOCK, SI_ORDER_ANY, rangelock_sys_init, NULL);

static struct rl_q_entry *
rlqentry_alloc(void)
{

	return (uma_zalloc(rl_entry_zone, M_WAITOK));
}

void
rlqentry_free(struct rl_q_entry *rleq)
{

	uma_zfree(rl_entry_zone, rleq);
}

void
rangelock_init(struct rangelock *lock)
{

	TAILQ_INIT(&lock->rl_waiters);
	lock->rl_currdep = NULL;
}

void
rangelock_destroy(struct rangelock *lock)
{

	KASSERT(TAILQ_EMPTY(&lock->rl_waiters), ("Dangling waiters"));
}

/*
 * Two entries are compatible if their ranges do not overlap, or both
 * entries are for read.
 */
static int
ranges_overlap(const struct rl_q_entry *e1,
    const struct rl_q_entry *e2)
{

	if (e1->rl_q_start < e2->rl_q_end && e1->rl_q_end > e2->rl_q_start)
		return (1);
	return (0);
}

/*
 * Recalculate the lock->rl_currdep after an unlock.
 */
static void
rangelock_calc_block(struct rangelock *lock)
{
	struct rl_q_entry *entry, *nextentry, *entry1;

	for (entry = lock->rl_currdep; entry != NULL; entry = nextentry) {
		nextentry = TAILQ_NEXT(entry, rl_q_link);
		if (entry->rl_q_flags & RL_LOCK_READ) {
			/* Reads must not overlap with granted writes. */
			for (entry1 = TAILQ_FIRST(&lock->rl_waiters);
			    !(entry1->rl_q_flags & RL_LOCK_READ);
			    entry1 = TAILQ_NEXT(entry1, rl_q_link)) {
				if (ranges_overlap(entry, entry1))
					goto out;
			}
		} else {
			/* Write must not overlap with any granted locks. */
			for (entry1 = TAILQ_FIRST(&lock->rl_waiters);
			    entry1 != entry;
			    entry1 = TAILQ_NEXT(entry1, rl_q_link)) {
				if (ranges_overlap(entry, entry1))
					goto out;
			}

			/* Move grantable write locks to the front. */
			TAILQ_REMOVE(&lock->rl_waiters, entry, rl_q_link);
			TAILQ_INSERT_HEAD(&lock->rl_waiters, entry, rl_q_link);
		}

		/* Grant this lock. */
		entry->rl_q_flags |= RL_LOCK_GRANTED;
		wakeup(entry);
	}
out:
	lock->rl_currdep = entry;
}

static void
rangelock_unlock_locked(struct rangelock *lock, struct rl_q_entry *entry,
    struct mtx *ilk, bool do_calc_block)
{

	MPASS(lock != NULL && entry != NULL && ilk != NULL);
	mtx_assert(ilk, MA_OWNED);

	if (!do_calc_block) {
		/*
		 * This is the case where rangelock_enqueue() has been called
		 * with trylock == true and just inserted this entry in the
		 * queue.
		 * If rl_currdep is this entry, rl_currdep needs to
		 * be set to the next entry in the rl_waiters list.
		 * However, since this entry is the last entry in the
		 * list, the next entry is NULL.
		 */
		if (lock->rl_currdep == entry) {
			KASSERT(TAILQ_NEXT(lock->rl_currdep, rl_q_link) == NULL,
			    ("rangelock_enqueue: next entry not NULL"));
			lock->rl_currdep = NULL;
		}
	} else
		KASSERT(entry != lock->rl_currdep, ("stuck currdep"));

	TAILQ_REMOVE(&lock->rl_waiters, entry, rl_q_link);
	if (do_calc_block)
		rangelock_calc_block(lock);
	mtx_unlock(ilk);
	if (curthread->td_rlqe == NULL)
		curthread->td_rlqe = entry;
	else
		rlqentry_free(entry);
}

void
rangelock_unlock(struct rangelock *lock, void *cookie, struct mtx *ilk)
{

	MPASS(lock != NULL && cookie != NULL && ilk != NULL);

	mtx_lock(ilk);
	rangelock_unlock_locked(lock, cookie, ilk, true);
}

/*
 * Unlock the sub-range of granted lock.
 */
void *
rangelock_unlock_range(struct rangelock *lock, void *cookie, off_t start,
    off_t end, struct mtx *ilk)
{
	struct rl_q_entry *entry;

	MPASS(lock != NULL && cookie != NULL && ilk != NULL);
	entry = cookie;
	KASSERT(entry->rl_q_flags & RL_LOCK_GRANTED,
	    ("Unlocking non-granted lock"));
	KASSERT(entry->rl_q_start == start, ("wrong start"));
	KASSERT(entry->rl_q_end >= end, ("wrong end"));

	mtx_lock(ilk);
	if (entry->rl_q_end == end) {
		rangelock_unlock_locked(lock, cookie, ilk, true);
		return (NULL);
	}
	entry->rl_q_end = end;
	rangelock_calc_block(lock);
	mtx_unlock(ilk);
	return (cookie);
}

/*
 * Add the lock request to the queue of the pending requests for
 * rangelock.  Sleep until the request can be granted unless trylock == true.
 */
static void *
rangelock_enqueue(struct rangelock *lock, off_t start, off_t end, int mode,
    struct mtx *ilk, bool trylock)
{
	struct rl_q_entry *entry;
	struct thread *td;

	MPASS(lock != NULL && ilk != NULL);

	td = curthread;
	if (td->td_rlqe != NULL) {
		entry = td->td_rlqe;
		td->td_rlqe = NULL;
	} else
		entry = rlqentry_alloc();
	MPASS(entry != NULL);
	entry->rl_q_flags = mode;
	entry->rl_q_start = start;
	entry->rl_q_end = end;

	mtx_lock(ilk);
	/*
	 * XXXKIB TODO. Check that a thread does not try to enqueue a
	 * lock that is incompatible with another request from the same
	 * thread.
	 */

	TAILQ_INSERT_TAIL(&lock->rl_waiters, entry, rl_q_link);
	/*
	 * If rl_currdep == NULL, there is no entry waiting for a conflicting
	 * range to be resolved, so set rl_currdep to this entry.  If there is
	 * no conflicting entry for this entry, rl_currdep will be set back to
	 * NULL by rangelock_calc_block().
	 */
	if (lock->rl_currdep == NULL)
		lock->rl_currdep = entry;
	rangelock_calc_block(lock);
	while (!(entry->rl_q_flags & RL_LOCK_GRANTED)) {
		if (trylock) {
			/*
			 * For this case, the range is not actually locked
			 * yet, but removal from the list requires the same
			 * steps, except for not doing a rangelock_calc_block()
			 * call, since rangelock_calc_block() was called above.
			 */
			rangelock_unlock_locked(lock, entry, ilk, false);
			return (NULL);
		}
		msleep(entry, ilk, 0, "range", 0);
	}
	mtx_unlock(ilk);
	return (entry);
}

void *
rangelock_rlock(struct rangelock *lock, off_t start, off_t end, struct mtx *ilk)
{

	return (rangelock_enqueue(lock, start, end, RL_LOCK_READ, ilk, false));
}

void *
rangelock_tryrlock(struct rangelock *lock, off_t start, off_t end,
    struct mtx *ilk)
{

	return (rangelock_enqueue(lock, start, end, RL_LOCK_READ, ilk, true));
}

void *
rangelock_wlock(struct rangelock *lock, off_t start, off_t end, struct mtx *ilk)
{

	return (rangelock_enqueue(lock, start, end, RL_LOCK_WRITE, ilk, false));
}

void *
rangelock_trywlock(struct rangelock *lock, off_t start, off_t end,
    struct mtx *ilk)
{

	return (rangelock_enqueue(lock, start, end, RL_LOCK_WRITE, ilk, true));
}

#ifdef INVARIANT_SUPPORT
void
_rangelock_cookie_assert(void *cookie, int what, const char *file, int line)
{
	struct rl_q_entry *entry;
	int flags;

	MPASS(cookie != NULL);
	entry = cookie;
	flags = entry->rl_q_flags;
	switch (what) {
	case RCA_LOCKED:
		if ((flags & RL_LOCK_GRANTED) == 0)
			panic("rangelock not held @ %s:%d\n", file, line);
		break;
	case RCA_RLOCKED:
		if ((flags & (RL_LOCK_GRANTED | RL_LOCK_READ)) !=
		    (RL_LOCK_GRANTED | RL_LOCK_READ))
			panic("rangelock not rlocked @ %s:%d\n", file, line);
		break;
	case RCA_WLOCKED:
		if ((flags & (RL_LOCK_GRANTED | RL_LOCK_WRITE)) !=
		    (RL_LOCK_GRANTED | RL_LOCK_WRITE))
			panic("rangelock not wlocked @ %s:%d\n", file, line);
		break;
	default:
		panic("Unknown rangelock assertion: %d @ %s:%d", what, file,
		    line);
	}
}
#endif	/* INVARIANT_SUPPORT */
