/*
 * Copyright (c) 2023, Jesper Svensson <me@raweden.se>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * WebAssembly get random numbers from host enviroment.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include "namespace.h"
#if defined(__FreeBSD__)
#include <assert.h>
#endif
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
 
#include "libc_private.h"
#include "un-namespace.h"

#ifndef __WASM_IMPORT
#define __WASM_IMPORT(module, symbol) __attribute__((import_module(#module), import_name(#symbol)))
#endif


// for wasm we use what the host gives us in crypto.getRandomValues() which in most
// cases uses the same source of random as /dev/random
void __random_source(void *buf, uint32_t bufsz) __WASM_IMPORT(kern, random_source);


static inline void _rs_random_u32(uint32_t *val)
{
    __random_source(val, sizeof(*val));
}

uint32_t arc4random(void)
{
	uint32_t ret;
	__random_source(&ret, sizeof(ret));
	return ret;
}

void _rs_random_buf(void *ptr, size_t len)
{
	__random_source(ptr, len);
}