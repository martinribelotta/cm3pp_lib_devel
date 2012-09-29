#include "RTOS.h"

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <task.h>

#include <cstdlib>

namespace RTOS {

void TaskHelper::trampoline(void *ptr) {
	reinterpret_cast<Task*>(ptr)->execute();
}

void TaskHelper::registerTask(Task *t) {
	xTaskCreate( //
			&TaskHelper::trampoline,//
			(const signed char*)"",//
			configMINIMAL_STACK_SIZE,//
			(void*)t,//
			2,//
			&(t->handler)//
			);
}

void startRTOS() {
	vTaskStartScheduler();
}

void taskWait(int ticks) {
	vTaskDelay(ticks);
}

static inline uint32_t get_IPSR(void) {
	uint32_t r1 asm("r1");
    asm("MRS r1, IPSR");
    return r1;
}

#define EXCEPTION_MASK 0x0000001f

static inline bool isInTaskMode(void) {
	return (get_IPSR() & EXCEPTION_MASK) == 0;
}

unsigned int currentTick(void) {
	if (isInTaskMode())
		return xTaskGetTickCount();
	else
		return xTaskGetTickCountFromISR();
}

}
