/*
 * Stream.h
 *
 *  Created on: 23/09/2012
 *      Author: ruso
 */

#ifndef STREAM_H_
#define STREAM_H_

#include <cstdint>
#include <cstddef>

namespace OwnStream {

class AbstractStream {
protected:
	virtual void write(char c) = 0;

	virtual void write(const char *ptr, int size) {
		while (size--)
			write(*ptr++);
	}

	virtual void write(const char *ptr) {
		while (*ptr)
			write(*ptr++);
	}

public:
	enum Radix_t {
		BIN = 2,
		OCT = 8,
		DEC = 10,
		HEX = 16
	};

	AbstractStream() : m_radix(DEC) {
	}

	virtual ~AbstractStream() {
	}

	AbstractStream& operator<<(const char *str) {
		write(str);
		return *this;
	}

	AbstractStream& operator<<(char c) {
		write(c);
		return *this;
	}

	AbstractStream& operator<<(int n) {
		print(n);
		return *this;
	}

	AbstractStream& operator<<(Radix_t radix) {
		m_radix = radix;
		return *this;
	}

private:
	void print(int value);
	void print(unsigned int value);

	Radix_t m_radix;
};

} /* namespace Stream */
#endif /* STREAM_H_ */
