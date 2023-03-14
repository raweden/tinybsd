
#ifndef _MACHINE_ATOMIC_WASM_H_
#define _MACHINE_ATOMIC_WASM_H_

#include <sys/stdint.h>

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

uint8_t atomic_load8(const uint8_t *valp);
void atomic_store8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_add8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_sub8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_and8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_or8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_xor8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_xchg8(volatile uint8_t *valp, uint8_t val);
uint8_t atomic_cmpxchg8(volatile uint8_t *valp, uint8_t expected, uint8_t replacement);

uint16_t atomic_load16(const uint16_t *valp);
void atomic_store16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_add16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_sub16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_and16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_or16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_xor16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_xchg16(volatile uint16_t *valp, uint16_t val);
uint16_t atomic_cmpxchg16(volatile uint16_t *valp, uint16_t expected, uint16_t replacement);

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