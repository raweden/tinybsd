
#include <sys/types.h>
#include <sys/systm.h>


void *memmove(void * _Nonnull dest, const void * _Nonnull src, size_t n) {
    if (dest < src) 
        return memcpy(dest, src, n);
    unsigned char *d = (unsigned char *)dest + n;
    const unsigned char *s = (const unsigned char *)src + n;
#pragma clang loop unroll(disable)
    while(n--)
        *--d = *--s;
    return dest;
}