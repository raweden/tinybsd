/*-
 * SPDX-License-Identifier: BSD-4-Clause
 *
 * Copyright (c) 1991 Regents of the University of California.
 * All rights reserved.
 * Copyright (c) 1994 John S. Dyson
 * All rights reserved.
 * Copyright (c) 1994 David Greenman
 * All rights reserved.
 * Copyright (c) 2003 Peter Wemm
 * All rights reserved.
 * Copyright (c) 2005-2010 Alan L. Cox <alc@cs.rice.edu>
 * All rights reserved.
 * Copyright (c) 2014 Andrew Turner
 * All rights reserved.
 * Copyright (c) 2014 The FreeBSD Foundation
 * All rights reserved.
 * Copyright (c) 2015-2018 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * the Systems Programming Group of the University of Utah Computer
 * Science Department and William Jolitz of UUNET Technologies Inc.
 *
 * Portions of this software were developed by Andrew Turner under
 * sponsorship from The FreeBSD Foundation.
 *
 * Portions of this software were developed by SRI International and the
 * University of Cambridge Computer Laboratory under DARPA/AFRL contract
 * FA8750-10-C-0237 ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Portions of this software were developed by the University of Cambridge
 * Computer Laboratory as part of the CTSRD Project, with support from the
 * UK Higher Education Innovation Fund (HEIF).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	from:	@(#)pmap.c	7.7 (Berkeley)	5/12/91
 */
/*-
 * Copyright (c) 2003 Networks Associates Technology, Inc.
 * All rights reserved.
 *
 * This software was developed for the FreeBSD Project by Jake Burkholder,
 * Safeport Network Services, and Network Associates Laboratories, the
 * Security Research Division of Network Associates, Inc. under
 * DARPA/SPAWAR contract N66001-01-C-8035 ("CBOSS"), as part of the DARPA
 * CHATS research program.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include "sys/types.h"
__FBSDID("$FreeBSD$");

/*
 *	Manages physical address maps.
 *
 *	Since the information managed by this module is
 *	also stored by the logical address mapping module,
 *	this module may throw away valid virtual-to-physical
 *	mappings at almost any time.  However, invalidations
 *	of virtual-to-physical mappings must be done as
 *	requested.
 *
 *	In order to cope with hardware architectures which
 *	make virtual-to-physical map invalidates expensive,
 *	this module may delay invalidate or reduced protection
 *	operations until such time as they are actually
 *	necessary.  This module is given full information as
 *	to which processors are currently using which maps,
 *	and to when physical maps must be made correct.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/bitstring.h>
#include <sys/bus.h>
#include <sys/cpuset.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mman.h>
#include <sys/msgbuf.h>
#include <sys/mutex.h>
#include <sys/physmem.h>
#include <sys/proc.h>
#include <sys/rwlock.h>
#include <sys/sbuf.h>
#include <sys/sx.h>
#include <sys/vmem.h>
#include <sys/vmmeter.h>
#include <sys/sched.h>
#include <sys/sysctl.h>
#include <sys/smp.h>

#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/vm_kern.h>
#include <vm/vm_page.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vm_extern.h>
#include <vm/vm_pageout.h>
#include <vm/vm_pager.h>
#include <vm/vm_phys.h>
#include <vm/vm_radix.h>
#include <vm/vm_reserv.h>
#include <vm/vm_dumpset.h>
#include <vm/uma.h>

//#include <machine/machdep.h>
#include <machine/md_var.h>
#include <machine/pcb.h>
//#include <machine/sbi.h>

/*
 * Boundary values for the page table page index space:
 *
 * L3 pages: [0, NUL2E)
 * L2 pages: [NUL2E, NUL2E + NUL1E)
 * L1 pages: [NUL2E + NUL1E, NUL2E + NUL1E + NUL0E)
 *
 * Note that these ranges are used in both SV39 and SV48 mode.  In SV39 mode the
 * ranges are not fully populated since there are at most Ln_ENTRIES^2 L3 pages
 * in a set of page tables.
 */
#define	NUL0E		Ln_ENTRIES
#define	NUL1E		(Ln_ENTRIES * NUL0E)
#define	NUL2E		(Ln_ENTRIES * NUL1E)

#if !defined(DIAGNOSTIC)
#ifdef __GNUC_GNU_INLINE__
#define PMAP_INLINE	__attribute__((__gnu_inline__)) inline
#else
#define PMAP_INLINE	extern inline
#endif
#else
#define PMAP_INLINE
#endif

#ifdef PV_STATS
#define PV_STAT(x)	do { x ; } while (0)
#else
#define PV_STAT(x)	do { } while (0)
#endif

#define	pmap_l1_pindex(v)	(NUL2E + ((v) >> L1_SHIFT))
#define	pmap_l2_pindex(v)	((v) >> L2_SHIFT)
#define	pa_to_pvh(pa)		(&pv_table[pa_index(pa)])

#define	NPV_LIST_LOCKS	MAXCPU

#define	PHYS_TO_PV_LIST_LOCK(pa)	\
			(&pv_list_locks[pmap_l2_pindex(pa) % NPV_LIST_LOCKS])

#define	CHANGE_PV_LIST_LOCK_TO_PHYS(lockp, pa)	do {	\
	struct rwlock **_lockp = (lockp);		\
	struct rwlock *_new_lock;			\
							\
	_new_lock = PHYS_TO_PV_LIST_LOCK(pa);		\
	if (_new_lock != *_lockp) {			\
		if (*_lockp != NULL)			\
			rw_wunlock(*_lockp);		\
		*_lockp = _new_lock;			\
		rw_wlock(*_lockp);			\
	}						\
} while (0)

#define	CHANGE_PV_LIST_LOCK_TO_VM_PAGE(lockp, m)	\
			CHANGE_PV_LIST_LOCK_TO_PHYS(lockp, VM_PAGE_TO_PHYS(m))

#define	RELEASE_PV_LIST_LOCK(lockp)		do {	\
	struct rwlock **_lockp = (lockp);		\
							\
	if (*_lockp != NULL) {				\
		rw_wunlock(*_lockp);			\
		*_lockp = NULL;				\
	}						\
} while (0)

#define	VM_PAGE_TO_PV_LIST_LOCK(m)	\
			PHYS_TO_PV_LIST_LOCK(VM_PAGE_TO_PHYS(m))

static SYSCTL_NODE(_vm, OID_AUTO, pmap, CTLFLAG_RD | CTLFLAG_MPSAFE, 0,
    "VM/pmap parameters");

/* The list of all the user pmaps */
LIST_HEAD(pmaplist, pmap);
static struct pmaplist allpmaps = LIST_HEAD_INITIALIZER();

enum pmap_mode __read_frequently pmap_mode = PMAP_MODE_SV39;
SYSCTL_INT(_vm_pmap, OID_AUTO, mode, CTLFLAG_RDTUN | CTLFLAG_NOFETCH,
    &pmap_mode, 0,
    "translation mode, 0 = SV39, 1 = SV48");

struct pmap kernel_pmap_store;

vm_offset_t virtual_avail;	/* VA of first avail page (after kernel bss) */
vm_offset_t virtual_end;	/* VA of last avail page (end of kernel AS) */
vm_offset_t kernel_vm_end = 0;
vm_offset_t init_pt_va;

vm_paddr_t dmap_phys_base;	/* The start of the dmap region */
vm_paddr_t dmap_phys_max;	/* The limit of the dmap region */
vm_offset_t dmap_max_addr;	/* The virtual address limit of the dmap */

static struct rwlock_padalign pvh_global_lock;
static struct mtx_padalign allpmaps_lock;

static int superpages_enabled = 1;
SYSCTL_INT(_vm_pmap, OID_AUTO, superpages_enabled,
    CTLFLAG_RDTUN, &superpages_enabled, 0,
    "Enable support for transparent superpages");

static SYSCTL_NODE(_vm_pmap, OID_AUTO, l2, CTLFLAG_RD | CTLFLAG_MPSAFE, 0,
    "2MB page mapping counters");

static u_long pmap_l2_demotions;
SYSCTL_ULONG(_vm_pmap_l2, OID_AUTO, demotions, CTLFLAG_RD,
    &pmap_l2_demotions, 0,
    "2MB page demotions");

static u_long pmap_l2_mappings;
SYSCTL_ULONG(_vm_pmap_l2, OID_AUTO, mappings, CTLFLAG_RD,
    &pmap_l2_mappings, 0,
    "2MB page mappings");

static u_long pmap_l2_p_failures;
SYSCTL_ULONG(_vm_pmap_l2, OID_AUTO, p_failures, CTLFLAG_RD,
    &pmap_l2_p_failures, 0,
    "2MB page promotion failures");

static u_long pmap_l2_promotions;
SYSCTL_ULONG(_vm_pmap_l2, OID_AUTO, promotions, CTLFLAG_RD,
    &pmap_l2_promotions, 0,
    "2MB page promotions");

/*
 * Data for the pv entry allocation mechanism
 */
static TAILQ_HEAD(pch, pv_chunk) pv_chunks = TAILQ_HEAD_INITIALIZER(pv_chunks);
static struct mtx pv_chunks_mutex;
static struct rwlock pv_list_locks[NPV_LIST_LOCKS];
static struct md_page *pv_table;
static struct md_page pv_dummy;

extern cpuset_t all_harts;

/*
 * Internal flags for pmap_enter()'s helper functions.
 */
#define	PMAP_ENTER_NORECLAIM	0x1000000	/* Don't reclaim PV entries. */
#define	PMAP_ENTER_NOREPLACE	0x2000000	/* Don't replace mappings. */

static void	free_pv_chunk(struct pv_chunk *pc);
static void	free_pv_entry(pmap_t pmap, pv_entry_t pv);
static pv_entry_t get_pv_entry(pmap_t pmap, struct rwlock **lockp);
static vm_page_t reclaim_pv_chunk(pmap_t locked_pmap, struct rwlock **lockp);
static void	pmap_pvh_free(struct md_page *pvh, pmap_t pmap, vm_offset_t va);
static pv_entry_t pmap_pvh_remove(struct md_page *pvh, pmap_t pmap,
		    vm_offset_t va);
static bool	pmap_demote_l2(pmap_t pmap, pd_entry_t *l2, vm_offset_t va);
static bool	pmap_demote_l2_locked(pmap_t pmap, pd_entry_t *l2,
		    vm_offset_t va, struct rwlock **lockp);
static int	pmap_enter_l2(pmap_t pmap, vm_offset_t va, pd_entry_t new_l2,
		    u_int flags, vm_page_t m, struct rwlock **lockp);
static vm_page_t pmap_enter_quick_locked(pmap_t pmap, vm_offset_t va,
    vm_page_t m, vm_prot_t prot, vm_page_t mpte, struct rwlock **lockp);
static int pmap_remove_l3(pmap_t pmap, pt_entry_t *l3, vm_offset_t sva,
    pd_entry_t ptepde, struct spglist *free, struct rwlock **lockp);
static boolean_t pmap_try_insert_pv_entry(pmap_t pmap, vm_offset_t va,
    vm_page_t m, struct rwlock **lockp);

static vm_page_t _pmap_alloc_l3(pmap_t pmap, vm_pindex_t ptepindex,
		struct rwlock **lockp);

static void _pmap_unwire_ptp(pmap_t pmap, vm_offset_t va, vm_page_t m,
    struct spglist *free);
static int pmap_unuse_pt(pmap_t, vm_offset_t, pd_entry_t, struct spglist *);

static int pmap_change_attr_locked(vm_offset_t va, vm_size_t size, int mode);

#define	pmap_clear(pte)			pmap_store(pte, 0)
#define	pmap_clear_bits(pte, bits)	atomic_clear_64(pte, bits)
#define	pmap_load_store(pte, entry)	atomic_swap_64(pte, entry)
#define	pmap_load_clear(pte)		pmap_load_store(pte, 0)
#define	pmap_load(pte)			atomic_load_64(pte)
#define	pmap_store(pte, entry)		atomic_store_64(pte, entry)
#define	pmap_store_bits(pte, bits)	atomic_set_64(pte, bits)

/********************/
/* Inline functions */
/********************/

static __inline void
pagecopy(void *s, void *d)
{

	memcpy(d, s, PAGE_SIZE);
}

static __inline void
pagezero(void *p)
{

	bzero(p, PAGE_SIZE);
}

#define	pmap_l0_index(va)	(((va) >> L0_SHIFT) & Ln_ADDR_MASK)
#define	pmap_l1_index(va)	(((va) >> L1_SHIFT) & Ln_ADDR_MASK)
#define	pmap_l2_index(va)	(((va) >> L2_SHIFT) & Ln_ADDR_MASK)
#define	pmap_l3_index(va)	(((va) >> L3_SHIFT) & Ln_ADDR_MASK)

#define	PTE_TO_PHYS(pte) \
    ((((pte) & ~PTE_HI_MASK) >> PTE_PPN0_S) * PAGE_SIZE)
#define	L2PTE_TO_PHYS(l2) \
    ((((l2) & ~PTE_HI_MASK) >> PTE_PPN1_S) << L2_SHIFT)

static __inline pd_entry_t *
pmap_l0(pmap_t pmap, vm_offset_t va)
{
	KASSERT(pmap_mode != PMAP_MODE_SV39, ("%s: in SV39 mode", __func__));
	KASSERT(VIRT_IS_VALID(va),
	    ("%s: malformed virtual address %#lx", __func__, va));
	return (&pmap->pm_top[pmap_l0_index(va)]);
}

static __inline pd_entry_t *
pmap_l0_to_l1(pd_entry_t *l0, vm_offset_t va)
{
	vm_paddr_t phys;
	pd_entry_t *l1;

	KASSERT(pmap_mode != PMAP_MODE_SV39, ("%s: in SV39 mode", __func__));
	phys = PTE_TO_PHYS(pmap_load(l0));
	l1 = (pd_entry_t *)PHYS_TO_DMAP(phys);

	return (&l1[pmap_l1_index(va)]);
}

static __inline pd_entry_t *
pmap_l1(pmap_t pmap, vm_offset_t va)
{
	pd_entry_t *l0;

	KASSERT(VIRT_IS_VALID(va),
	    ("%s: malformed virtual address %#lx", __func__, va));
	if (pmap_mode == PMAP_MODE_SV39) {
		return (&pmap->pm_top[pmap_l1_index(va)]);
	} else {
		l0 = pmap_l0(pmap, va);
		if ((pmap_load(l0) & PTE_V) == 0)
			return (NULL);
		if ((pmap_load(l0) & PTE_RX) != 0)
			return (NULL);
		return (pmap_l0_to_l1(l0, va));
	}
}

static __inline pd_entry_t *
pmap_l1_to_l2(pd_entry_t *l1, vm_offset_t va)
{
	vm_paddr_t phys;
	pd_entry_t *l2;

	phys = PTE_TO_PHYS(pmap_load(l1));
	l2 = (pd_entry_t *)PHYS_TO_DMAP(phys);

	return (&l2[pmap_l2_index(va)]);
}

static __inline pd_entry_t *
pmap_l2(pmap_t pmap, vm_offset_t va)
{
	pd_entry_t *l1;

	l1 = pmap_l1(pmap, va);
	if (l1 == NULL)
		return (NULL);
	if ((pmap_load(l1) & PTE_V) == 0)
		return (NULL);
	if ((pmap_load(l1) & PTE_RX) != 0)
		return (NULL);

	return (pmap_l1_to_l2(l1, va));
}

static __inline pt_entry_t *
pmap_l2_to_l3(pd_entry_t *l2, vm_offset_t va)
{
	vm_paddr_t phys;
	pt_entry_t *l3;

	phys = PTE_TO_PHYS(pmap_load(l2));
	l3 = (pd_entry_t *)PHYS_TO_DMAP(phys);

	return (&l3[pmap_l3_index(va)]);
}

static __inline pt_entry_t *
pmap_l3(pmap_t pmap, vm_offset_t va)
{
	pd_entry_t *l2;

	l2 = pmap_l2(pmap, va);
	if (l2 == NULL)
		return (NULL);
	if ((pmap_load(l2) & PTE_V) == 0)
		return (NULL);
	if ((pmap_load(l2) & PTE_RX) != 0)
		return (NULL);

	return (pmap_l2_to_l3(l2, va));
}


static pt_entry_t *
pmap_early_page_idx(vm_offset_t l1pt, vm_offset_t va, u_int *l1_slot,
    u_int *l2_slot)
{
	pt_entry_t *l2;
	pd_entry_t *l1 __diagused;

	l1 = (pd_entry_t *)l1pt;
	*l1_slot = (va >> L1_SHIFT) & Ln_ADDR_MASK;

	/* Check locore has used a table L1 map */
	KASSERT((l1[*l1_slot] & PTE_RX) == 0,
		("Invalid bootstrap L1 table"));

	/* Find the address of the L2 table */
	l2 = (pt_entry_t *)init_pt_va;
	*l2_slot = pmap_l2_index(va);

	return (l2);
}

static vm_paddr_t
pmap_early_vtophys(vm_offset_t l1pt, vm_offset_t va)
{
	u_int l1_slot, l2_slot;
	pt_entry_t *l2;
	vm_paddr_t ret;

	l2 = pmap_early_page_idx(l1pt, va, &l1_slot, &l2_slot);

	/* Check locore has used L2 superpages */
	KASSERT((l2[l2_slot] & PTE_RX) != 0,
		("Invalid bootstrap L2 table"));

	/* L2 is superpages */
	ret = L2PTE_TO_PHYS(l2[l2_slot]);
	ret += (va & L2_OFFSET);

	return (ret);
}

static void
pmap_bootstrap_dmap(vm_offset_t kern_l1, vm_paddr_t min_pa, vm_paddr_t max_pa)
{
#if 0
	vm_offset_t va;
	vm_paddr_t pa;
	pd_entry_t *l1;
	u_int l1_slot;
	pt_entry_t entry;
	pn_t pn;

	pa = dmap_phys_base = min_pa & ~L1_OFFSET;
	va = DMAP_MIN_ADDRESS;
	l1 = (pd_entry_t *)kern_l1;
	l1_slot = pmap_l1_index(DMAP_MIN_ADDRESS);

	for (; va < DMAP_MAX_ADDRESS && pa < max_pa;
	    pa += L1_SIZE, va += L1_SIZE, l1_slot++) {
		KASSERT(l1_slot < Ln_ENTRIES, ("Invalid L1 index"));

		/* superpages */
		pn = (pa / PAGE_SIZE);
		entry = PTE_KERN;
		entry |= (pn << PTE_PPN0_S);
		pmap_store(&l1[l1_slot], entry);
	}

	/* Set the upper limit of the DMAP region */
	dmap_phys_max = pa;
	dmap_max_addr = va;

	sfence_vma();
#endif
}

static vm_offset_t
pmap_bootstrap_l3(vm_offset_t l1pt, vm_offset_t va, vm_offset_t l3_start)
{
#if 0
	vm_offset_t l3pt;
	pt_entry_t entry;
	pd_entry_t *l2;
	vm_paddr_t pa;
	u_int l2_slot;
	pn_t pn;

	KASSERT((va & L2_OFFSET) == 0, ("Invalid virtual address"));

	l2 = pmap_l2(kernel_pmap, va);
	l2 = (pd_entry_t *)((uintptr_t)l2 & ~(PAGE_SIZE - 1));
	l2_slot = pmap_l2_index(va);
	l3pt = l3_start;

	for (; va < VM_MAX_KERNEL_ADDRESS; l2_slot++, va += L2_SIZE) {
		KASSERT(l2_slot < Ln_ENTRIES, ("Invalid L2 index"));

		pa = pmap_early_vtophys(l1pt, l3pt);
		pn = (pa / PAGE_SIZE);
		entry = (PTE_V);
		entry |= (pn << PTE_PPN0_S);
		pmap_store(&l2[l2_slot], entry);
		l3pt += PAGE_SIZE;
	}

	/* Clean the L2 page table */
	memset((void *)l3_start, 0, l3pt - l3_start);

	return (l3pt);
#endif
}

/**
 *
 */
void pmap_bootstrap(vm_offset_t l1pt, vm_paddr_t kernstart, vm_size_t kernlen)
{
    vm_paddr_t physmap[PHYS_AVAIL_ENTRIES];
	uint64_t satp;
	vm_offset_t dpcpu, freemempos, l0pv, msgbufpv;
	vm_paddr_t l0pa, l1pa, max_pa, min_pa, pa;
	pd_entry_t *l0p;
	pt_entry_t *l2p;
	u_int l1_slot, l2_slot;
	u_int physmap_idx;
	int i, mode;

	printf("pmap_bootstrap %lx %lx %lx\n", (unsigned long)l1pt, (unsigned long)kernstart, (unsigned long)kernlen);

	/* Set this early so we can use the pagetable walking functions */
	kernel_pmap_store.pm_top = (pd_entry_t *)l1pt;
	PMAP_LOCK_INIT(kernel_pmap);

	rw_init(&pvh_global_lock, "pmap pv global");

	/*
	 * Set the current CPU as active in the kernel pmap. Secondary cores
	 * will add themselves later in init_secondary(). The SBI firmware
	 * may rely on this mask being precise, so CPU_FILL() is not used.
	 */
	//CPU_SET(PCPU_GET(hart), &kernel_pmap->pm_active);

	/* Assume the address we were loaded to is a valid physical address. */
	min_pa = max_pa = kernstart;

	physmap_idx = physmem_avail(physmap, nitems(physmap));
	physmap_idx /= 2;

	/*
	 * Find the minimum physical address. physmap is sorted,
	 * but may contain empty ranges.
	 */
	for (i = 0; i < physmap_idx * 2; i += 2) {
		if (physmap[i] == physmap[i + 1])
			continue;
		if (physmap[i] <= min_pa)
			min_pa = physmap[i];
		if (physmap[i + 1] > max_pa)
			max_pa = physmap[i + 1];
	}
	printf("physmap_idx %u\n", physmap_idx);
	printf("min_pa %lx\n", (unsigned long)min_pa);
	printf("max_pa %lx\n", (unsigned long)max_pa);

	/* Create a direct map region early so we can use it for pa -> va */
	pmap_bootstrap_dmap(l1pt, min_pa, max_pa);

	/*
	 * Read the page table to find out what is already mapped.
	 * This assumes we have mapped a block of memory from KERNBASE
	 * using a single L1 entry.
	 */
	(void)pmap_early_page_idx(l1pt, KERNBASE, &l1_slot, &l2_slot);

	/* Sanity check the index, KERNBASE should be the first VA */
	KASSERT(l2_slot == 0, ("The L2 index is non-zero"));

	freemempos = roundup2(kernstart + kernlen, PAGE_SIZE);

#ifndef __WASM
	/* Create the l3 tables for the early devmap */
	freemempos = pmap_bootstrap_l3(l1pt, VM_MAX_KERNEL_ADDRESS - L2_SIZE, freemempos);
#endif

	/*
	 * Invalidate the mapping we created for the DTB. At this point a copy
	 * has been created, and we no longer need it. We want to avoid the
	 * possibility of an aliased mapping in the future.
	 */
	l2p = pmap_l2(kernel_pmap, VM_EARLY_DTB_ADDRESS);
	if ((pmap_load(l2p) & PTE_V) != 0)
		pmap_clear(l2p);

	sfence_vma();

#define alloc_pages(var, np)						\
	(var) = freemempos;						        \
	freemempos += (np * PAGE_SIZE);					\
	memset((char *)(var), 0, ((np) * PAGE_SIZE));

    // TODO: we do not need to zero-initialize memory in wasm.

#ifndef __WASM
	mode = 0;
	TUNABLE_INT_FETCH("vm.pmap.mode", &mode);
	if (mode == PMAP_MODE_SV48) {
		/*
		 * Enable SV48 mode: allocate an L0 page and set SV48 mode in
		 * SATP.  If the implementation does not provide SV48 mode,
		 * the mode read back from the (WARL) SATP register will be
		 * unchanged, and we continue in SV39 mode.
		 */
		alloc_pages(l0pv, 1);
		l0p = (void *)l0pv;
		l1pa = pmap_early_vtophys(l1pt, l1pt);
		l0p[pmap_l0_index(KERNBASE)] = PTE_V | PTE_A | PTE_D |
		    ((l1pa >> PAGE_SHIFT) << PTE_PPN0_S);

		l0pa = pmap_early_vtophys(l1pt, l0pv);
		csr_write(satp, (l0pa >> PAGE_SHIFT) | SATP_MODE_SV48);
		satp = csr_read(satp);
		if ((satp & SATP_MODE_M) == SATP_MODE_SV48) {
			pmap_mode = PMAP_MODE_SV48;
			kernel_pmap_store.pm_top = l0p;
		} else {
			/* Mode didn't change, give the page back. */
			freemempos -= PAGE_SIZE;
		}
	}
#endif

	/* Allocate dynamic per-cpu area. */
	alloc_pages(dpcpu, DPCPU_SIZE / PAGE_SIZE);
	dpcpu_init((void *)dpcpu, 0);

	/* Allocate memory for the msgbuf, e.g. for /sbin/dmesg */
	alloc_pages(msgbufpv, round_page(msgbufsize) / PAGE_SIZE);
	msgbufp = (void *)msgbufpv;

	virtual_avail = roundup2(freemempos, L2_SIZE);
	virtual_end = VM_MAX_KERNEL_ADDRESS - L2_SIZE;
	kernel_vm_end = virtual_avail;

	pa = pmap_early_vtophys(l1pt, freemempos);

	physmem_exclude_region(kernstart, pa - kernstart, EXFLAG_NOALLOC);
}

/*
 *	Initialize a vm_page's machine-dependent fields.
 */
void
pmap_page_init(vm_page_t m)
{

	TAILQ_INIT(&m->md.pv_list);
	m->md.pv_memattr = VM_MEMATTR_WRITE_BACK;
}

/*
 *	Initialize the pmap module.
 *	Called by vm_init, to initialize any structures that the pmap
 *	system needs to map virtual memory.
 */
void
pmap_init(void)
{
	vm_size_t s;
	int i, pv_npg;

	/*
	 * Initialize the pv chunk and pmap list mutexes.
	 */
	mtx_init(&pv_chunks_mutex, "pmap pv chunk list", NULL, MTX_DEF);
	mtx_init(&allpmaps_lock, "allpmaps", NULL, MTX_DEF);

	/*
	 * Initialize the pool of pv list locks.
	 */
	for (i = 0; i < NPV_LIST_LOCKS; i++)
		rw_init(&pv_list_locks[i], "pmap pv list");

	/*
	 * Calculate the size of the pv head table for superpages.
	 */
	pv_npg = howmany(vm_phys_segs[vm_phys_nsegs - 1].end, L2_SIZE);

	/*
	 * Allocate memory for the pv head table for superpages.
	 */
	s = (vm_size_t)(pv_npg * sizeof(struct md_page));
	s = round_page(s);
	pv_table = kmem_malloc(s, M_WAITOK | M_ZERO);
	for (i = 0; i < pv_npg; i++)
		TAILQ_INIT(&pv_table[i].pv_list);
	TAILQ_INIT(&pv_dummy.pv_list);

	if (superpages_enabled)
		pagesizes[1] = L2_SIZE;
}