/*
 * LineReader.h
 *
 *  Created on: 27/10/2012
 *      Author: PC 2010
 */

#ifndef LINEREADER_H_
#define LINEREADER_H_

#include <cctype>

namespace Stream {

/**
 * @brief Template of function to get character from input
 * @param[out] cptr Pointer to character variable for store the reader char (can be null)
 * @return Zero if character is read, non zero if not have character
 */
typedef int (LineReader_getch)(char *cptr);
/**
 * @brief Template of function to put character to output
 * @param[in] c Character to put
 * @return Zero if character can be putted or non zero if not allow space on output
 */
typedef int (LineReader_putch)(char c);

/**
 * @brief Line input reader and echo controller
 *
 * Component with same functionality as "readline" and similar
 * line-oriented editor inputs.
 *
 * @tparam getch Function to read character from input
 * @tparam putch Function to put character to output
 * @tparam buflen Maximum line buffer length
 * @tparam maxargs Maximum arguments can be parsed
 * @see LineReader_getch
 * @see LineReader_putch
 */
template<LineReader_getch getch, LineReader_putch putch, int buflen = 128,
		int maxargs = 10>
class LineReader {
private:
	int idx;
	char buffer[buflen];

public:
	LineReader() :
			idx(0) //
	{
	}

	void reset() {
		idx = 0;
	}

	/**
	 * @brief Poll the line editing process.
	 *
	 * This process poll for line edition and perform character detection and
	 * echo handling to output (via template parameters getch and putch)
	 *
	 * When line editing is end (#read return TRUE), the buffer contain the
	 * zero-ended string getted form input and you can call #parse to
	 * obtain argc/argv functions
	 *
	 * @return False if line editing is in progress, true if line end is detected
	 * @see parse
	 */
	bool poll() {
		char c;
		if (getch(&c) != -1) {
			switch (c) {
			case static_cast<char>(127):
			case '\b':
				if (idx > 0) {
					putch(c);
					--idx;
				}
				break;
			case '\r':
			case '\n':
				putch('\r');
				putch('\n');
				return true;
			default:
				if (idx < buflen)
					putch(buffer[idx++] = c);
				break;
			}
		}
		buffer[idx] = 0;
		return false;
	}

	/**
	 * @brief Return current text line readed
	 * @return Pointer to text line
	 */
	inline char *text() {
		return buffer;
	}
};

} /* namespace Stream */
#endif /* LINEREADER_H_ */
