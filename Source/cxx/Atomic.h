#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <core_cmInstr.h>

namespace CM3 {

namespace Atomic {

inline uint8_t atomic_fetch_and_add(uint8_t n, uint8_t *ptr) {
	uint8_t v;
	do {
		v = __LDREXB(ptr);
	} while (__STREXB(v + n, ptr));
	return v;
}

inline uint16_t atomic_fetch_and_add(uint16_t n, uint16_t *ptr) {
	uint16_t v;
	do {
		v = __LDREXH(ptr);
	} while (__STREXH(v + n, ptr));
	return v;
}

inline uint32_t atomic_fetch_and_add(uint32_t n, uint32_t *ptr) {
	uint32_t v;
	do {
		v = __LDREXW(ptr);
	} while (__STREXW(v + n, ptr));
	return v;
}

inline uint8_t atomic_fetch_and_sub(uint8_t n, uint8_t *ptr) {
	uint8_t v;
	do {
		v = __LDREXB(ptr);
	} while (__STREXB(v - n, ptr));
	return v;
}

inline uint16_t atomic_fetch_and_sub(uint16_t n, uint16_t *ptr) {
	uint16_t v;
	do {
		v = __LDREXH(ptr);
	} while (__STREXH(v - n, ptr));
	return v;
}

inline uint32_t atomic_fetch_and_sub(uint32_t n, uint32_t *ptr) {
	uint32_t v;
	do {
		v = __LDREXW(ptr);
	} while (__STREXW(v - n, ptr));
	return v;
}

template<typename T>
T atomic_fetch_and_inc(T *ptr) {
	return atomic_fetch_and_add(1, ptr);
}

template<typename T>
T atomic_fetch_and_dec(T *ptr) {
	return atomic_fetch_and_sub(1, ptr);
}

} /* namespace Atomic */

} /* namespace CM3 */

#endif /* ATOMIC_H_ */
