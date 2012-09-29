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
class AbstractStream {
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

	AbstractStream() :
			width(0), fill_char(' '), m_radix(DEC) {
	}

	virtual ~AbstractStream() {
	}

	AbstractStream& operator<<(const char *str) {
		write(str);
		return *this;
	}

	AbstractStream& operator<<(char c) {
		write(c);
		return *this;
	}

	AbstractStream& operator<<(int n) {
		print(n);
		return *this;
	}

	AbstractStream& operator<<(unsigned int n) {
		print(n);
		return *this;
	}

	AbstractStream& operator<<(Radix_t radix) {
		m_radix = radix;
		return *this;
	}

	AbstractStream& operator<<(FillChar c) {
		fill_char = c.fill;
		return *this;
	}

	AbstractStream& operator<<(Width w) {
		width = w.width;
		return *this;
	}

	AbstractStream& operator<<(bool b) {
		return *this << (b? "true" : "false");
	}

private:
	void print(int value);
	void print(unsigned int value, unsigned int deep = 0);

	unsigned int width;
	char fill_char;
	Radix_t m_radix;
};

} /* namespace Stream */
#endif /* STREAM_H_ */
