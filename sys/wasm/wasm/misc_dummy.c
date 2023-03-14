
struct test {
	int var1;
	void *var2;
};

#include <sys/types.h>
#include <sys/pcpu.h>
#include <sys/proc.h>
#include <sys/sched.h>
#include <machine/proc.h>
#include <machine/reg.h>
#include <machine/ucontext.h>
#include <machine/wasm_module.h>

void __panic_abort(void) __WASM_IMPORT(kern, panic_abort);


void spinlock_enter(void)
{

}

void spinlock_exit(void)
{

}

struct pcpu __pcpu[1];
struct pcpu *pcpup;
struct thread *__curthread;
struct thread *__mainthread;
int cold;
char static_env[4] = "\x20\x20\x20\x20";
char static_hints[4] = "\x20\x20\x20\x20";
char machine[] = "wasm32";
extern char *static_kenv;


void cpu_fork(struct thread *th1, struct proc *p, struct thread *th2, int flags)
{

}

int	set_regs(struct thread *th, struct reg *reg)
{
	return 0;
}

int	set_fpregs(struct thread *th, struct fpreg *reg)
{
	return 0;
}

int	set_dbregs(struct thread *th, struct dbreg *reg)
{
	return 0;
}

int	fill_regs(struct thread *th, struct reg *reg)
{
	return 0;
}

int	fill_fpregs(struct thread *th, struct fpreg *reg)
{
	return 0;
}

int	fill_dbregs(struct thread *th, struct dbreg *reg)
{
	return 0;
}

int	fill_regs32(struct thread *th, struct reg32 *reg)
{
	return 0;
}

int	set_regs32(struct thread *th, struct reg32 *reg)
{
	return 0;
}

int	fill_fpregs32(struct thread *th, struct fpreg32 *reg)
{
	return 0;
}

int	set_fpregs32(struct thread *th, struct fpreg32 *reg)
{
	return 0;
}

int	fill_dbregs32(struct thread *th, struct dbreg32 *reg)
{
	return 0;
}

int	set_dbregs32(struct thread *th, struct dbreg32 *reg)
{
	return 0;
}

int set_mcontext(struct thread *th, mcontext_t *ctx)
{
	return 0;
}

int get_mcontext(struct thread *th, mcontext_t *dst, int flags)
{
	return 0;
}

void makectx(struct trapframe *tf, struct pcb *pcb)
{

}

int	savectx(struct pcb *pcb)
{
	return 0;
}

// the rest

#include <sys/epoch.h>
#include <sys/random.h>
#include <sys/sysproto.h>
#include <sys/conf.h> 						// dumperinfo
#include <sys/kerneldump.h>					// minidumpstate
#include <sys/mount.h>						// must be included before freebsd32 for fsid_t
#include <compat/freebsd32/freebsd32.h>		// rusage32

#if 0
int atomic_thread_fence_seq_cst(void)
{

}

int atomic_thread_fence_acq(void)
{

}

int atomic_thread_fence_acq_rel(void)
{

}
#endif

// dmap_phys_base

u_int hc_source_mask;
int do_minidump;

// domainset_roundrobin
// domainset_fixed
// domainset_prefer

int uiomove_fromphys(struct vm_page *ma[], vm_offset_t offset, int n, struct uio *uio)
{
	return 0;
}


void DELAY(int usec)
{
	if (__curthread == __mainthread) {
		__panic_abort();
		return;
	}
}

u_long random(void)
{
	return 0;
}

void random_harvest_queue_(const void *, u_int, enum random_entropy_source)
{

}

void random_harvest_direct_(const void *, u_int, enum random_entropy_source)
{

}

int read_random_uio(struct uio *, bool)
{
	return 0;
}





#if 0
int	wdog_kern_pat(u_int utim)
{
	return 0;
}
#endif





int	ptrace_single_step(struct thread *_td)
{
	return 0;
}

bool in_pcbrele_wlocked(struct inpcb *pcb)
{
	return 0;
}

int convert_sigevent32(struct sigevent32 *sig32, struct sigevent *sig)
{
	return 0;
}

void stack_save(struct stack *)
{

}



void freebsd32_rusage_out(const struct rusage *s, struct rusage32 *s32)
{

}

void ck_epoch_synchronize_wait(ck_epoch_t *epoch, ck_epoch_wait_cb_t *cb, void *data)
{

}

void _ck_epoch_addref(ck_epoch_record_t *rec, ck_epoch_section_t *sec)
{

}

bool _ck_epoch_delref(ck_epoch_record_t *rec, ck_epoch_section_t *sec)
{
	return 0;
}

bool ck_epoch_poll_deferred(struct ck_epoch_record *record, ck_stack_t *deferred)
{

}

void in_pcbref(struct inpcb *)
{

}

void *uma_small_alloc(uma_zone_t zone, vm_size_t bytes, int domain, uint8_t *pflag, int wait)
{
	return NULL;
}

void uma_small_free(void *mem, vm_size_t size, uint8_t flags)
{

}



int	sys_sigreturn(struct thread *, struct sigreturn_args *)
{
	return 0;
}

void dumpsys_map_chunk(vm_paddr_t addr, size_t len, void **a)
{

}


int cpu_minidumpsys(struct dumperinfo *info, const struct minidumpstate *state)
{
	return 0;
}

int	ptrace_clear_single_step(struct thread *_td)
{
	return 0;
}

int	ptrace_set_pc(struct thread *_td, unsigned long _addr)
{
	return 0;
}





// _rt_numfibs

struct tcpcb *tcp_drop(struct tcpcb *tp, int errno)
{
	return NULL;
}



int	freebsd32_copyinuio(struct iovec32 *iovp, u_int iovcnt, struct uio **uiop)
{
	return 0;
}

void ck_epoch_init(struct ck_epoch *global) // ck_epoch.c
{

}

void ck_epoch_register(struct ck_epoch *global, struct ck_epoch_record *record, void *ct) // ck_epoch.c
{

}



// Open Crypto (ktls)

int ktls_ocf_try(struct socket *so, struct ktls_session *tls, int direction)
{
	return 0;
}

void ktls_ocf_free(struct ktls_session *tls)
{

}

int ktls_ocf_encrypt(struct ktls_ocf_encrypt_state *state, struct ktls_session *tls, struct mbuf *m, struct iovec *outiov, int outiovcnt)
{
	return 0;
}

bool ktls_ocf_recrypt_supported(struct ktls_session *tls)
{
	return 0;
}

void sballoc_ktls_rx(struct sockbuf *sb, struct mbuf *m)
{

}

void sbfree_ktls_rx(struct sockbuf *sb, struct mbuf *m)
{

}

int ktls_ocf_recrypt(struct ktls_session *tls, const struct tls_record_layer *hdr, struct mbuf *m, uint64_t seqno)
{
	return 0;
}

int ktls_ocf_decrypt(struct ktls_session *tls, const struct tls_record_layer *hdr, struct mbuf *m, uint64_t seqno, int *trailer_len)
{
	return 0;
}

// sys/compat/freebsd32/freebsd32_misc.c
int syscall32_helper_register(struct syscall_helper_data *sd, int flags)
{
	return 0;
}

int syscall32_helper_unregister(struct syscall_helper_data *sd)
{
	return 0;
}

//

// placeholder for WebAssembly main thread callsite.
void wasm_main_callsite(void (*fn)(void))
{
	fn();
}

void kthread_dispatch_sync(int (*fn)(void)) __WASM_IMPORT(kern, kthread_dispatch_sync);

__WASM_EXPORT(wasm_mainthr_test)
int wasm_mainthr_test(void) {
	if (__curthread != __mainthread) {
		kthread_dispatch_sync(wasm_mainthr_test);
		return 321;
	}

	return 123;
}

