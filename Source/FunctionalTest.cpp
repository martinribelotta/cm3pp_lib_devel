#include <cxx/Functional.h>
#include <cxx/USBStream.h>

#ifndef __GXX_EXPERIMENTAL_CXX0X__
#define __GXX_EXPERIMENTAL_CXX0X__
#endif

#ifdef __GXX_RTTI
#undef __GXX_RTTI
#endif

#include <functional>

class A {
public:
	int f(char a, float b) {
		return int(a * b);
	}
};

void functional_test() {
	A a;
	auto f = Functional::build(&a, &A::f);
	f('a', 1.2);
}

class LambdaTester {
	const Functional::LambdaCaller_t lamb;
public:
	LambdaTester() :
			lamb(Functional::build([this] (void) -> void {
				OwnStream::usb << "Hello lambda";
			})) {
	}

	void callLambda() const {
		lamb();
	}
};

void lambda_test() {
	LambdaTester().callLambda();
}
