

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/bus.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/lock.h>
#include <sys/memdesc.h>
#include <sys/mutex.h>
#include <sys/uio.h>
#include <vm/vm.h>
#include <vm/vm_extern.h>
#include <vm/pmap.h>

#include <machine/bus.h>

int	_bus_dmamap_load_buffer(bus_dma_tag_t dmat, bus_dmamap_t map, void *buf, bus_size_t buflen, struct pmap *pmap, int flags, bus_dma_segment_t *segs, int *segp)
{
	return 0;
}

int	_bus_dmamap_load_ma(bus_dma_tag_t dmat, bus_dmamap_t map, struct vm_page **ma, bus_size_t tlen, int ma_offs, int flags, bus_dma_segment_t *segs, int *segp)
{
	return 0;
}

void _bus_dmamap_waitok(bus_dma_tag_t dmat, bus_dmamap_t map, struct memdesc *mem, bus_dmamap_callback_t *callback, void *callback_arg)
{
	return;
}

int	_bus_dmamap_load_phys(bus_dma_tag_t dmat, bus_dmamap_t map, vm_paddr_t paddr, bus_size_t buflen, int flags, bus_dma_segment_t *segs, int *segp)
{
	return 0;
}

bus_dma_segment_t *_bus_dmamap_complete(bus_dma_tag_t dmat, bus_dmamap_t map, bus_dma_segment_t *segs, int nsegs, int error)
{
	return NULL;
}

int bus_dma_tag_create(bus_dma_tag_t parent, bus_size_t alignment, bus_addr_t boundary, bus_addr_t lowaddr, bus_addr_t highaddr, bus_dma_filter_t *filtfunc, void *filtfuncarg, bus_size_t maxsize, int nsegments, bus_size_t maxsegsz, int flags, bus_dma_lock_t *lockfunc, void *lockfuncarg, bus_dma_tag_t *dmat)
{
	return 0;
} 