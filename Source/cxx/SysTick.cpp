/*
 * SysTick.cpp
 *
 *  Created on: 11/09/2012
 *      Author: user001
 */

#include "SysTick.h"

namespace CM3 {

#if 0
Systick SystemTick;

inline void SysTick_Handler_cpp(void) {
	SystemTick.m_current_tick++;
}
#endif

volatile int SystemTick_t::m_current_tick = 0;

inline void SysTick_Handler_cpp(void) {
	SystemTick_t::m_current_tick++;
}

} /* namespace CM3 */

extern "C" void SysTick_Handler(void) {
	CM3::SysTick_Handler_cpp();
}
