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

template<typename Writer>
class OutStream {
private:
	Writer out;

protected:
	void write(char c) {
		out.write(c);
	}

	void write(const char *ptr, int size) {
		out.write(ptr, size);
	}

	void write(const char *ptr) {
		out.write(ptr);
	}

public:
	OutStream() = delete;

	OutStream& operator<<(const char *str) {
		write(str);
		return *this;
	}

	OutStream& operator<<(char c) {
		write(c);
		return *this;
	}

	OutStream& operator<<(int n) {
		return *this;
	}
};


} /* namespace Stream */
#endif /* STREAM_H_ */
