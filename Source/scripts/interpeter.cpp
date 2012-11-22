#include <usbd_cdc_vcp.h>
#include <cstring>
#include <cxx/LineReader.h>

#include "builtins.h"

extern "C" char interpreter_getchar(void) {
	char c = -1;
	usb_cdc_getc(&c);
	return c;
}

extern "C" void interpreter_putchar(char c) {
	usb_cdc_putc(c);
}

void interpreter_puts(const char *s) {
	while (*s)
		interpreter_putchar(*s++);
}

extern Stream::LineReader<usb_cdc_getc, usb_cdc_putc> lineReader;

extern "C" int interpreter_readline(char *buf, size_t maxlen) {
	lineReader.reset();
	while(!lineReader.poll())
		;
	std::strncpy(buf, lineReader.text(), maxlen);
	return std::strlen(buf);
}

void interpreter_putln(const char *s) {
	interpreter_puts(s);
	interpreter_puts("\r\n");
}

struct entry_t {
	const char *name;
	int (*func)(int, const char **);
};

static entry_t entries[] = { //
		//
				{ "basic", tinybasic_interpreter }, //
				{ "tcl", picol_main }, //
				// { "jimtcl", jimtcl_main }, //
				{ "help", cmd_help } //
		};

template<typename T, int size>
int GetArrLength(T (&)[size]) {
	return size;
}

static bool cmp(const char *a, const char *b) {
	while (*a && *b)
		if (*a++ != *b++)
			return false;
	return *a == *b;
}

int execute(int argc, const char **argv) {
	if (argc > 0) {
		for (int i = 0; i < GetArrLength(entries); i++) {
			if (cmp(argv[0], entries[i].name))
				return entries[i].func(argc, argv);
		}
		interpreter_putln("Command not found");
	}
	return -1;
}
