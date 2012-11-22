#include "RTOS.h"

#include <FreeRTOS.h>
#include <stm32f10x.h>
#include <task.h>

#include <cstdlib>

#undef IPSR_IN_TASK_METHOD

namespace RTOS {

#ifdef IPSR_IN_TASK_METHOD
static inline uint32_t get_IPSR(void) {
	uint32_t ipsr;
	__asm__ __volatile__("mrs %0, ipsr" :"=r"(ipsr)::);
	return ipsr;
}

#define EXCEPTION_MASK 0x0000001f
#endif

/**
 * @brief Determine if processor is in task mode
 *
 * If processor is in task mode (no ISR) return TRUE value
 *
 * @return True if processor is in task mode. False if in handler mode
 */
bool isInTaskMode(void) {
#ifdef IPSR_IN_TASK_METHOD
	return (get_IPSR() & EXCEPTION_MASK) == 0;
#else
	return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk);
#endif
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
	else {
		portBASE_TYPE yReq = xTaskResumeFromISR((xTaskHandle) t->handler);
		if (yReq != pdFALSE)
			ISRContext::setNeedResched();
	}
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
 * @brief Causes the current task yield the processor
 */
void taskYield(void) {
	taskYIELD();
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

bool ISRContext::needResched = false;

/**
 * @brief Inform to kernel the entering of ISR
 *
 * Call this function on ISR entering
 * @see ISRContext::leaveISR(void);
 */
void ISRContext::enterISR(void) {
	needResched = false;
}

/**
 * @brief Inform to kernel the finalization of ISR
 *
 * Call this function on end of ISR
 * @see ISRContext::leaveISR(void);
 */
void ISRContext::leaveISR(void) {
	if (needResched) {
		vPortYieldFromISR();
		needResched = false;
	}
}

}
