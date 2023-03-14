/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)errno.h	8.5 (Berkeley) 1/21/94
 * $FreeBSD$
 */

#ifndef _SYS_ERRNO_H_
#define _SYS_ERRNO_H_

#if !defined(_KERNEL) && !defined(_STANDALONE)
#include <sys/cdefs.h>
__BEGIN_DECLS
int *	__error(void);
__END_DECLS
#define	errno		(* __error())
#endif

#ifdef __WASI_COMPAT_ERRNO

#define	EPERM		63		/* Operation not permitted */
#define	ENOENT		44		/* No such file or directory */
#define	ESRCH		71		/* No such process */
#define	EINTR		27		/* Interrupted system call */
#define	EIO		   29		/* Input/output error */
#define	ENXIO		60		/* Device not configured */
#define	E2BIG		1		/* Argument list too long */
#define	ENOEXEC		45		/* Exec format error */
#define	EBADF		8		/* Bad file descriptor */
#define	ECHILD		12		/* No child processes */
#define	EDEADLK		16		/* Resource deadlock avoided */
					/* 11 was EAGAIN */
#define	ENOMEM		48		/* Cannot allocate memory */
#define	EACCES		2		/* Permission denied */
#define	EFAULT		21		/* Bad address */
#ifndef _POSIX_SOURCE
#define	ENOTBLK		105		/* Block device required */
#endif
#define	EBUSY		10		/* Device busy */
#define	EEXIST		20		/* File exists */
#define	EXDEV		75		/* Cross-device link */
#define	ENODEV		43		/* Operation not supported by device */
#define	ENOTDIR		54		/* Not a directory */
#define	EISDIR		31		/* Is a directory */
#define	EINVAL		28		/* Invalid argument */
#define	ENFILE		41		/* Too many open files in system */
#define	EMFILE		33		/* Too many open files */
#define	ENOTTY		59		/* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define	ETXTBSY		74		/* Text file busy */
#endif
#define	EFBIG		22		/* File too large */
#define	ENOSPC		51		/* No space left on device */
#define	ESPIPE		70		/* Illegal seek */
#define	EROFS		69		/* Read-only filesystem */
#define	EMLINK		34		/* Too many links */
#define	EPIPE		64		/* Broken pipe */

/* math software */
#define	EDOM		18		/* Numerical argument out of domain */
#define	ERANGE		68		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		6		/* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	26		/* Operation now in progress */
#define	EALREADY	7		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	57		/* Socket operation on non-socket */
#define	EDESTADDRREQ	17		/* Destination address required */
#define	EMSGSIZE	35		/* Message too long */
#define	EPROTOTYPE	67		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	50		/* Protocol not available */
#define	EPROTONOSUPPORT	66		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	137		/* Socket type not supported */
#define	EOPNOTSUPP	138		/* Operation not supported */
#define	ENOTSUP		EOPNOTSUPP	/* Operation not supported */
#define	EPFNOSUPPORT	139		/* Protocol family not supported */
#define	EAFNOSUPPORT	5		/* Address family not supported by protocol family */
#define	EADDRINUSE	3		/* Address already in use */
#define	EADDRNOTAVAIL	4		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	38		/* Network is down */
#define	ENETUNREACH	40		/* Network is unreachable */
#define	ENETRESET	39		/* Network dropped connection on reset */
#define	ECONNABORTED	13		/* Software caused connection abort */
#define	ECONNRESET	15		/* Connection reset by peer */
#define	ENOBUFS		42		/* No buffer space available */
#define	EISCONN		30		/* Socket is already connected */
#define	ENOTCONN	53		/* Socket is not connected */
#define	ESHUTDOWN	140		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	141		/* Too many references: can't splice */
#define	ETIMEDOUT	73		/* Operation timed out */
#define	ECONNREFUSED	14		/* Connection refused */

#define	ELOOP		32		/* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
#define	ENAMETOOLONG	37		/* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define	EHOSTDOWN	142		/* Host is down */
#define	EHOSTUNREACH	23		/* No route to host */
#endif /* _POSIX_SOURCE */
#define	ENOTEMPTY	55		/* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define	EPROCLIM	519		/* Too many processes */
#define	EUSERS		136		/* Too many users */
#define	EDQUOT		19		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		72		/* Stale NFS file handle */
#define	EREMOTE		121		/* Too many levels of remote in path */
#define	EBADRPC		520		/* RPC struct is bad */
#define	ERPCMISMATCH	521		/* RPC version wrong */
#define	EPROGUNAVAIL	522		/* RPC prog. not avail */
#define	EPROGMISMATCH	523		/* Program version wrong */
#define	EPROCUNAVAIL	524		/* Bad procedure for program */
#endif /* _POSIX_SOURCE */

#define	ENOLCK		46		/* No locks available */
#define	ENOSYS		52		/* Function not implemented */

#ifndef _POSIX_SOURCE
#define	EFTYPE		525		/* Inappropriate file type or format */
#define	EAUTH		526		/* Authentication error */
#define	ENEEDAUTH	527		/* Need authenticator */
#define	EIDRM		24		/* Identifier removed */
#define	ENOMSG		49		/* No message of desired type */
#define	EOVERFLOW	61		/* Value too large to be stored in data type */
#define	ECANCELED	11		/* Operation canceled */
#define	EILSEQ		25		/* Illegal byte sequence */
#define	ENOATTR		528		/* Attribute not found */

#define	EDOOFUS		529		/* Programming error */
#endif /* _POSIX_SOURCE */

#define	EBADMSG		9		/* Bad message */
#define	EMULTIHOP	36		/* Multihop attempted */
#define	ENOLINK		47		/* Link has been severed */
#define	EPROTO		65		/* Protocol error */

#ifndef _POSIX_SOURCE
#define	ENOTCAPABLE	530		/* Capabilities insufficient */
#define	ECAPMODE	531		/* Not permitted in capability mode */
#define	ENOTRECOVERABLE	56		/* State not recoverable */
#define	EOWNERDEAD	62		/* Previous owner died */
#define	EINTEGRITY	532		/* Integrity check failed */
#endif /* _POSIX_SOURCE */

#else

// original freebsd errno
#define	EPERM		1		/* Operation not permitted */
#define	ENOENT		2		/* No such file or directory */
#define	ESRCH		3		/* No such process */
#define	EINTR		4		/* Interrupted system call */
#define	EIO		5		/* Input/output error */
#define	ENXIO		6		/* Device not configured */
#define	E2BIG		7		/* Argument list too long */
#define	ENOEXEC		8		/* Exec format error */
#define	EBADF		9		/* Bad file descriptor */
#define	ECHILD		10		/* No child processes */
#define	EDEADLK		11		/* Resource deadlock avoided */
					/* 11 was EAGAIN */
#define	ENOMEM		12		/* Cannot allocate memory */
#define	EACCES		13		/* Permission denied */
#define	EFAULT		14		/* Bad address */
#ifndef _POSIX_SOURCE
#define	ENOTBLK		15		/* Block device required */
#endif
#define	EBUSY		16		/* Device busy */
#define	EEXIST		17		/* File exists */
#define	EXDEV		18		/* Cross-device link */
#define	ENODEV		19		/* Operation not supported by device */
#define	ENOTDIR		20		/* Not a directory */
#define	EISDIR		21		/* Is a directory */
#define	EINVAL		22		/* Invalid argument */
#define	ENFILE		23		/* Too many open files in system */
#define	EMFILE		24		/* Too many open files */
#define	ENOTTY		25		/* Inappropriate ioctl for device */
#ifndef _POSIX_SOURCE
#define	ETXTBSY		26		/* Text file busy */
#endif
#define	EFBIG		27		/* File too large */
#define	ENOSPC		28		/* No space left on device */
#define	ESPIPE		29		/* Illegal seek */
#define	EROFS		30		/* Read-only filesystem */
#define	EMLINK		31		/* Too many links */
#define	EPIPE		32		/* Broken pipe */

/* math software */
#define	EDOM		33		/* Numerical argument out of domain */
#define	ERANGE		34		/* Result too large */

/* non-blocking and interrupt i/o */
#define	EAGAIN		35		/* Resource temporarily unavailable */
#ifndef _POSIX_SOURCE
#define	EWOULDBLOCK	EAGAIN		/* Operation would block */
#define	EINPROGRESS	36		/* Operation now in progress */
#define	EALREADY	37		/* Operation already in progress */

/* ipc/network software -- argument errors */
#define	ENOTSOCK	38		/* Socket operation on non-socket */
#define	EDESTADDRREQ	39		/* Destination address required */
#define	EMSGSIZE	40		/* Message too long */
#define	EPROTOTYPE	41		/* Protocol wrong type for socket */
#define	ENOPROTOOPT	42		/* Protocol not available */
#define	EPROTONOSUPPORT	43		/* Protocol not supported */
#define	ESOCKTNOSUPPORT	44		/* Socket type not supported */
#define	EOPNOTSUPP	45		/* Operation not supported */
#define	ENOTSUP		EOPNOTSUPP	/* Operation not supported */
#define	EPFNOSUPPORT	46		/* Protocol family not supported */
#define	EAFNOSUPPORT	47		/* Address family not supported by protocol family */
#define	EADDRINUSE	48		/* Address already in use */
#define	EADDRNOTAVAIL	49		/* Can't assign requested address */

/* ipc/network software -- operational errors */
#define	ENETDOWN	50		/* Network is down */
#define	ENETUNREACH	51		/* Network is unreachable */
#define	ENETRESET	52		/* Network dropped connection on reset */
#define	ECONNABORTED	53		/* Software caused connection abort */
#define	ECONNRESET	54		/* Connection reset by peer */
#define	ENOBUFS		55		/* No buffer space available */
#define	EISCONN		56		/* Socket is already connected */
#define	ENOTCONN	57		/* Socket is not connected */
#define	ESHUTDOWN	58		/* Can't send after socket shutdown */
#define	ETOOMANYREFS	59		/* Too many references: can't splice */
#define	ETIMEDOUT	60		/* Operation timed out */
#define	ECONNREFUSED	61		/* Connection refused */

#define	ELOOP		62		/* Too many levels of symbolic links */
#endif /* _POSIX_SOURCE */
#define	ENAMETOOLONG	63		/* File name too long */

/* should be rearranged */
#ifndef _POSIX_SOURCE
#define	EHOSTDOWN	64		/* Host is down */
#define	EHOSTUNREACH	65		/* No route to host */
#endif /* _POSIX_SOURCE */
#define	ENOTEMPTY	66		/* Directory not empty */

/* quotas & mush */
#ifndef _POSIX_SOURCE
#define	EPROCLIM	67		/* Too many processes */
#define	EUSERS		68		/* Too many users */
#define	EDQUOT		69		/* Disc quota exceeded */

/* Network File System */
#define	ESTALE		70		/* Stale NFS file handle */
#define	EREMOTE		71		/* Too many levels of remote in path */
#define	EBADRPC		72		/* RPC struct is bad */
#define	ERPCMISMATCH	73		/* RPC version wrong */
#define	EPROGUNAVAIL	74		/* RPC prog. not avail */
#define	EPROGMISMATCH	75		/* Program version wrong */
#define	EPROCUNAVAIL	76		/* Bad procedure for program */
#endif /* _POSIX_SOURCE */

#define	ENOLCK		77		/* No locks available */
#define	ENOSYS		78		/* Function not implemented */

#ifndef _POSIX_SOURCE
#define	EFTYPE		79		/* Inappropriate file type or format */
#define	EAUTH		80		/* Authentication error */
#define	ENEEDAUTH	81		/* Need authenticator */
#define	EIDRM		82		/* Identifier removed */
#define	ENOMSG		83		/* No message of desired type */
#define	EOVERFLOW	84		/* Value too large to be stored in data type */
#define	ECANCELED	85		/* Operation canceled */
#define	EILSEQ		86		/* Illegal byte sequence */
#define	ENOATTR		87		/* Attribute not found */

#define	EDOOFUS		88		/* Programming error */
#endif /* _POSIX_SOURCE */

#define	EBADMSG		89		/* Bad message */
#define	EMULTIHOP	90		/* Multihop attempted */
#define	ENOLINK		91		/* Link has been severed */
#define	EPROTO		92		/* Protocol error */

#ifndef _POSIX_SOURCE
#define	ENOTCAPABLE	93		/* Capabilities insufficient */
#define	ECAPMODE	94		/* Not permitted in capability mode */
#define	ENOTRECOVERABLE	95		/* State not recoverable */
#define	EOWNERDEAD	96		/* Previous owner died */
#define	EINTEGRITY	97		/* Integrity check failed */
#endif /* _POSIX_SOURCE */

#endif

#ifndef _POSIX_SOURCE
#define	ELAST		97		/* Must be equal largest errno */
#endif /* _POSIX_SOURCE */

#if defined(_KERNEL) || defined(_WANT_KERNEL_ERRNO) || defined(_STANDALONE)
/* pseudo-errors returned inside kernel to modify return to process */
#define	ERESTART	(-1)		/* restart syscall */
#define	EJUSTRETURN	(-2)		/* don't modify regs, just return */
#define	ENOIOCTL	(-3)		/* ioctl not handled by this layer */
#define	EDIRIOCTL	(-4)		/* do direct ioctl in GEOM */
#define	ERELOOKUP	(-5)		/* retry the directory lookup */
#endif

#ifndef _KERNEL
#if __EXT1_VISIBLE
/* ISO/IEC 9899:2011 K.3.2.2 */
#ifndef _ERRNO_T_DEFINED
#define _ERRNO_T_DEFINED
typedef int errno_t;
#endif
#endif /* __EXT1_VISIBLE */
#endif

#endif
