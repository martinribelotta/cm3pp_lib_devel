/*
 * Mutex.cpp
 *
 *  Created on: 29/09/2012
 *      Author: PC 2010
 */

#include "Mutex.h"
#include "RTOS.h"

#include <FreeRTOS.h>
#include <semphr.h>

namespace RTOS {

Mutex::Mutex() :
		handler(xSemaphoreCreateMutex()) //
{
}

Mutex::~Mutex() {
	vSemaphoreDelete(handler);
}

void Mutex::lock() {
	if (isInTaskMode()) {
		xSemaphoreTake(handler, portMAX_DELAY);
	} else {
		// TODO: Add code to take mutex on ISR
	}
}

bool Mutex::tryLock() {
	if (isInTaskMode()) {
		return xSemaphoreTake(handler, 0) == pdTRUE;
	} else {
		// TODO: Add code to take mutex on ISR
		return false;
	}
}

void Mutex::unlock() {
	if (isInTaskMode())
		xSemaphoreGive(handler);
	else {
		signed portBASE_TYPE yReq;
		xSemaphoreGiveFromISR(handler, &yReq);
		if (yReq)
			ISRContext::setNeedResched();
	}
}

} /* namespace RTOS */
