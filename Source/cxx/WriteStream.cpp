/*
 * Stream.cpp
 *
 *  Created on: 23/09/2012
 *      Author: ruso
 */

#include "WriteStream.h"

namespace Stream {

static const char DIGIT_TABLE[] = "0123456789ABCDEF";

void AbstractWriteStream::print(unsigned int value, unsigned int deep) {
	if (value >= m_radix)
		print(value / m_radix, deep + 1);
	else
		while (++deep < m_width)
			write(m_fill);
	write(DIGIT_TABLE[(value % m_radix)]);
}

void AbstractWriteStream::print(int value) {
	if (value < 0) {
		write('-');
		value = -value;
	}
	print(static_cast<unsigned int>(value));
}

} /* namespace Stream */
