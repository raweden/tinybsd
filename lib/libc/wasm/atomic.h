/*-
 * SPDX-License-Identifier: BSD-4-Clause
 *
 * Copyright (C) 2023 Jesper Svensson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *  This product includes software developed by Brini.
 * 4. The name of Brini may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BRINI ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL BRINI BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#ifndef _LIBC_WASM_ATOMIC_H_
#define _LIBC_WASM_ATOMIC_H_

#include <sys/stdint.h>

#ifndef _MACHINE_ATOMIC_WASM_H_

// TODO: wasm has both signed and unsigned operations.

/**
 * @return Returns the number of woken up agents
 */
uint32_t atomic_notify(const uint32_t *valp, uint32_t count);

void atomic_fence(void);

enum wait_result {
    ATOMIC_WAIT_OK = 0,
    ATOMIC_WAIT_NOT_EQUAL = 1,
    ATOMIC_WAIT_TIMEOUT = 2
};

/**
 * 
 * @see https://github.com/WebAssembly/threads/blob/main/proposals/threads/Overview.md#wait
 * @param timeout specifies a timeout in nanoseconds
 */
enum wait_result atomic_wait32(volatile uint32_t *valp, uint32_t expected, uint64_t timeout);

uint32_t atomic_load32(const uint32_t *valp);
void atomic_store32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_add32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_sub32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_and32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_or32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_xor32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_xchg32(volatile uint32_t *valp, uint32_t val);
uint32_t atomic_cmpxchg32(volatile uint32_t *valp, uint32_t expected, uint32_t replacement);

// 64-bit

enum wait_result atomic_wait64(volatile uint64_t *valp, uint64_t expected, uint64_t timeout);

uint64_t atomic_load64(const uint64_t *valp);
void atomic_store64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_add64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_sub64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_and64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_or64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_xor64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_xchg64(volatile uint64_t *valp, uint64_t val);
uint64_t atomic_cmpxchg64(volatile uint64_t *valp, uint64_t expected, uint64_t replacement);

#endif

// https://www.daemon-systems.org/man/atomic_add_32.3.html
// http://fxr.watson.org/fxr/source/arm/include/atomic.h?v=FREEBSD-6-0
// sys/kern/cddl/compat/opensolaris/kern/opensolaris_atomic.c
// start placeholders

#define atomic_thread_fence_rel()

#if 0
#define NO_ATOMICS
#endif

__attribute__((always_inline))
static inline int atomic_cmpset_32(volatile uint32_t *p, uint32_t cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg32(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

// https://nxmnpg.lemoda.net/9/atomic_fcmpset
__attribute__((always_inline))
static inline int atomic_fcmpset_32(volatile uint32_t *p, uint32_t *cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == *cmpval) {
        *p = newval;
        return 1;
    } else {
        *cmpval = *p;
        return 0;
    }
#else
    uint32_t tmp = atomic_cmpxchg32(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_rel_32(volatile uint32_t *p, uint32_t cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg32(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_acq_32(volatile uint32_t *p, uint32_t cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg32(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_rel_32(volatile uint32_t *p, uint32_t *cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == *cmpval) {
        *p = newval;
        return 1;
    } else {
        *cmpval = *p;
        return 0;
    }
#else
    uint32_t tmp = atomic_cmpxchg32(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_acq_32(volatile uint32_t *p, uint32_t *cmpval, uint32_t newval)
{
#ifdef NO_ATOMICS
    if (*p == *cmpval) {
        *p = newval;
        return 1;
    } else {
        *cmpval = *p;
        return 0;
    }
#else
    uint32_t tmp = atomic_cmpxchg32(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline void atomic_add_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add32(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_rel_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add32(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_acq_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add32(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub32(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_rel_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub32(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_set_32(volatile uint32_t *p, uint32_t setmask)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp |= setmask;
    *p = tmp;
#else
    atomic_or32(p, setmask);
#endif
}

__attribute__((always_inline))
static inline void atomic_clear_32(volatile uint32_t *p, uint32_t clearmask)
{
#ifdef NO_ATOMICS
    uint32_t tmp = *p;
    tmp &= ~clearmask;
    *p = tmp;
#else
    uint32_t tmp = ~clearmask;
    atomic_and32(p, tmp);
#endif
}

/**
 * The atomic_fetchadd_32() adds the value operand to atomic variable object and returns the original contents of the atomic variable.
 */
 __attribute__((always_inline))
static inline uint32_t atomic_fetchadd_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t tmp, ret = 0;
    int res;
    return (ret);
#else
    return atomic_add32(p, val);
#endif
}

__attribute__((always_inline))
static inline uint32_t atomic_readandclear_32(volatile uint32_t *p)
{
#ifdef NO_ATOMICS
    uint32_t ret;
    ret = *p;
    *p = 0;
    return (ret);
#else
    return atomic_xchg32(p, 0);
#endif
}

static inline uint32_t atomic_swap_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    uint32_t ret;
    ret = *p;
    *p = val;
    return (ret);
#else
    return atomic_xchg32(p, val);
#endif
}

__attribute__((always_inline))
static inline uint32_t atomic_load_acq_32(volatile uint32_t *p)
{
#ifdef NO_ATOMICS
    uint32_t ret;
    ret = *p;
    return (ret);
#else
    return atomic_load32(p);
#endif
}

__attribute__((always_inline))
static inline void atomic_store_rel_32(volatile uint32_t *p, uint32_t val)
{
#ifdef NO_ATOMICS
    *p = val;
#else
    return atomic_store32(p, val);
#endif
}

// uint64 handlers

__attribute__((always_inline))
static inline void atomic_add_64(volatile uint64_t *p, uint64_t val)
{
#ifdef NO_ATOMICS
    uint64_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add64(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_set_64(volatile uint64_t *p, uint64_t setmask)
{
#ifdef NO_ATOMICS
    uint64_t tmp = *p;
    tmp |= setmask;
    *p = tmp;
#else
    atomic_or64(p, setmask);
#endif
}

__attribute__((always_inline))
static inline void atomic_clear_64(volatile uint64_t *p, uint64_t clearmask)
{
#ifdef NO_ATOMICS
    uint64_t tmp = *p;
    tmp &= ~clearmask;
    *p = tmp;
#else
    uint64_t tmp = ~clearmask;
    atomic_and64(p, tmp);
#endif
}

__attribute__((always_inline))
static inline uint64_t  atomic_fetchadd_64(volatile uint64_t *p, uint64_t val)
{
#ifdef NO_ATOMICS
    uint64_t tmp, ret = 0;
    int res;
    return (ret);
#else
    return atomic_add64(p, val);
#endif
}

__attribute__((always_inline))
static inline uint64_t atomic_readandclear_64(volatile uint64_t *p)
{
#ifdef NO_ATOMICS
    uint64_t ret;
    ret = *p;
    *p = 0;
    return (ret);
#else
    return atomic_xchg64(p, 0);
#endif
}

__attribute__((always_inline))
static inline uint64_t atomic_swap_64(volatile uint64_t *p, uint64_t val)
{
#ifdef NO_ATOMICS
    uint64_t ret;
    ret = *p;
    *p = val;
    return (ret);
#else
    return atomic_xchg64(p, val);
#endif
}

__attribute__((always_inline))
static inline uint64_t atomic_load_acq_64(volatile uint64_t *p)
{
#ifdef NO_ATOMICS
    uint64_t ret;
    ret = *p;
    return (ret);
#else
    return atomic_load64(p);
#endif
}

__attribute__((always_inline))
static inline uint64_t atomic_load_64(volatile uint32_t *p)
{
#ifdef NO_ATOMICS
    uint32_t ret;
    ret = *p;
    return (ret);
#else
    return atomic_load64(p);
#endif
}

__attribute__((always_inline))
static inline void atomic_store_rel_64(volatile uint64_t *p, uint64_t val)
{
#ifdef NO_ATOMICS
    *p = val;
#else
    atomic_store64(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_store_64(volatile uint64_t *p, uint64_t val)
{
#ifdef NO_ATOMICS
    *p = val;
#else
    atomic_store64(p, val);
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_64(volatile uint64_t *p, uint64_t cmpval, uint64_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg64(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_64(volatile uint64_t *p, uint64_t *cmpval, uint64_t newval)
{
#ifdef NO_ATOMICS
    if (*p == *cmpval) {
        *p = newval;
        return 1;
    } else {
        *cmpval = *p;
        return 0;
    }
#else
    uint64_t tmp = atomic_cmpxchg64(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

// https://nxmnpg.lemoda.net/9/atomic_fcmpset
__attribute__((always_inline))
static inline int atomic_fcmpset_8(volatile uint8_t *p, uint8_t *cmpval, uint8_t newval)
{
#ifdef NO_ATOMICS
    if (*p == *cmpval) {
        *p = newval;
        return 1;
    } else {
        *cmpval = *p;
        return 0;
    }
#else
    uint32_t tmp = atomic_cmpxchg32(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

// end placeholders 

__attribute__((always_inline))
static inline u_long atomic_swap_long(volatile u_long *p, u_long v)
{
    return (atomic_swap_32((volatile uint32_t *)p, v));
}

#define atomic_clear_ptr        atomic_clear_32
#define atomic_clear_acq_ptr    atomic_clear_acq_32
#define atomic_clear_rel_ptr    atomic_clear_rel_32
#define atomic_set_ptr          atomic_set_32
#define atomic_set_acq_ptr      atomic_set_acq_32
#define atomic_set_rel_ptr      atomic_set_rel_32
#define atomic_fcmpset_ptr      atomic_fcmpset_32
#define atomic_fcmpset_rel_ptr  atomic_fcmpset_rel_32
#define atomic_fcmpset_acq_ptr  atomic_fcmpset_acq_32
#define atomic_cmpset_ptr       atomic_cmpset_32
#define atomic_cmpset_acq_ptr   atomic_cmpset_acq_32
#define atomic_cmpset_rel_ptr   atomic_cmpset_rel_32
#define atomic_load_acq_ptr     atomic_load_acq_32
#define atomic_store_rel_ptr    atomic_store_rel_32
#define atomic_swap_ptr         atomic_swap_32
#define atomic_readandclear_ptr atomic_readandclear_32

#define atomic_add_int          atomic_add_32
#define atomic_add_acq_int      atomic_add_acq_32
#define atomic_add_rel_int      atomic_add_rel_32
#define atomic_subtract_int     atomic_subtract_32
#define atomic_subtract_acq_int atomic_subtract_acq_32
#define atomic_subtract_rel_int atomic_subtract_rel_32
#define atomic_clear_int        atomic_clear_32
#define atomic_clear_acq_int    atomic_clear_acq_32
#define atomic_clear_rel_int    atomic_clear_rel_32
#define atomic_set_int          atomic_set_32
#define atomic_set_acq_int      atomic_set_acq_32
#define atomic_set_rel_int      atomic_set_rel_32
#define atomic_fcmpset_int      atomic_fcmpset_32
#define atomic_fcmpset_acq_int  atomic_fcmpset_acq_32
#define atomic_fcmpset_rel_int  atomic_fcmpset_rel_32
#define atomic_cmpset_int       atomic_cmpset_32
#define atomic_cmpset_acq_int   atomic_cmpset_acq_32
#define atomic_cmpset_rel_int   atomic_cmpset_rel_32
#define atomic_fetchadd_int     atomic_fetchadd_32
#define atomic_readandclear_int atomic_readandclear_32
#define atomic_load_acq_int     atomic_load_acq_32
#define atomic_store_rel_int    atomic_store_rel_32
#define atomic_swap_int         atomic_swap_32

#if defined(__WASM) && __WASM == 32
#define atomic_add_long          atomic_add_32
#define atomic_add_acq_long      atomic_add_acq_32
#define atomic_add_rel_long      atomic_add_rel_32
#define atomic_subtract_long     atomic_subtract_32
#define atomic_subtract_acq_long atomic_subtract_acq_32
#define atomic_subtract_rel_long atomic_subtract_rel_32
#define atomic_clear_long        atomic_clear_32
#define atomic_clear_acq_long    atomic_clear_acq_32
#define atomic_clear_rel_long    atomic_clear_rel_32
#define atomic_set_long          atomic_set_32
#define atomic_set_acq_long      atomic_set_acq_32
#define atomic_set_rel_long      atomic_set_rel_32
#define atomic_fcmpset_long      atomic_fcmpset_32
#define atomic_fcmpset_acq_long  atomic_fcmpset_acq_32
#define atomic_fcmpset_rel_long  atomic_fcmpset_rel_32
#define atomic_cmpset_long       atomic_cmpset_32
#define atomic_cmpset_acq_long   atomic_cmpset_acq_32
#define atomic_cmpset_rel_long   atomic_cmpset_rel_32
#define atomic_fetchadd_long     atomic_fetchadd_32
#define atomic_readandclear_long atomic_readandclear_32
#define atomic_load_acq_long     atomic_load_acq_32
#define atomic_store_rel_long    atomic_store_rel_32
#define atomic_swap_long         atomic_swap_32
#elif defined(__WASM) && __WASM == 64
#define atomic_add_long          atomic_add_64
#define atomic_add_acq_long      atomic_add_acq_64
#define atomic_add_rel_long      atomic_add_rel_64
#define atomic_subtract_long     atomic_subtract_64
#define atomic_subtract_acq_long atomic_subtract_acq_64
#define atomic_subtract_rel_long atomic_subtract_rel_64
#define atomic_clear_long        atomic_clear_64
#define atomic_clear_acq_long    atomic_clear_acq_64
#define atomic_clear_rel_long    atomic_clear_rel_64
#define atomic_set_long          atomic_set_64
#define atomic_set_acq_long      atomic_set_acq_64
#define atomic_set_rel_long      atomic_set_rel_64
#define atomic_fcmpset_long      atomic_fcmpset_64
#define atomic_fcmpset_acq_long  atomic_fcmpset_acq_64
#define atomic_fcmpset_rel_long  atomic_fcmpset_rel_64
#define atomic_cmpset_long       atomic_cmpset_64
#define atomic_cmpset_acq_long   atomic_cmpset_acq_64
#define atomic_cmpset_rel_long   atomic_cmpset_rel_64
#define atomic_fetchadd_long     atomic_fetchadd_64
#define atomic_readandclear_long atomic_readandclear_64
#define atomic_load_acq_long     atomic_load_acq_64
#define atomic_store_rel_long    atomic_store_rel_64
#define atomic_swap_long         atomic_swap_64
#endif

#endif /* _MACHINE_ATOMIC_H_ */
