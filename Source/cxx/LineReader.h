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
 * @brief Line reader argument result
 * Argument bundled with count and vector of parsed arguments
 * @see LineReader
 */
struct args_t {
	int argc; /**< @brief Argument counter */
	const char ** const argv; /**< @brief Vector of arguments */

	/**
	 * @brief Create an argument bundled
	 * @param v Pointer of preallocated argument vector
	 */
	args_t(const char ** const v) :
			argc(0), argv(v) {
	}

	/**
	 * @brief Count of arguments in vector
	 * @return Current argc value
	 */
	inline const int count() const {
		return argc;
	}

	/**
	 * @brief Access an element in the vector of arguments
	 * @warning This function not check index bound
	 * @param n Number of argument to access
	 * @return Pointer to text argument
	 */
	inline const char *operator[](int n) const {
		return argv[n];
	}

	/**
	 * @brief Verify if the argument block is null
	 * @return True if argument block not contains any arguments
	 */
	inline bool isNull() const {
		return argc == 0;
	}
};

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
public:
	LineReader() :
			idx(0), ptr(buffer), m_args(buf_argv) {
	}

	/**
	 * @brief Restart the parser and clear the buffer
	 */
	inline void reset() {
		ptr = buffer;
		m_args.argc = 0;
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

	/**
	 * @brief Parse readed line and build argc/argv
	 *
	 * Perform a string parsing over a previous readed line
	 * with #read function and build argc/argv pairs
	 *
	 * @return Numbers of arguments founds (argc)
	 * @see argv
	 * @see argc
	 * @see args
	 */
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

	/**
	 * @brief Return current arguments (counter and array of strings)
	 * @return Parsed arguments
	 */
	inline const args_t& args() const {
		return m_args;
	}

	/**
	 * @brief Return current argument strings vector
	 * @return Pointer to argument vector
	 */
	inline const char * const * const argv() const {
		return m_args.argv;
	}

	/**
	 * @brief Return current argument counter
	 * @return Number of arguments parsed
	 */
	inline const int argc() const {
		return m_args.argc;
	}

	/**
	 * @brief Return current text line readed
	 * @return Pointer to text line
	 */
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
