#ifndef STREAM_H_
#define STREAM_H_

#include <cstdint>
#include <cstddef>

/**
 * @brief Provide a way to send data over a serial channel
 */
namespace Stream {

/**
 * @brief Base class of serial stream
 */
class AbstractWriteStream {
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
		BIN = 2, OCT = 8, DEC = 10, HEX = 16
	};

	class FillChar {
	public:
		const char fill;

		FillChar(char c) :
				fill(c) {
		}
	};

	class Width {
	public:
		const unsigned int width;
		Width(unsigned int n) :
				width(n) {
		}
	};

	class Config {
	public:
		const int m_width;
		const char m_fill;
		const Radix_t m_radix;
		Config(const int width, const char fill, const Radix_t radix) :
			m_width(width), m_fill(fill), m_radix(radix) {
		}
	};

	AbstractWriteStream() :
			m_width(0), m_fill(' '), m_radix(DEC) {
	}

	virtual ~AbstractWriteStream() {
	}

	inline AbstractWriteStream& operator<<(const char *str) {
		write(str);
		return *this;
	}

	inline AbstractWriteStream& operator<<(char c) {
		write(c);
		return *this;
	}

	inline AbstractWriteStream& operator<<(int n) {
		print(n);
		return *this;
	}

	inline AbstractWriteStream& operator<<(unsigned int n) {
		print(n);
		return *this;
	}

	inline AbstractWriteStream& operator<<(Radix_t radix) {
		m_radix = radix;
		return *this;
	}

	inline AbstractWriteStream& operator<<(FillChar c) {
		m_fill = c.fill;
		return *this;
	}

	inline AbstractWriteStream& operator<<(Width w) {
		m_width = w.width;
		return *this;
	}

	inline AbstractWriteStream& operator<<(bool b) {
		return *this << (b? "true" : "false");
	}

	inline AbstractWriteStream& operator<<(Config c) {
		m_width = c.m_width;
		m_fill = c.m_fill;
		m_radix = c.m_radix;
		return *this;
	}

private:
	void print(int value);
	void print(unsigned int value, unsigned int deep = 0);

	unsigned int m_width;
	char m_fill;
	Radix_t m_radix;
};

} /* namespace Stream */
#endif /* STREAM_H_ */
