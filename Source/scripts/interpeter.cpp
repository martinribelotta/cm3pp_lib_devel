#include <usbd_cdc_vcp.h>

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

void interpreter_putln(const char *s) {
	interpreter_puts(s);
	interpreter_puts("\r\n");
}

struct entry_t {
	const char *name;
	int (*func)(int, const char **);
};

extern "C" int tinybasic_interpreter(int argc, const char* argv[]);

int cmd_help(int argc, const char* argv[]) {
	(void) argc;
	(void) argv;
	interpreter_putln("FUAAAAAAAAAAAA!!!");
	return 0;
}

static entry_t entries[] = { //
		{ "basic", tinybasic_interpreter },
		{ "help", cmd_help }
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
