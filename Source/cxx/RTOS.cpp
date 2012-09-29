#include "RTOS.h"

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <task.h>

#include <cstdlib>

namespace RTOS {

static inline uint32_t get_IPSR(void) {
	uint32_t r1 asm("r1") = 0;
	asm("MRS r1, IPSR");
	return r1;
}

#define EXCEPTION_MASK 0x0000001f

static inline bool isInTaskMode(void) {
	return (get_IPSR() & EXCEPTION_MASK) == 0;
}

void TaskHelper::trampoline(void *ptr) {
	reinterpret_cast<Task*>(ptr)->execute();
}

void Task::execute() {
	while (1) {
		if (func)
			func();
		suspend();
	}
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

void TaskHelper::suspend(Task *t) {
	vTaskSuspend((xTaskHandle) t->handler);
}

void TaskHelper::resume(Task *t) {
	if (isInTaskMode())
		vTaskResume((xTaskHandle) t->handler);
	else
		xTaskResumeFromISR((xTaskHandle) t->handler);
}

/**
 * @brief Start OS execution
 *
 * Initialize OS scheduler and never return.
 *
 * The O.S. may have crashed or some others
 * alterations has occurred
 */
void startRTOS() {
	vTaskStartScheduler();
}

/**
 * @brief Suspend execution of the current task
 * @param ticks Times (in OS ticks) to suspend task
 */
void taskWait(int ticks) {
	vTaskDelay(ticks);
}

/**
 * @brief Obtain the current tick of OS
 * @return Current time tick
 */
unsigned int currentTick(void) {
	if (isInTaskMode())
		return xTaskGetTickCount();
	else
		return xTaskGetTickCountFromISR();
}

}
