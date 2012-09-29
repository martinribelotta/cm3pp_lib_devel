#ifndef RTOS_H_
#define RTOS_H_

#include "Functional.h"

#include <cstddef>

/**
 * @brief Real Time OS high level layer
 *
 * This block abstract the OS implementation with a C++
 */
namespace RTOS {

class Task;

/**
 * @internal For internal use of #Task class
 */
class TaskHelper {
private:
	friend class Task;

	static void registerTask(Task* t);
	static void suspend(Task *t);
	static void resume(Task *t);

	static void trampoline(void *);
};

/**
 * @brief OS task wrapper
 *
 * represents a block of code that runs in a separate task
 *
 * To create a task can only define a new object Task with
 * the corresponding functor on the constructor such as:
 * @code
 *    Task t1(Functional::build([] () {
 *       // My task code
 *    }));
 * @endcode
 * the ability to use functors allows arbitrarily to call
 * a complex object code such as class members or code closures
 * with a minimum overhead.
 *
 * The call of a member code is simple than:
 * @code
 *    void MyObject::myConfigurationMember() {
 *       taskPointer = new Task(Functional::build([this]() {
 *          this->myNonVirtualMember();
 *       }));
 *    }
 * @endcode
 *
 * Additionally, you can create a task without a functor
 * and later move a functor to this thread same as:
 *
 * - Create static task
 * @code
 * Task socketProcessTask;
 * @endcode
 *
 * - Start new functor on the task
 * @code
 *    void MyServer::newPetition() {
 *       // Imaginary methods and objects for illustration purpose
 *       CurrentPetition p = socket.getPetition();
 *       socketProcessTask.moveToTask([this, p]() {
 *           this->processIncommingData(p);
 *           // When task terminate, switch to suspend mode
 *       });
 *    }
 * @endcode
 *
 * - Start or create task with a plain, C style, function pointer
 * @code
 * static void processLed(void) {
 *    // Some plain C code
 * }
 * ...
 * Task ledTask(processLed);
 * ...
 *     extern void processLedFast(void);
 *     ledTask.moveToTask(processLedFast);
 * @endcode
 */
class Task {
private:
	Functional::LambdaCaller_t func;
	void *handler;

public:
	/**
	 * @brief Create task with a functor
	 *
	 * The functor can't expected any parameters
	 * and the return value of it is ignored.
	 *
	 * If you need parameters, you can pass it on a captured
	 * closure list
	 *
	 * @param f Functor of code as callable object without parameter
	 */
	Task(Functional::LambdaCaller_t f) :
			func(f), handler(0l) {
		TaskHelper::registerTask(this);
	}

	/**
	 * @brief Create an empty task
	 *
	 * The empty task start suspended and with for call
	 * of \ref moveToTask(Functional::LambdaCaller_t)
	 *
	 * if you can call \ref resume() from an empty task
	 * the object simply execute suspend with the scheduler
	 * call this object without a functor
	 */
	Task() :
			func(0l), handler(0l) {
		TaskHelper::registerTask(this);
		suspend();
	}

	/**
	 * @brief Move functor to this task
	 *
	 * The actions for this function involves:
	 *
	 * - Suspend the task (if not suspended)
	 * - Set the functor code to f (replacing the last)
	 * - Resume task for scheduling
	 *
	 * The rules for the functor is same as \ref Task(Functional::LambdaCaller_t)
	 *
	 * @param f Functor of code to execute in task
	 */
	void moveToTask(Functional::LambdaCaller_t f) {
		suspend();
		func = f;
		resume();
	}

	/**
	 * @brief Suspend task for scheduling
	 *
	 * This action eliminate task for the list of scheduler
	 * but maintain the memory and the status of the thread
	 */
	inline void suspend() {
		TaskHelper::suspend(this);
	}

	/**
	 * @brief Resume task suspended for scheduling
	 *
	 * This action re enable the scheduling of the thread
	 * with the actual statue of this
	 */
	inline void resume() {
		TaskHelper::resume(this);
	}

private:
	void execute();

	friend class TaskHelper;
};

extern void startRTOS();
extern void taskWait(int ticks);
extern unsigned int currentTick(void);

}

#endif /* RTOS_H_ */
