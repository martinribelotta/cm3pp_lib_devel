/*
 * Stream.cpp
 *
 *  Created on: 23/09/2012
 *      Author: ruso
 */

#include "Stream.h"

namespace OwnStream {

static const char DIGIT_TABLE[] = "0123456789ABCDEF";

void AbstractStream::print(unsigned int value, unsigned int deep) {
	if (value >= m_radix)
		print(value / m_radix, deep + 1);
	else
		while (++deep < width)
			write(fill_char);
	write(DIGIT_TABLE[(value % m_radix)]);
}

void AbstractStream::print(int value) {
	if (value < 0) {
		write('-');
		value = -value;
	}
	print(static_cast<unsigned int>(value));
}

} /* namespace Stream */
