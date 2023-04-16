
// wasm-page 65536 bytes, page shift: 1 << 16
// page: 4096 bytes , page shift: 1 << 12       or alt. 8192 bytes 1 << 13

#include <stdint.h>
#include <stddef.h>

// https://github.com/arneg/GJAlloc
// https://github.com/webconn/sbmalloc
// https://github.com/Nopekick/malloc/blob/main/malloc.c
// https://github.com/unixpickle/anpages/blob/master/src/anpages.c

// what we need:
// manage and reserve 4096 or 65536 range of memory, alignement same as the size.
// allocation per pool
// general allocation
//
// the 65536 page-size are need to reserve stack for each tread, in 1 x 65536 or 2 x 65536 etc.

#define MAX_NO_PAGES 1954

#define MAXPAGESIZES 3
#define PAGE_SHIFT 12
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define PAGE_MASK (PAGE_SIZE - 1)

#define WASM_PAGE_SIZE 65536
#define WASM_PAGE_SHIFT 16

struct mempage {
    struct mempage *prev;
    struct mempage *next;
    uint8_t pgsz;
};

static uint32_t page_bitmap[MAX_NO_PAGES * 16];        // this one needs to be changed when changing the smallest page size.
static uint32_t pagesizes[MAXPAGESIZES] = {4096, 65536};
static struct mempage **super_pages; // maps a pointer to each struct page at super level (each chunk of memory of 65536 bytes size)
// ^^ 7816 bytes

struct kmemstats {
    uint32_t *pgbitmap; // 
    uint32_t lowfreepg; // holds the lowest free page index.
    uint32_t pgsizes[MAXPAGESIZES] = {4096, 65536};
};

static void kmem_init(void)
{
    uint32_t memlow;
    uint32_t memhigh;
}


#define atop(x)     ((unsigned long)(x) >> PAGE_SHIFT)
#define ptoa(x)     ((unsigned long)(x) << PAGE_SHIFT)

struct mempage *get_page(void *addr)
{
    uint32_t i = atop(addr);
    uint32_t b = i / 32;
    uint32_t r = i % 32;
    uint32_t used = (page_bitmap[b] & (1 << r)) != 0;
    struct mempage *pg = NULL;
    if (!used)
        return pg;
    i = (unsigned long)(addr) >> WASM_PAGE_SHIFT;
    pg = super_pages[i];

    return pg;
}

struct mempage *next_free(unsigned int pgsize)
{
    uint32_t i = atop(addr);
    uint32_t b = i / 32;
    uint32_t r = i % 32;
    bool used = (page_bitmap[b] & (1 << r)) != 0;
    uint32_t i = a > WASM_PAGE_SIZE ? a / WASM_PAGE_SIZE : 0;
    struct mempage *pg = NULL;

    return pg;
}

void *mm_malloc()
{

}

// page bitmap ops could have its own mutex and dont need to be shared with malloc.

// addr-to-page
// alloc & free page
// places where a page are optimized to be used rather than a malloc
// - thread stack memory (allocated at high memory)
// - fixed size buffers such as those in Pipe

// malloc rules
// check mutex (spin wait for a certain period)
// 

// free rules
// check mutex (spin wait for a certain period)

/* freebsd initial data.

```
#1 .rodata
start: 1024
  end: 55408 (size 54384 bytes)

#2 .data
start: 55408
  end: 103480 (size: 48072 bytes)

#3 set_pcpu
start: 103488
  end: 104000 (size: 512 bytes)

#4 .bss
start: 104000
  end: 10613608 (size: 10509608 bytes)

```

page-align the ending: 10616832 

*/