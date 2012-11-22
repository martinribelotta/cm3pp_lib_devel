#include <reent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/unistd.h>

int _gettimeofday_r(struct _reent *e, struct timeval *__tp, void *__tzp) {
	return -1;
}

int _close_r(struct _reent *ctx, int fd) {
	return -1;
}

int _execve_r(struct _reent *ctx, const char *filename, char * const *argv,
		char * const *envp) {
	errno = ENOMEM;
	return -1;
}

int _fcntl_r(struct _reent *ctx, int fd, int cmd, int param) {
	errno = EBADF;
	return -1;
}

int _fork_r(struct _reent *ctx) {
	errno = EAGAIN;
	return -1;
}

int _fstat_r(struct _reent *ctx, int fd, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _getpid_r(struct _reent *ctx) {
	return 1;
}

int _isatty_r(struct _reent *ctx, int fd) {
	switch (fd) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
	case STDIN_FILENO:
		return 1;
	default:
		//errno = ENOTTY;
		errno = EBADF;
		return 0;
	}
}

int _kill_r(struct _reent *ctx, int pid, int sig) {
	errno = EINVAL;
	return -1;
}

int _link_r(struct _reent *ctx, const char *oldname, const char *newname) {
	errno = EMLINK;
	return -1;
}

_off_t _lseek_r(struct _reent *ctx, int fd, _off_t offset, int dir) {
	return 0;
}

int _mkdir_r(struct _reent *ctx, const char *filename, int mode) {
	errno = ENOENT;
	return -1;
}

int _open_r(struct _reent *ctx, const char *filename, int mode, int extra) {
	return -1;
}

int _rename_r(struct _reent *ctx, const char *oldname, const char *newname) {
	errno = ENOENT;
	return -1;
}

void *_sbrk_r(struct _reent *ctx, ptrdiff_t incr) {
	extern int _end;
	static unsigned char *heap = NULL;
	unsigned char *prev_heap;

	if (heap == NULL )
		heap = (unsigned char *) &_end;
	prev_heap = heap;
	heap += incr;
	return (void*) prev_heap;
}

int _stat_r(struct _reent *ctx, const char *fname, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

unsigned long _times_r(struct _reent *ctx, struct tms *ms) {
	return -1;
}

int _unlink_r(struct _reent *ctx, const char *filename) {
	errno = ENOENT;
	return -1;
}

int _wait_r(struct _reent *ctx, int *status) {
	errno = ECHILD;
	return -1;
}

extern int usb_cdc_putc(char c);
extern int usb_cdc_getc(char *c);

_ssize_t _write_r(struct _reent *ctx, int fd, const void *data, size_t size) {
	_ssize_t counter;
	switch (fd) {
	case STDOUT_FILENO:
	case STDERR_FILENO:
		for (counter = 0; counter < size; counter++)
			if (usb_cdc_putc(*(char*)(data++)) == -1)
				return counter;
		return counter;
	default:
		errno = EBADF;
		return -1;
	}
}

_ssize_t _read_r(struct _reent *ctx, int fd, void *buf, size_t size) {
	_ssize_t counter;
	switch (fd) {
	case STDIN_FILENO:
		for (counter = 0; counter < size; counter++)
			if (usb_cdc_getc((char*)(buf++)) == -1)
				return counter;
		return counter;
	default:
		errno = EBADF;
		return -1;
	}
}
