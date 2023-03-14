
// pmap maps the relationship between virtual and psycial memory addresses.
// as wasm provides no means for doing virtual mapping ontop of the ArrayBuffer
// interface to the memory of a WebAssembly module, for wasm we need to remove
// this layer of abstraction completly. The layer which implements wasm provides
// this low level layer in-itself and WebAssembly is without the knowledge of how
// its mapped in the layer above it.
// 
// every method with a vm_* prefix will aslo needs to be altered, keeping alloc/alloc-map etc.
// 
// how the kernel access memory is by the means of separte containers each providing 
// its own local range (0 to ArrayBuffer.byteLength) each address therefor needs to 
// reference not just the local address but also the index which the kernel knows that
// memory container by 'memidx'
// 
// mmap system calls will which maps file content to memory pages which can be accessed from
// processes in userland cannot be shared across more than one such memory container, in the 
// case of more than one container changes in one container on a range which is also mounted
// in another container will be hard to syncronize. The features need in order for this to work
// is either to be able to share a SharedArrayBuffer within a chunk of another or to be able to
// override certain ranges within SharedArrayBuffer (like a proxy/reflection API that acts on a 
// specified range within a ArrayBuffer)

#include <sys/cdefs.h>
#include <sys/_null.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <vm/vm.h>
#include <vm/vm_object.h>
#include <vm/pmap.h>
//#include <machine/pmap-v6.h>
#include <machine/wasm_module.h>

void __panic_abort(void) __WASM_IMPORT(kern, panic_abort);

//struct pmap kernel_pmap_store;
//vm_paddr_t dmap_phys_base;
//vm_offset_t kernel_vm_end;
int unmapped_buf_allowed;

void pmap_qenter(vm_offset_t a, vm_page_t *b, int c)
{
	__panic_abort();
}

void pmap_zero_page(vm_page_t a)
{
	__panic_abort();
}

void pmap_qremove(vm_offset_t a, int b)
{
	__panic_abort();
}

vm_paddr_t pmap_kextract(vm_offset_t a)
{
	__panic_abort();
	return 0;
}

void pmap_zero_page_area(vm_page_t a, int off, int size)
{
	__panic_abort();
}

int  pmap_enter(pmap_t pmap, vm_offset_t va, vm_page_t m, vm_prot_t prot, u_int flags, int8_t psind)
{
	__panic_abort();
	return 0;
}

void pmap_remove_all(vm_page_t m)
{
	__panic_abort();
}

void pmap_page_set_memattr(vm_page_t a, vm_memattr_t b)
{
	__panic_abort();
}

void pmap_remove(pmap_t a, vm_offset_t b, vm_offset_t c)
{
	__panic_abort();
}

void pmap_activate(struct thread *td)
{
	__panic_abort();
}

vm_offset_t	pmap_map(vm_offset_t *a, vm_paddr_t b, vm_paddr_t c, int d)
{
	//__panic_abort();
	return 0;
}

void pmap_remove_write(vm_page_t m)
{
	__panic_abort();
}

void pmap_copy_page(vm_page_t a, vm_page_t b)
{
	__panic_abort();
}

void pmap_protect(pmap_t a, vm_offset_t b, vm_offset_t c, vm_prot_t d)
{
	__panic_abort();
}

void pmap_remove_pages(pmap_t a)
{
	__panic_abort();
}

int  pmap_mincore(pmap_t pmap, vm_offset_t addr, vm_paddr_t *pap)
{
	__panic_abort();
	return 0;
}

vm_paddr_t pmap_extract(pmap_t pmap, vm_offset_t va)
{
	__panic_abort();
	return 0;
}

void pmap_copy_pages(vm_page_t ma[], vm_offset_t a_offset, vm_page_t mb[], vm_offset_t b_offset, int xfersize)
{
	__panic_abort();
}

bool pmap_ps_enabled(pmap_t pmap)
{
	__panic_abort();
	return 0;
}

void pmap_advise(pmap_t pmap, vm_offset_t sva, vm_offset_t eva, int advice)
{
	__panic_abort();
}

void pmap_enter_object(pmap_t pmap, vm_offset_t start, vm_offset_t end, vm_page_t m_start, vm_prot_t prot)
{
	__panic_abort();
}

void pmap_unwire(pmap_t pmap, vm_offset_t start, vm_offset_t end)
{
	__panic_abort();
}

int  pmap_pinit(pmap_t a)
{
	__panic_abort();
	return 0;
}

void pmap_copy(pmap_t a, pmap_t b, vm_offset_t c, vm_size_t d, vm_offset_t e)
{
	__panic_abort();
}

boolean_t pmap_is_modified(vm_page_t m)
{
	__panic_abort();
	return 0;
}

void pmap_kenter_device(vm_offset_t a, vm_size_t b, vm_paddr_t c)
{
	__panic_abort();
}

void pmap_kremove_device(vm_offset_t a, vm_size_t b)
{
	__panic_abort();
}

int  pmap_page_wired_mappings(vm_page_t m)
{
	__panic_abort();
	return 0;
}

boolean_t pmap_is_prefaultable(pmap_t pmap, vm_offset_t va)
{
	__panic_abort();
	return 0;
}

void pmap_enter_quick(pmap_t pmap, vm_offset_t va, vm_page_t m, vm_prot_t prot)
{
	__panic_abort();
}

vm_page_t pmap_extract_and_hold(pmap_t pmap, vm_offset_t va, vm_prot_t prot)
{
	__panic_abort();
	return NULL;
}

void pmap_sync_icache(pmap_t a, vm_offset_t b, vm_size_t c)
{
	__panic_abort();
}

void pmap_release(pmap_t a)
{
	__panic_abort();
}

void pmap_growkernel(vm_offset_t b)
{
	__panic_abort();
}

void pmap_object_init_pt(pmap_t pmap, vm_offset_t addr, vm_object_t object, vm_pindex_t pindex, vm_size_t size)
{
	__panic_abort();
}

void pmap_align_superpage(vm_object_t a, vm_ooffset_t b, vm_offset_t *c, vm_size_t d)
{
	__panic_abort();
}

bool pmap_page_is_mapped(vm_page_t a)
{
	__panic_abort();
	return 0;
}

void pmap_clear_modify(vm_page_t m)
{
	__panic_abort();
}

boolean_t pmap_is_referenced(vm_page_t m)
{
	return true;
}

//vm_offset_t virtual_avail = 0;
//vm_offset_t virtual_end = 0xFFFFFF00;

void domainset_init(void)
{

}

void domainset_zero(void)
{

}