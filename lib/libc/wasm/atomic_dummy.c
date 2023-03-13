

#include <sys/cdefs.h>
#include <machine/atomic_wasm.h>
#include <machine/wasm_module.h>

// The role of this source file is not to provide a actual implementation but to act as placeholders for atomic
// operations ensuring that call sites for atomic methods are not inlined. The concept is that if the call instruction
// uses the same register set as the instruction used in wasm then the call instruction can be more or less replaced
// with the instruction for the same atomic operations.

void __panic_abort(void) __WASM_IMPORT(kern, panic_abort);

/**
 * @return Returns the number of woken up agents
 */
uint32_t __noinline atomic_notify(const uint32_t *valp, uint32_t count)
{
    __panic_abort();
    return 0;
}

void __noinline atomic_fence(void)
{
    __panic_abort(); 
}

/**
 * 
 * @see https://github.com/WebAssembly/threads/blob/main/proposals/threads/Overview.md#wait
 * @param timeout specifies a timeout in nanoseconds
 */
enum wait_result __noinline atomic_wait32(volatile uint32_t *valp, uint32_t expected, uint64_t timeout)
{
    __panic_abort();
    return ATOMIC_WAIT_NOT_EQUAL;
}

uint32_t __noinline atomic_load32(const uint32_t *valp)
{
    return (*valp);
}

void __noinline atomic_store32(volatile uint32_t *valp, uint32_t val)
{
    *valp = val;
}

uint32_t __noinline atomic_add32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = tmp + val;
    return tmp;
}

uint32_t __noinline atomic_sub32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = tmp - val;
    return tmp;
}

uint32_t __noinline atomic_and32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = tmp & val;
    return tmp;
}

uint32_t __noinline atomic_or32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = tmp | val;
    return tmp;
}

uint32_t __noinline atomic_xor32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = tmp ^ val;
    return tmp;
}

uint32_t __noinline atomic_xchg32(volatile uint32_t *valp, uint32_t val)
{
    uint32_t tmp = *valp;
    *valp = val;
    return tmp;
}

uint32_t __noinline atomic_cmpxchg32(volatile uint32_t *valp, uint32_t expected, uint32_t replacement)
{
    uint32_t tmp = *valp;
    if (expected == tmp) {
        *valp = replacement;
    }
    return tmp;
}


// 64-bit

enum wait_result __noinline atomic_wait64(volatile uint64_t *valp, uint64_t expected, uint64_t timeout)
{
    __panic_abort();
    return ATOMIC_WAIT_NOT_EQUAL;
}

uint64_t __noinline atomic_load64(const uint64_t *valp)
{
    return (*valp);
}

void __noinline atomic_store64(volatile uint64_t *valp, uint64_t val)
{
    *valp = val;
}

uint64_t __noinline atomic_add64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = tmp + val;
    return tmp;
}

uint64_t __noinline atomic_sub64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = tmp - val;
    return tmp;
}

uint64_t __noinline atomic_and64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = tmp & val;
    return tmp;
}

uint64_t __noinline atomic_or64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = tmp | val;
    return tmp;
}

uint64_t __noinline atomic_xor64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = tmp ^ val;
    return tmp;
}

uint64_t __noinline atomic_xchg64(volatile uint64_t *valp, uint64_t val)
{
    uint64_t tmp = *valp;
    *valp = val;
    return tmp;
}

uint64_t __noinline atomic_cmpxchg64(volatile uint64_t *valp, uint64_t expected, uint64_t replacement)
{
    uint64_t tmp = *valp;
    if (expected == tmp) {
        *valp = replacement;
    }
    return tmp;
}
