/*
 * Print.h
 *
 *  Created on: 16/09/2012
 *      Author: ruso
 */

#ifndef PRINT_H_
#define PRINT_H_

#include <cstddef>

namespace IO {

class DummyWriter {
public:
	void write(char c) {
	}

	void write(const char *c, int n) {
	}
};

template<typename T_Writer = DummyWriter>
class Print {
private:
	T_Writer w;

	inline void write(char c) {
		w.write(c);
	}

	inline void write(const char *str) {
		w.write(str);
	}

	inline void write(const void *data, int size) {
		w.wite(data, size);
	}
public:
	enum Radix_t {
		BYTE = 0, BIN = 2, OCT = 8, DEC = 10, HEX = 16
	};

	inline void print(char c) {
		write(c);
	}

	inline void print(const char str[]) {
		write(str);
	}

	void print(uint8_t value, Radix_t radix = DEC);
	void print(int value, Radix_t radix = DEC);
	void print(unsigned int value, Radix_t radix = DEC);
	void print(long value, Radix_t radix = DEC);
	void print(unsigned long value, Radix_t radix = DEC);
	void print(long long value, Radix_t radix = DEC);
	void print(unsigned long long value, Radix_t radix = DEC);
	void print(double value, int digits = 2);

	inline void println(void) {
		print("\r\n");
	}

	inline void println(char c) {
		print(c);
		println();
	}

	inline void println(const char str[]) {
		print(str);
		println();
	}

	inline void println(uint8_t value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(int value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(unsigned int value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(long value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(unsigned long value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(long long value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(unsigned long long value, Radix_t radix = DEC) {
		print(value, radix);
		println();
	}

	inline void println(double value, int digits = 2) {
		print(value, digits);
		println();
	}

private:
	void printNumber(unsigned long long, uint8_t);
	void printFloat(double, uint8_t);
};

} /* namespace IO */
#endif /* PRINT_H_ */
