#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <core_cmInstr.h>

namespace CM3 {

/**
 * @brief Atomic-processor-based operations
 *
 * operations based only on ARM-v7m capabilities
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

/**
 * @brief Default yield functors
 *
 * The functors over this namespace provide default
 * actions to yield the processor on blocking mutex
 * lock fail
 *
 * @see BlockingMutex
 * @see BlockingMutex::lock
 */
namespace Yield {

class Functor {
};

/**
 * @brief No operation yield functor
 *
 * This yield functor execute nop instruction when is called
 *
 * The natural companion for this yield functor is Notify::Noop
 */
class Noop: public Functor {
public:
	inline void operator()(void) {
		__NOP();
	}
};

/**
 * @brief Wait for interrupt yield functor
 *
 * This yield functor wait for interrupt when is called
 *
 * @warning To unlock the processor need generate some interrupt
 * @see Notify::SVC
 * @see Notify::PendSV
 */
class IntWait: public Functor {
public:
	inline void operator()(void) {
		__WFI();
	}
};

/**
 * @brief Wait for event yield functor
 *
 * This yield functor wait for event when is called
 *
 * @warning To unlock the processor need generate some event
 * @see Notify::Event
 */
class Event: public Functor {
public:
	inline void operator()(void) {
		__WFE();
	}
};

}

/**
 * @brief Default notify functors
 *
 * The functors over this namespace provide default
 * actions to notify the processor on blocking mutex
 * unlock function
 *
 * @see BlockingMutex
 * @see BlockingMutex::unlock
 */
namespace Notify {

class Functor {
};

/**
 * @brief No operation notify functor
 *
 * No operation is performed when this notify functor is called
 *
 * Natural yield functor companion is Yield::Noop
 */
class Noop: public Functor {
public:
	inline void operator()(void) {
		__NOP();
	}
};

/**
 * @brief Supervisor Call notify functor
 *
 * Generate a supervisor call interrupt when it is called
 *
 * This is one method to generate interruption event for
 * yield functor Yield::IntWait
 *
 * @see Notify::PendSV
 */
class SVC: public Functor {
public:
	inline void operator()(void) {
		__asm__ __volatile__("svc");
	}
};

/**
 * @brief Pending interrupt notify functor
 *
 * Generate a pending interrupt when it is called
 *
 * This is one method to generate interruption event for
 * yield functor Yield::IntWait
 *
 * @see Notify::SVC
 */
class PendSV: public Functor {
public:
	inline void operator()(void) {
		// trigger PendSV
		*((uint32_t volatile *)0xE000ED04) = 0x10000000;
	}
};

/**
 * @brief Event send notify functor
 *
 * Send event to the processor when it is called
 *
 * This is one method to generate event for Yield::Event
 * yield functor
 */
class Event: public Functor {
public:
	inline void operator()(void) {
		__SEV();
	}
};

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
	inline bool tryLock() {
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
	inline void unlock() {
		value = UNLOCK;
	}
};

/**
 * @brief Blocking mutex version
 *
 * This version of mutex can block thread using specific yield function
 *
 * @tparam m_yield Function to implement yield (nop by default)
 * @tparam m_notify Function to notify unlock action (nop by default)
 *
 * Some useful m_yield and m_notify implementations are over Implementation::Yield
 * and Implementation::Notify namespace
 *
 */
template<Implementation::Yield::Functor m_yield = Implementation::Yield::Noop,
		Implementation::Notify::Functor m_notify = Implementation::Notify::Noop>
class BlockingMutex {
	Mutex m;
/*
 * The basic yield function provides is:
 * @li @c Implementation::Yield::Noop No operation (asm nop) while mutex can't locked
 * @li @c Implementation::Yield::IntWait Wait for interrupt while mutex can't locked
 * @li @c Implementation::Yield::EventWait Wait for event while mutex can't locked
 *
 * The basic motify function provides is:
 * @li @c Implementation::Notify::Noop No operation (asm nop) for notify action
 * @li @c Implementation::Notify::SVC Generate supervisor call (asm svn) on notify action
 * @li @c Implementation::Notify::PendSV Generate PendSV call on notify action
 * @li @c Implementation::Notify::Event Generate send event (asm sev) on notify action
 *
 */
public:
	/**
	 * @brief Try to obtain mutex
	 * @return True if mutex can obtained
	 * @return False if not obtain mutex
	 */
	inline bool tryLock() {
		return m.tryLock();
	}

	/**
	 * @brief Obtain the mutex, block while not obtain
	 */
	inline void lock() {
		while (tryLock())
			m_yield();
	}

	/**
	 * @brief unlock locked mutex
	 *
	 * Unlock mutex and notify other thread via m_notify
	 */
	inline void unlock() {
		m.unlock();
		m_notify();
	}
};

} /* namespace Atomic */

} /* namespace CM3 */

#endif /* ATOMIC_H_ */
