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

struct args_t {
	int argc;
	const char ** const argv;

	args_t(const char ** const v) :
			argc(0), argv(v) {
	}

	inline const int count() const {
		return argc;
	}

	inline const char *operator[](int n) const {
		return argv[n];
	}
};

template<int buflen, int maxargs, int getch(char *c), int putch(char c)>
class LineReader {
public:
	LineReader() :
			idx(0), ptr(buffer), m_args(buf_argv) {
	}

	inline void reset() {
		idx = 0;
		ptr = buffer;
		m_args.argc = 0;
	}

	bool read() {
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
				reset();
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

	int parse() {
		char *ptr = buffer;
		while (*ptr) {
			char *token = skipblanks(ptr);
			if (*token != '\0') {
				m_args.argv[m_args.argc++] = token;
				ptr = skiptoken(token);
				if (*ptr)
					*ptr++ = '\0';
			} else
				break;
		}
		return argc();
	}

	inline const args_t& args() const {
		return m_args;
	}

	inline const char * const * const argv() const {
		return m_args.argv;
	}

	inline const int argc() const {
		return m_args.argc;
	}

	inline const char *text() const {
		return buffer;
	}

private:
	int idx;
	char *ptr;
	args_t m_args;

	char buffer[buflen];
	const char *buf_argv[maxargs];

	static inline char *skipblanks(char *ptr) {
		while (*ptr && std::isspace(*ptr))
			ptr++;
		return ptr;
	}

	static inline char *skiptoken(char *ptr) {
		while (*ptr && !std::isspace(*ptr))
			ptr++;
		return ptr;
	}
};

} /* namespace Stream */
#endif /* LINEREADER_H_ */
