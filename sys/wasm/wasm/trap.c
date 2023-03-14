/*-
 * Copyright (c) 2022-2023 Jesper Svensson <me@raweden.se>
 * All rights reserved.
 *
 * Portions of this software were developed by SRI International and the
 * University of Cambridge Computer Laboratory under DARPA/AFRL contract
 * FA8750-10-C-0237 ("CTSRD"), as part of the DARPA CRASH research programme.
 *
 * Portions of this software were developed by the University of Cambridge
 * Computer Laboratory as part of the CTSRD Project, with support from the
 * UK Higher Education Innovation Fund (HEIF).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/ktr.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/bus.h>
#include <sys/proc.h>
#include <sys/ptrace.h>
#include <sys/syscall.h>
#include <sys/sysent.h>
#ifdef KDB
#include <sys/kdb.h>
#endif

#include <vm/vm.h>
#include <vm/pmap.h>
#include <vm/vm_kern.h>
#include <vm/vm_map.h>
#include <vm/vm_param.h>
#include <vm/vm_extern.h>

#ifdef FPE
#include <machine/fpe.h>
#endif
#include <machine/frame.h>
#include <machine/pcb.h>
#include <machine/pcpu.h>

#include <machine/resource.h>
#include <machine/intr.h>

#ifndef __WASM
#error "This source should only be used with WebAssembly"
#endif

#include <machine/wasm_module.h>

int cpu_fetch_syscall_args(struct thread *td)
{
	struct proc *p;
	syscallarg_t *ap, *dst_ap;
	struct syscall_args *sa;

	p = td->td_proc;
	sa = &td->td_sa;
	ap = &td->td_frame->tf_a[0];
	dst_ap = &sa->args[0];

	sa->code = td->td_frame->tf_t[0];
	sa->original_code = sa->code;

	if (__predict_false(sa->code == SYS_syscall || sa->code == SYS___syscall)) {
		sa->code = *ap++;
	} else {
		*dst_ap++ = *ap++;
	}

	if (__predict_false(sa->code >= p->p_sysent->sv_size))
		sa->callp = &p->p_sysent->sv_table[0];
	else
		sa->callp = &p->p_sysent->sv_table[sa->code];

	KASSERT(sa->callp->sy_narg <= nitems(sa->args), ("Syscall %d takes too many arguments", sa->code));

	memcpy(dst_ap, ap, (NARGREG - 1) * sizeof(*dst_ap));

	td->td_retval[0] = 0;
	td->td_retval[1] = 0;

	return (0);
}

#include "../sys/kern/subr_syscall.c"

__WASM_EXPORT(do_syscall_handler)
void do_syscall_handler(void)
{
	struct thread *td = curthread;

	syscallenter(td);
	syscallret(td);
}
