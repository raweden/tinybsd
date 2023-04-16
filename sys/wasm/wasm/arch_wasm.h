
#include <sys/stdint.h>

// this provides a layer which allows clang to optimize for the stack signature that
// is present for the memory.fill and memory.copy instructions within wasm. 

void arch_memset(void *dst, char fill, size_t len);
void arch_memcpy(void *dst, const void *src, size_t len);

static inline void *memset(void *dst, char fill, size_t len)
{
    arch_memset(dst, fill, len);
    return dst;
}

static inline void *memcpy(void *dst, const void *src, size_t len)
{
    arch_memcpy(dst, src, len);
    return dst;
}