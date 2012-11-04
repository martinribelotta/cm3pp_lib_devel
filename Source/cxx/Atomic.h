#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <core_cmInstr.h>

namespace CM3 {

/**
 * @brief Atomic processor-based operations
 *
 * Atomic operations based only on ARM-v7m capabilities
 */
namespace Atomic {

/**
 * @brief Implementation-detailed functions over ARM-v7m
 */
namespace Implementation {

/**
 * @brief Load exclusive BYTE access version
 * @param ptr Pointer to data
 * @return The contents of *ptr
 */
inline uint8_t ldrex(uint8_t *ptr) {
	return __LDREXB(ptr);
}

/**
 * @brief Load exclusive HALF-WORD access version
 * @param ptr Pointer to data
 * @return The contents of *ptr
 */
inline uint16_t ldrex(uint16_t *ptr) {
	return __LDREXH(ptr);
}

/**
 * @brief Load exclusive WORD access version
 * @param ptr Pointer to data
 * @return The contents of *ptr
 */
inline uint32_t ldrex(uint32_t *ptr) {
	return __LDREXW(ptr);
}

/**
 * @brief Store exclusive BYTE access version
 * @param v The value to write
 * @param ptr Pointer to store data
 * @return 0 if data stored is correct
 * @return 1 if data stored is other than v
 */
inline uint8_t strex(uint8_t v, uint8_t *ptr) {
	return __STREXB(v, ptr);
}

/**
 * @brief Store exclusive HALF-WORD access version
 * @param v The value to write
 * @param ptr Pointer to store data
 * @return 0 if data stored is correct
 * @return 1 if data stored is other than v
 */
inline uint16_t strex(uint16_t v, uint16_t *ptr) {
	return __STREXH(v, ptr);
}

/**
 * @brief Store exclusive WORD access version
 * @param v The value to write
 * @param ptr Pointer to store data
 * @return 0 if data stored is correct
 * @return 1 if data stored is other than v
 */
inline uint32_t strex(uint32_t v, uint32_t *ptr) {
	return __STREXW(v, ptr);
}

/**
 * @brief Data memory barrier
 * This function ensures the apparent order of the explicit memory operations before
 * and after the instruction, without ensuring their completion.
 */
inline void dataMemoryBarrier(void) {
	__DMB();
}

/**
 * @brief Data Synchronization Barrier
 * This function acts as a special kind of Data Memory Barrier.
 * It completes when all explicit memory accesses before this instruction complete.
 */
inline void dataSyncBarrier(void) {
	__DSB();
}

/**
 * @brief Instruction Synchronization Barrier
 * Instruction Synchronization Barrier flushes the pipeline in the processor,
 * so that all instructions following the ISB are fetched from cache or
 * memory, after the instruction has been completed.
 */
inline void instructionBarrier(void) {
	__ISB();
}

}

/**
 * @brief Get data and set new value atomically
 *
 * Read data of *ptr and set the new value n returning
 * the previous.
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param n The new value
 * @param ptr Pointer of data to operate
 * @return The old value of *ptr
 */
template<typename T>
inline T get_and_set(T n, T *ptr) {
	T v;
	do {
		v = Implementation::ldrex(ptr);
		Implementation::dataMemoryBarrier();
	} while (Implementation::strex(n, ptr));
	return v;
}

/**
 * @brief Fetch and add atomically
 *
 * Fetch the old value of *ptr and add n to this
 * returning the old value of *ptr
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param n The value to add
 * @param ptr Pointer of data to operate
 * @return The old value of *ptr
 */
template<typename T>
inline T fetch_and_add(T n, T *ptr) {
	T v;
	do {
		v = Implementation::ldrex(ptr);
		Implementation::dataMemoryBarrier();
	} while (Implementation::strex(v + n, ptr));
	return v;
}

/**
 * @brief Fetch and subtract atomically
 *
 * Fetch the old value of *ptr and subtract n to this
 * returning the old value of *ptr
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param n The value to subtract
 * @param ptr Pointer of data to operate
 * @return The old value of *ptr
 */
template<typename T>
inline T fetch_and_sub(T n, T *ptr) {
	T v;
	do {
		v = Implementation::ldrex(ptr);
		Implementation::dataMemoryBarrier();
	} while (Implementation::strex(v - n, ptr));
	return v;
}

/**
 * @brief Fetch and increment atomically
 *
 * Fetch the old value of *ptr and increment it
 * returning the old value of *ptr
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param ptr Pointer of data to operate
 * @return The old value of *ptr
 */
template<typename T>
T fetch_and_inc(T *ptr) {
	return fetch_and_add(1, ptr);
}

/**
 * @brief Fetch and decrement atomically
 *
 * Fetch the old value of *ptr and decrement it
 * returning the old value of *ptr
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param ptr Pointer of data to operate
 * @return The old value of *ptr
 */
template<typename T>
T fetch_and_dec(T *ptr) {
	return fetch_and_sub(1, ptr);
}

/**
 * @brief Get a value from *ptr into *old_value and try to set to new_value
 *
 * @tparam T type of data.
 *
 * The possible data type of T is
 * @li @c uint8_t unsigned 8bit integer (BYTE)
 * @li @c uint16_t unsigned 16bit integer (HALF-WORD)
 * @li @c uint32_t unsigned 32bit integer (WORD)
 *
 * @param ptr Pointer of memory to modify
 * @param new_value The new value to store
 * @param old_value Pointer of memory to store the old value
 * @return True if *ptr can set to new_value
 * @return False if *ptr can't set to new_value (because another process modify *ptr instread)
 */
template<typename T>
inline bool try_get_and_set(T *ptr, T new_value, T *old_value) {
	*old_value = Implementation::ldrex(ptr);
	Implementation::dataMemoryBarrier();
	return Implementation::strex(new_value, ptr) == 0;
}

/**
 * @brief Processor based mutex
 *
 * Mutex implemented with processor only resources
 *
 * Because this class can't know to yield processor to other threads
 * only non-block primitives is implemented.
 *
 * Additionally, #unlock primitive is unsafe if used over a
 * non-obtained mutex
 */
class Mutex {
	uint8_t value;
public:
	enum {
		LOCK = 0, //!< Lock state
		UNLOCK = 1, //!< Unlock state
	};

	/**
	 * @brief Create an unlocked mutex
	 */
	Mutex() :
			value(UNLOCK) {
	}

	/**
	 * @brief Try to obtain mutex
	 * @return True if mutex can obtained
	 * @return False if not obtain mutex
	 */
	bool tryLock() {
		uint8_t old;
		if (try_get_and_set(&value, uint8_t(LOCK), &old))
			return old == UNLOCK;
		return false;
	}

	/**
	 * @brief unlock obtained mutex
	 * @warning This function can't test if unlock-caller is
	 * the mutex obtainer.
	 */
	void unlock() {
		value = UNLOCK;
	}
};

} /* namespace Atomic */

} /* namespace CM3 */

#endif /* ATOMIC_H_ */
