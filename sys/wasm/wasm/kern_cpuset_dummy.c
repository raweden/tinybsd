


// placeholder dummy for sys/kern/kern_cpuset.c

#include <sys/types.h>
#include <sys/pcpu.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <sys/protosw.h>
#include <sys/cpuset.h>
#include <sys/domainset.h>
#include <machine/proc.h>

LIST_HEAD(domainlist, domainset);
struct domainset __read_mostly domainset_firsttouch;
struct domainset __read_mostly domainset_fixed[MAXMEMDOM];
struct domainset __read_mostly domainset_interleave;
struct domainset __read_mostly domainset_prefer[MAXMEMDOM];
struct domainset __read_mostly domainset_roundrobin;

cpuset_t *cpuset_root;
cpuset_t cpuset_domain[MAXMEMDOM];


struct cpuset *cpuset_thread0() {
	return NULL;
}

int	sys_cpuset(struct thread *th, struct cpuset_args *args)
{
	return 0;
}

int	sys_cpuset_setid(struct thread *th, struct cpuset_setid_args *args)
{
	return 0;
}

int	sys_cpuset_getid(struct thread *th, struct cpuset_getid_args *args)
{
	return 0;
}

int	sys_cpuset_getaffinity(struct thread *th, struct cpuset_getaffinity_args *args)
{
	return 0;
}

int	sys_cpuset_setaffinity(struct thread *th, struct cpuset_setaffinity_args *args)
{
	return 0;
}

int	sys_cpuset_getdomain(struct thread *th, struct cpuset_getdomain_args *args)
{
	return 0;
}

int	sys_cpuset_setdomain(struct thread *th, struct cpuset_setdomain_args *args)
{
	return 0;
}

int	cpuset_which(cpuwhich_t which, id_t id, struct proc **p, struct thread **th, struct cpuset **cs)
{
	return 0;
}

struct cpuset *cpuset_ref(struct cpuset *cs)
{
	return 0;
}

int	cpuset_create_root(struct prison *prison, struct cpuset **cs)
{
	return 0;
}

int	cpuset_setproc_update_set(struct proc *p, struct cpuset *cs)
{
	return 0;
}

int	cpuset_setthread(lwpid_t id, cpuset_t *cs)
{
	return 0;
}

int	cpuset_setithread(lwpid_t id, int cpu)
{
	return 0;
}

void cpuset_kernthread(struct thread *th)
{

}

void cpuset_rel(struct cpuset *cs)
{

}

void cpu_copy_thread(struct thread *td, struct thread *td0)
{

}

void cpu_thread_alloc(struct thread *td)
{

}

void cpu_thread_clean(struct thread *td)
{

}

void cpu_thread_exit(struct thread *td)
{

}

void cpu_thread_free(struct thread *td)
{

}

bool cpu_exec_vmspace_reuse(struct proc *p, struct vm_map *map)
{
	return 0;
}

void cpu_exit(struct thread *td)
{

}

void cpu_pcpu_init(struct pcpu *pcpu, int cpuid, size_t size)
{

}

int	cpu_procctl(struct thread *td, int idtype, id_t id, int com, void *data)
{
	return 0;
}

void cpu_set_upcall(struct thread *td, void (*fn)(void *), void *d, stack_t *s)
{

}

int	cpu_set_user_tls(struct thread *td, void *tls_base)
{
	return 0;
}

void cpu_fork_kthread_handler(struct thread *td, void (*fn)(void *), void *data)
{

}
