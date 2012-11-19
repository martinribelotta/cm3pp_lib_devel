/*
 * CmdLineParser.cpp
 *
 *  Created on: 18/11/2012
 *      Author: PC 2010
 */

#include "CmdLineParser.h"
#include <cctype>

namespace Stream {

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

int do_parse(char *buffer, const char *argv[]) {
	int argc = 0;
	char *ptr = buffer;
	while (*ptr) {
		char *token = skipblanks(ptr);
		if (*token != '\0') {
			argv[argc++] = token;
			ptr = skiptoken(token);
			if (*ptr)
				*ptr++ = '\0';
		} else
			break;
	}
	return argc;
}

} /* namespace Stream */
