
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/buf.h>
#include <sys/cons.h>
#include <sys/devmap.h>
#include <sys/kdb.h>
#include <sys/kernel.h>
#include <sys/linker.h>
#include <sys/msgbuf.h>
#include <sys/physmem.h>
#include <sys/proc.h>
#include <sys/reboot.h>
#include <sys/rwlock.h>
#include <sys/tslog.h>
#include "sys/pcpu.h"
#include <sys/vmmeter.h>

#include <vm/vm.h>
#include <vm/vm_param.h>
#include <vm/vm_kern.h>
#include <vm/vm_object.h>
#include <vm/vm_page.h>
#include <vm/vm_phys.h>
#include <vm/pmap.h>
#include <vm/vm_map.h>
#include <vm/vm_pager.h>

static struct trapframe proc0_tf;

int early_boot = 1;

struct kva_md_info kmi;

extern struct thread *__curthread;
extern struct thread *__mainthread;

char static_kenv[PAGE_SIZE] = {0x20, 0x20}; // to prevent static_kenv to end up in .bss

struct wasm_bootparams {
    vm_offset_t	kern_l1pt;	/* Kernel L1 base */
	vm_offset_t	kern_phys;	/* Kernel base (physical) addr */
	vm_offset_t	kern_stack;
	vm_offset_t	dtbp_virt;	/* Device tree blob virtual addr */
	vm_offset_t	dtbp_phys;	/* Device tree blob physical addr */
	vm_offset_t	modulep;	/* loader(8) metadata */
    vm_offset_t	datamin;
    vm_offset_t	datamax;
};

static struct wasm_bootparams __static_wabp;

void identify_cpu(void)
{

}

static void
cpu_startup(void *dummy)
{

	identify_cpu();

	printf("real memory  = %ju (%ju MB)\n", ptoa((uintmax_t)realmem), ptoa((uintmax_t)realmem) / (1024 * 1024));

	/*
	 * Display any holes after the first chunk of extended memory.
	 */
	if (bootverbose) {
		int indx;

		printf("Physical memory chunk(s):\n");
		for (indx = 0; phys_avail[indx + 1] != 0; indx += 2) {
			vm_paddr_t size;

			size = phys_avail[indx + 1] - phys_avail[indx];
			printf(
			    "0x%016jx - 0x%016jx, %ju bytes (%ju pages)\n",
			    (uintmax_t)phys_avail[indx],
			    (uintmax_t)phys_avail[indx + 1] - 1,
			    (uintmax_t)size, (uintmax_t)size / PAGE_SIZE);
		}
	}

	vm_ksubmap_init(&kmi);

	printf("avail memory = %ju (%ju MB)\n",
	    ptoa((uintmax_t)vm_free_count()),
	    ptoa((uintmax_t)vm_free_count()) / (1024 * 1024));
	if (bootverbose)
		devmap_print_table();

	bufinit();
	vm_pager_bufferinit();
}

SYSINIT(cpu, SI_SUB_CPU, SI_ORDER_FIRST, cpu_startup, NULL);

void mi_startup(void);

static void
init_proc0(vm_offset_t kstack)
{
	struct pcpu *pcpup;

	pcpup = &__pcpu[0];

	proc_linkup0(&proc0, &thread0);
	//thread0.td_kstack = kstack;
	thread0.td_kstack_pages = kstack_pages;
#ifndef __WASM
	thread0.td_pcb = (struct pcb *)(thread0.td_kstack +
	    thread0.td_kstack_pages * PAGE_SIZE) - 1;
	thread0.td_pcb->pcb_fpflags = 0;
#endif
	thread0.td_frame = &proc0_tf;
	pcpup->pc_curpcb = thread0.td_pcb;
}

static struct mtx thread0_lock;

void initwasm(struct wasm_bootparams *wabp)
{
    struct pcpu *pcpup;
    char *env;

    __mainthread = __curthread;
    pageproc = &proc0;

	mtx_init(&thread0_lock, "thread0_mtx", NULL, MTX_DEF);
	thread0.td_lock = &thread0_lock;

    TSRAW(__curthread, TS_ENTER, __func__, NULL);

    log(0, "offset of td->td_sa %lu", offsetof(struct thread, td_sa));
    log(0, "offset of td->td_frame %lu", offsetof(struct thread, td_frame));

	/* Set the pcpu data, this is needed by pmap_bootstrap */
	pcpup = &__pcpu[0];
	pcpu_init(pcpup, 0, sizeof(struct pcpu));

    init_static_kenv(static_kenv, sizeof(static_kenv));

#if defined(FDT)
	/*
	 * Exclude reserved memory specified by the device tree. Typically,
	 * this contains an entry for memory used by the runtime SBI firmware.
	 */
	if (fdt_get_reserved_mem(mem_regions, &mem_regions_sz) == 0) {
		physmem_exclude_regions(mem_regions, mem_regions_sz,
		    EXFLAG_NODUMP | EXFLAG_NOALLOC);
	}

	/* Grab physical memory regions information from device tree. */
	if (fdt_get_mem_regions(mem_regions, &mem_regions_sz, NULL) != 0) {
		panic("Cannot get physical memory regions");
	}
	physmem_hardware_regions(mem_regions, mem_regions_sz);
#endif

    init_param1();

    //cache_setup();

    //kernlen = (lastaddr - KERNBASE);
    pmap_bootstrap(wabp->kern_l1pt, 1024, 12238848);

    physmem_exclude_region(1024, 12237824, EXFLAG_NOALLOC);
    physmem_hardware_region(12238848, 115556352); // 128057344 - (65536 * 4)

	physmem_init_kernel_globals();

	/* Establish static device mappings */
	devmap_bootstrap(0, NULL);

	cninit();

	/*
	 * Dump the boot metadata. We have to wait for cninit() since console
	 * output is required. If it's grossly incorrect the kernel will never
	 * make it this far.
	 */
	if (getenv_is_true("debug.dump_modinfo_at_boot"))
		preload_dump();

	init_proc0(0); /*rvbp->kern_stack*/

	msgbufinit(msgbufp, msgbufsize);
	mutex_init();
	init_param2(physmem);
	kdb_init();

#ifdef KDB
	if ((boothowto & RB_KDB) != 0)
		kdb_enter(KDB_WHY_BOOTFLAGS, "Boot flags requested debugger");
#endif

	env = kern_getenv("kernelname");
	if (env != NULL)
		strlcpy(kernelname, env, sizeof(kernelname));

	if (boothowto & RB_VERBOSE)
		physmem_print_tables();

	early_boot = 0;

	TSEXIT();
}

/**
 * this is to be called only once by the first kernel thread instance.
 */
void global_start(void)
{

    // setup page mapping...

    initwasm(&__static_wabp);
    mi_startup();
}

// we also need to somewhere have logics in place to allocate memory which is used
// per instance of the kernel, each userland process might execute kernel wasm binary
// which stack needs to also be keept in the same memory.