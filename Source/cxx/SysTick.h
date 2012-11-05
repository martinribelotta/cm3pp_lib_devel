/*
 * SysTick.h
 *
 *  Created on: 11/09/2012
 *      Author: user001
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <stm32f10x.h>

namespace ARMV7M {

class SystemTick_t: SysTick_Type {
public:
	SystemTick_t() = delete;

	void init(uint32_t ticks = SystemCoreClock) {
#ifndef RTOS_ENABLED
		SysTick_Config(ticks / 1000);
#endif
	}

	inline int current_tick() const {
		return m_current_tick;
	}

	inline void clear_tick() const {
		m_current_tick = 0;
	}

	inline void delay(int ticks) const {
		int future_tick = m_current_tick + ticks;
		while (m_current_tick < future_tick)
			;
	}

private:
	static volatile int m_current_tick;

	friend void SysTick_Handler_cpp(void);
};

static SystemTick_t& SystemTick = *reinterpret_cast<SystemTick_t*>(SysTick );

}
/* namespace CM3 */
#endif /* SYSTICK_H_ */
