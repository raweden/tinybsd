
#include <sys/types.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/sysproto.h>

#include <machine/proc.h>
#include <machine/wasm_module.h>

void __panic_abort(void) __WASM_IMPORT(kern, panic_abort);

void sched_prio(struct thread *td, u_char prio)
{
	__panic_abort();
}

void sched_lend_user_prio(struct thread *td, u_char pri)
{
	__panic_abort();
}

void sched_bind(struct thread *td, int cpu)
{
	__panic_abort();
}

void sched_add(struct thread *td, int flags)
{
	__panic_abort();
}

fixpt_t	sched_pctcpu(struct thread *td)
{
	return NULL;
}

void sched_unbind(struct thread *td)
{
	__panic_abort();
}

void sched_wakeup(struct thread *td, int srqflags)
{
	__panic_abort();
}

void sched_sleep(struct thread *td, int prio)
{
	__panic_abort();
}

void sched_class(struct thread *td, int class)
{
	__panic_abort();
}

void sched_user_prio(struct thread *td, u_char prio)
{
	__panic_abort();
}

void sched_fork_thread(struct thread *td, struct thread *child)
{
	__panic_abort();
}

u_int sched_estcpu(struct thread *td)
{
	__panic_abort();
	return 0;
}

void sched_nice(struct proc *p, int nice)
{
	__panic_abort();
}

struct thread *sched_choose(void)
{
	__panic_abort();
}

void sched_lend_prio(struct thread *td, u_char prio)
{
	__panic_abort();
}

void sched_unlend_prio(struct thread *td, u_char prio)
{
	__panic_abort();
}

void sched_clock(struct thread *td, int cnt)
{
	__panic_abort();
}

void sched_exit(struct proc *p, struct thread *childtd)
{
	__panic_abort();
}

void sched_fork(struct thread *td, struct thread *childtd)
{
	__panic_abort();
}

void sched_fork_exit(struct thread *td)
{
	__panic_abort();
}

void sched_ithread_prio(struct thread *td, u_char prio)
{
	__panic_abort();
}

int	sched_sizeof_proc(void)
{
	return KINFO_PROC_SIZE;
}

int	sched_sizeof_thread(void)
{
	return sizeof(struct thread);
}

void sched_switch(struct thread *td, int flags)
{
	__panic_abort();
}

void sched_exit_thread(struct thread *td, struct thread *child)
{
	__panic_abort();
}

void sched_throw(struct thread *td)
{
	__panic_abort();
}

void sched_relinquish(struct thread *td)
{
	__panic_abort();
}

void sched_lend_user_prio_cond(struct thread *td, u_char pri)
{
	__panic_abort();
}

int	sched_is_bound(struct thread *td)
{
	__panic_abort();
}

void sched_userret_slowpath(struct thread *td)
{
	__panic_abort();
}


// placeholder for sys/kern/p1003_1b.c

int	sys_sched_setparam(struct thread *th, struct sched_setparam_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_getparam(struct thread *th, struct sched_getparam_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_setscheduler(struct thread *th, struct sched_setscheduler_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_getscheduler(struct thread *th, struct sched_getscheduler_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_yield(struct thread *th, struct sched_yield_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_get_priority_max(struct thread *th, struct sched_get_priority_max_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_get_priority_min(struct thread *th, struct sched_get_priority_min_args *args)
{
	__panic_abort();
	return 0;
}

int	sys_sched_rr_get_interval(struct thread *th, struct sched_rr_get_interval_args *args)
{
	__panic_abort();
	return 0;
}


