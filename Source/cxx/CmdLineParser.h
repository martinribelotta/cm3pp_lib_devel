/*
 * CmdLineParser.h
 *
 *  Created on: 18/11/2012
 *      Author: PC 2010
 */

#ifndef CMDLINEPARSER_H_
#define CMDLINEPARSER_H_

namespace Stream {

extern int do_parse(char *buffer, const char *argv[]);

template<int maxargs = 10>
class CmdLineParser {
	const char *m_argv[maxargs];
	const int len;
public:
	CmdLineParser(char *buffer) :
			len(do_parse(buffer, m_argv)) //
	{
	}

	int argc() const {
		return len;
	}

	const char **argv() {
		return m_argv;
	}

	const char *operator[](int n) const {
		return m_argv[n];
	}
};

} /* namespace Stream */

#endif /* CMDLINEPARSER_H_ */
