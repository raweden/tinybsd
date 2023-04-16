
#include <sys/types.h>
#include <machine/atomic_wasm.h>

#define MUTEX_WAIT 0
#define MUTEX_AWAKE 1

struct mtx_waiter;

// mockup for the lock_object defined in <sys/_lock.h>
struct lock_object2 {
    struct mtx_waiter *mtx_wq;
};

struct mtx_waiter {
    uint32_t mw_cond;
    struct mtx_waiter *mw_next; // use ring-buffer like struct list in linux.
};

// as usage of the pool must also be sequential, wp_usage indicates the usage of the pool. ops on this int must be atomic
// the enter condition is 0 zero, if not equal to 0 zero the the thread allocating the lock should wait until zero is reached.
// the release condition is wp_usage == 0 && wp_bitmap == 0
// 
// a alternative approach would be to have mtx_waiter present in the memory managed by the owning thread.
// 

struct mtx_waiter_pool {
    uint32_t wp_usage;      // number of threads using the pool
    uint32_t wp_bitmap;
    struct mtx_waiter_pool *wp_next;
    struct mtx_waiter wp_waiter[32];
};

void __mtx_lock2(struct lock_object2 *mtx)
{
    struct mtx_waiter *wq;
    atomic_wait32(&wq->mw_cond, MUTEX_WAIT, 0);
}

void __mtx_unlock2(struct lock_object2 *mtx)
{
    struct mtx_waiter wq* = mtx->mtx_wq;
    atomic_notify(&wq.mw_cond, 1);
}

