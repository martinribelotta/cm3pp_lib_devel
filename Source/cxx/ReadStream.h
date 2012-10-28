/*
 * AbstractReadStream.h
 *
 *  Created on: 26/10/2012
 *      Author: PC 2010
 */

#ifndef ABSTRACTREADSTREAM_H_
#define ABSTRACTREADSTREAM_H_

namespace Stream {

class AbstractReadStream {
protected:
	virtual bool read(char *c) = 0;

	virtual int read(char *ptr, int size) {
		for (int counter = 0; counter < size; counter++)
			if (!read(ptr++))
				return counter;
		return size;
	}

public:
	enum Status {
		OK, EOF
	};

	AbstractReadStream() :
			m_currentStatus(OK) {
	}

	virtual ~AbstractReadStream() {
	}

	inline AbstractReadStream& operator>>(char& c) {
		if (!read(&c))
			m_currentStatus = EOF;
		return *this;
	}

	inline Status currentStatus() const {
		return m_currentStatus;
	}

	inline bool haveData() const {
		return currentStatus() != EOF;
	}
private:
	Status m_currentStatus;
};

} /* namespace Stream */
#endif /* ABSTRACTREADSTREAM_H_ */
