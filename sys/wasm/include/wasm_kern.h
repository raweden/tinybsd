

#ifndef _WASM_KERN_H_
#define _WASM_KERN_H_

// function which is used as a special marker, it marks the callsite which follows as one that is
// performed on the kernel main thread, this concept only exists within WebAssembly.
void wasm_main_callsite(void (*fn)(void));


#endif /* _WASM_KERN_H_ */