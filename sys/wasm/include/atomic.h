/* $NetBSD: atomic.h,v 1.1 2002/10/19 12:22:34 bsh Exp $ */

/*-
 * SPDX-License-Identifier: BSD-4-Clause
 *
 * Copyright (C) 2003-2004 Olivier Houchard
 * Copyright (C) 1994-1997 Mark Brinicombe
 * Copyright (C) 1994 Brini
 * All rights reserved.
 *
 * This code is derived from software written for Brini by Mark Brinicombe
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

#ifndef _MACHINE_ATOMIC_H_
#define _MACHINE_ATOMIC_H_

#include <sys/atomic_common.h>
#include <machine/atomic_wasm.h>
#include "wasm/include/atomic_wasm.h"

#ifndef _KERNEL
#include <machine/sysarch.h>
#endif

// https://www.daemon-systems.org/man/atomic_add_32.3.html
// http://fxr.watson.org/fxr/source/arm/include/atomic.h?v=FREEBSD-6-0
// sys/kern/cddl/compat/opensolaris/kern/opensolaris_atomic.c
// start placeholders

void wasm32_atomic_fence(void);

// 32-bit

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

// what does _rel_ do diffrently?
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
// what does _acq_ do diffrently?
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

// what does _rel_ do diffrently?
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
// what does _acq_ do diffrently?
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

// 64-bit uint64 handlers

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
static inline uint64_t atomic_fetchadd_64(volatile uint64_t *p, uint64_t val)
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

// 8-bit handlers

__attribute__((always_inline))
static inline int atomic_cmpset_8(volatile uint8_t *p, uint8_t cmpval, uint8_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg8(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

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
    uint8_t tmp = atomic_cmpxchg8(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_rel_8(volatile uint8_t *p, uint8_t cmpval, uint8_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg8(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_acq_8(volatile uint8_t *p, uint8_t cmpval, uint8_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg8(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_rel_8(volatile uint8_t *p, uint8_t *cmpval, uint8_t newval)
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
    uint8_t tmp = atomic_cmpxchg8(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_acq_8(volatile uint8_t *p, uint8_t *cmpval, uint8_t newval)
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
    uint8_t tmp = atomic_cmpxchg8(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline void atomic_add_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add8(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_rel_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add8(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_acq_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add8(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub8(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_rel_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub8(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_set_8(volatile uint8_t *p, uint8_t setmask)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp |= setmask;
    *p = tmp;
#else
    atomic_or8(p, setmask);
#endif
}

__attribute__((always_inline))
static inline void atomic_clear_8(volatile uint8_t *p, uint8_t clearmask)
{
#ifdef NO_ATOMICS
    uint8_t tmp = *p;
    tmp &= ~clearmask;
    *p = tmp;
#else
    uint8_t tmp = ~clearmask;
    atomic_and8(p, tmp);
#endif
}

 __attribute__((always_inline))
static inline uint8_t atomic_fetchadd_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t tmp, ret = 0;
    int res;
    return (ret);
#else
    return atomic_add8(p, val);
#endif
}

__attribute__((always_inline))
static inline uint8_t atomic_readandclear_8(volatile uint8_t *p)
{
#ifdef NO_ATOMICS
    uint8_t ret;
    ret = *p;
    *p = 0;
    return (ret);
#else
    return atomic_xchg8(p, 0);
#endif
}

static inline uint8_t atomic_swap_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    uint8_t ret;
    ret = *p;
    *p = val;
    return (ret);
#else
    return atomic_xchg8(p, val);
#endif
}

__attribute__((always_inline))
static inline uint8_t atomic_load_acq_8(volatile uint8_t *p)
{
#ifdef NO_ATOMICS
    uint8_t ret;
    ret = *p;
    return (ret);
#else
    return atomic_load8(p);
#endif
}

__attribute__((always_inline))
static inline void atomic_store_rel_8(volatile uint8_t *p, uint8_t val)
{
#ifdef NO_ATOMICS
    *p = val;
#else
    atomic_store8(p, val);
#endif
}


// 16-bit handlers

__attribute__((always_inline))
static inline int atomic_cmpset_16(volatile uint16_t *p, uint16_t cmpval, uint16_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg16(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_16(volatile uint16_t *p, uint16_t *cmpval, uint16_t newval)
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
    uint16_t tmp = atomic_cmpxchg16(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_rel_16(volatile uint16_t *p, uint16_t cmpval, uint16_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg16(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_cmpset_acq_16(volatile uint16_t *p, uint16_t cmpval, uint16_t newval)
{
#ifdef NO_ATOMICS
    if (*p == cmpval) {
        *p = newval;
        return 1;
    } else {
        return 0;
    }
#else
    return atomic_cmpxchg16(p, cmpval, newval) == cmpval ? 1 : 0;
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_rel_16(volatile uint16_t *p, uint16_t *cmpval, uint16_t newval)
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
    uint16_t tmp = atomic_cmpxchg16(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline int atomic_fcmpset_acq_16(volatile uint16_t *p, uint16_t *cmpval, uint16_t newval)
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
    uint16_t tmp = atomic_cmpxchg16(p, *cmpval, newval);
    if (tmp == *cmpval) {
        return 1;
    } else {
        *cmpval = tmp;
        return 0;
    }
#endif
}

__attribute__((always_inline))
static inline void atomic_add_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add16(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_rel_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add16(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_add_acq_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp += val;
    *p = tmp;
#else
    atomic_add16(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub16(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_subtract_rel_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp -= val;
    *p = tmp;
#else
    atomic_sub16(p, val);
#endif
}

__attribute__((always_inline))
static inline void atomic_set_16(volatile uint16_t *p, uint16_t setmask)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp |= setmask;
    *p = tmp;
#else
    atomic_or16(p, setmask);
#endif
}

__attribute__((always_inline))
static inline void atomic_clear_16(volatile uint16_t *p, uint16_t clearmask)
{
#ifdef NO_ATOMICS
    uint16_t tmp = *p;
    tmp &= ~clearmask;
    *p = tmp;
#else
    uint16_t tmp = ~clearmask;
    atomic_and16(p, tmp);
#endif
}

 __attribute__((always_inline))
static inline uint16_t atomic_fetchadd_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t tmp, ret = 0;
    int res;
    return (ret);
#else
    return atomic_add16(p, val);
#endif
}

__attribute__((always_inline))
static inline uint16_t atomic_readandclear_16(volatile uint16_t *p)
{
#ifdef NO_ATOMICS
    uint16_t ret;
    ret = *p;
    *p = 0;
    return (ret);
#else
    return atomic_xchg16(p, 0);
#endif
}

static inline uint16_t atomic_swap_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    uint16_t ret;
    ret = *p;
    *p = val;
    return (ret);
#else
    return atomic_xchg16(p, val);
#endif
}

__attribute__((always_inline))
static inline uint16_t atomic_load_acq_16(volatile uint16_t *p)
{
#ifdef NO_ATOMICS
    uint16_t ret;
    ret = *p;
    return (ret);
#else
    return atomic_load16(p);
#endif
}

__attribute__((always_inline))
static inline void atomic_store_rel_16(volatile uint16_t *p, uint16_t val)
{
#ifdef NO_ATOMICS
    *p = val;
#else
    atomic_store16(p, val);
#endif
}

// end placeholders 

//#include <machine/atomic-v6.h>

__attribute__((always_inline))
static inline u_long atomic_swap_long(volatile u_long *p, u_long v)
{
    return (atomic_swap_32((volatile uint32_t *)p, v));
}

#define atomic_thread_fence_rel wasm32_atomic_fence
#define atomic_thread_fence_acq wasm32_atomic_fence
#define atomic_thread_fence_seq_cst wasm32_atomic_fence
#define atomic_thread_fence_acq_rel wasm32_atomic_fence

#define atomic_interrupt_fence  wasm32_atomic_fence

#define rmb wasm32_atomic_fence

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
#define atomic_store_ptr        atomic_store_rel_32
#define atomic_swap_ptr         atomic_swap_32
#define atomic_readandclear_ptr atomic_readandclear_32
#define atomic_load_ptr         atomic_load_acq_32
#define atomic_load_consume_ptr atomic_load_acq_32

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
#define atomic_store_int        atomic_store_rel_32
#define atomic_swap_int         atomic_swap_32
#define atomic_load_int         atomic_load_acq_32
#define atomic_load_32          atomic_load_acq_32

#define atomic_add_char          atomic_add_8
#define atomic_add_acq_char      atomic_add_acq_8
#define atomic_add_rel_char      atomic_add_rel_8
#define atomic_subtract_char     atomic_subtract_8
#define atomic_subtract_acq_char atomic_subtract_acq_8
#define atomic_subtract_rel_char atomic_subtract_rel_8
#define atomic_clear_char        atomic_clear_8
#define atomic_clear_acq_char    atomic_clear_acq_8
#define atomic_clear_rel_char    atomic_clear_rel_8
#define atomic_set_char          atomic_set_8
#define atomic_set_acq_char      atomic_set_acq_8
#define atomic_set_rel_char      atomic_set_rel_8
#define atomic_fcmpset_char      atomic_fcmpset_8
#define atomic_fcmpset_acq_char  atomic_fcmpset_acq_8
#define atomic_fcmpset_rel_char  atomic_fcmpset_rel_8
#define atomic_cmpset_char       atomic_cmpset_8
#define atomic_cmpset_acq_char   atomic_cmpset_acq_8
#define atomic_cmpset_rel_char   atomic_cmpset_rel_8
#define atomic_fetchadd_char     atomic_fetchadd_8
#define atomic_readandclear_char atomic_readandclear_8
#define atomic_load_acq_char     atomic_load_acq_8
#define atomic_load_char         atomic_load_acq_8
#define atomic_store_rel_char    atomic_store_rel_8
#define atomic_store_char        atomic_store_rel_8
#define atomic_swap_char         atomic_swap_8

#define atomic_add_short          atomic_add_16
#define atomic_add_acq_short      atomic_add_acq_16
#define atomic_add_rel_short      atomic_add_rel_16
#define atomic_subtract_short     atomic_subtract_16
#define atomic_subtract_acq_short atomic_subtract_acq_16
#define atomic_subtract_rel_short atomic_subtract_rel_16
#define atomic_clear_short        atomic_clear_16
#define atomic_clear_acq_short    atomic_clear_acq_16
#define atomic_clear_rel_short    atomic_clear_rel_16
#define atomic_set_short          atomic_set_16
#define atomic_set_acq_short      atomic_set_acq_16
#define atomic_set_rel_short      atomic_set_rel_16
#define atomic_fcmpset_short      atomic_fcmpset_16
#define atomic_fcmpset_acq_short  atomic_fcmpset_acq_16
#define atomic_fcmpset_rel_short  atomic_fcmpset_rel_16
#define atomic_cmpset_short       atomic_cmpset_16
#define atomic_cmpset_acq_short   atomic_cmpset_acq_16
#define atomic_cmpset_rel_short   atomic_cmpset_rel_16
#define atomic_fetchadd_short     atomic_fetchadd_16
#define atomic_readandclear_short atomic_readandclear_16
#define atomic_load_acq_short     atomic_load_acq_16
#define atomic_store_rel_short    atomic_store_rel_16
#define atomic_store_short        atomic_store_rel_16
#define atomic_swap_short         atomic_swap_16
#define atomic_load_short         atomic_load_acq_16
#define atomic_load_16            atomic_load_acq_16

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
#define atomic_store_long        atomic_store_rel_32
#define atomic_swap_long         atomic_swap_32
#define atomic_load_long         atomic_load_acq_32
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

//#include <sys/_atomic_subword.h>

#endif /* _MACHINE_ATOMIC_H_ */
