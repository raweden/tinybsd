

#ifndef _MACHINE_PROC_H_
#define _MACHINE_PROC_H_

struct mdthread {
	int	md_spinlock_count;	/* (k) */
	register_t md_saved_sstatus_ie;	/* (k) */
};

struct mdproc {
	int dummy;
};

#define	KINFO_PROC_SIZE		768 /* should be 1088*/
#define	KINFO_PROC32_SIZE	816

#ifdef _KERNEL
#include <machine/pcb.h>

/* Get the current kernel thread stack usage. */
#define	GET_STACK_USAGE(total, used) do {				\
	struct thread *td = curthread;					\
	(total) = td->td_kstack_pages * PAGE_SIZE - sizeof(struct pcb);	\
	(used) = td->td_kstack + (total) - (vm_offset_t)&td;		\
} while (0)

#endif  /* _KERNEL */
#endif /* _MACHINE_PROC_H_ */
