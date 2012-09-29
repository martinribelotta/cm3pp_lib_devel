#ifndef RTOS_H_
#define RTOS_H_

#include "Functional.h"

#include <cstddef>

namespace RTOS {

class Task;

class TaskHelper {
private:
	friend class Task;

	static void registerTask(Task *t);

	static void trampoline(void *);

};

class Task {
	const Functional::LambdaCaller_t func;
	void *handler;
public:
	Task(Functional::LambdaCaller_t f) :
			func(f), handler(0l) {
		TaskHelper::registerTask(this);
	}

private:
	void execute() const {
		func();
	}

	friend class TaskHelper;
};

extern void startRTOS();

extern void taskWait(int ticks);

extern unsigned int currentTick(void);

}

#endif /* RTOS_H_ */
