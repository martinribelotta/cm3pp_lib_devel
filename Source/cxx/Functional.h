/*
 * Functional.h
 *
 *  Created on: 26/09/2012
 *      Author: PC 2010
 */

#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

/**
 * @brief Contains function objects and tools to create it
 *
 * The functional unit is the base of the callback functions
 * and task code manipulation.
 *
 * Using Functional::build(...) variants you can obtain a
 * object of code for a task scheduling or callback purpose
 */
namespace Functional {

template<typename Ret_T, typename Class_T, typename ... Args>
class GenericClassFunctor {
private:
	Class_T * const object;
	typedef Ret_T (Class_T::*functor_t)(Args...);

	functor_t method;

public:
	GenericClassFunctor(Class_T *o, functor_t m) :
			object(o), method(m) {
	}

	inline Ret_T operator()(Args ... v) const {
		return (object->*method)(v...);
	}
};

template<typename Ret_T, typename Class_T, typename ...Args>
inline const GenericClassFunctor<Ret_T, Class_T, Args...> build(
		Class_T * const obj, Ret_T (Class_T::* const m)(Args...)) {
	return GenericClassFunctor<Ret_T, Class_T, Args...>(obj, m);
}

/**
 * @brief Lambda code object
 *
 * Represent a lambda inline object of code or plain C function pointer
 *
 */
typedef void (*LambdaCaller_t)(void);

/**
 * @brief Build a #LambdaCaller_t from code functor or callabe object
 *
 * The functor parameter can be a lambda expression
 * or an object that defines operator()()
 *
 * @param functor Callable object
 * @return A LambdaCaller_t you can call as a simple functor
 */
template<typename Func>
inline LambdaCaller_t build(Func functor) {
	return [functor]() {
		functor();
	};
}

}

#endif /* FUNCTIONAL_H_ */
