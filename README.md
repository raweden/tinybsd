# tinybsd

tinybsd is a fork of the FreeBSD repository, to refactor, rewrite and optimize the source code to make it run in the WebAssembly runtime. The main objective of this undertaking is to provide a kernel which acts as the backend for libc.

*From whats mentioned in the README of the orignal repo i also mention the copyright notes here;*  
For copyright information, please see [the file COPYRIGHT](COPYRIGHT) in this directory. Additional copyright information also exists for some sources in this tree - please see the specific source directories for more information.

A link to the original [FreeBSD README](README-FreeBSD.md)

**I have no intention to merge this codebase into the original repo as WebAssembly will require changes which goes beyond the machine-depedant source code of the architectures supported by freebsd.**

### Why?

The motiviation behind this project goes back the obession of porting GnuStep to WebAssembly.

The benefits of having a standard compliant kernel backing the libc for WebAssembly, is not benefitial for one single purpose but the broad spectrum of any c source which is ported to WebAssembly. There is not that many projects which tries todo this, there is emscripten which generates some handling of the syscalls, but the feature set provided by emscripten is limited.

### Initial Goals

- Port the kernel, this without support Virtual Memory mapping. [it does compile, but have not figured out on how to surgically remove VM/UMA/MMU]
- Provide a *idbfs* driver for the filesystem which uses IndexedDB as its backing store.
- Port the binaries of `/bin` etc to provide a minimal unix system. 
