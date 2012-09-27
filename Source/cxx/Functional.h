/*
 * Functional.h
 *
 *  Created on: 26/09/2012
 *      Author: PC 2010
 */

#ifndef FUNCTIONAL_H_
#define FUNCTIONAL_H_

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

typedef void (*LambdaCaller_t)(void);

template<typename Func>
inline LambdaCaller_t build(Func functor) {
	return [functor]() {
		functor();
	};
}

}

#endif /* FUNCTIONAL_H_ */
