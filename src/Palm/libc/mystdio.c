
#include "io.h"
#include <stdio.h>
#include "libc_segments.h"

#define BUF_LEN 33
static char put_buf[BUF_LEN]; /* roughly one line */
static UInt16 buf_pos = 0;

static int flush_buf() {
	if (buf_pos > 0) {
		ioPutS(put_buf);
		buf_pos = 0;
	}
	return 0;
}
int puts(unsigned char *s) {
	flush_buf();
	ioPutS(s);
	return 0;
}

int putchar (unsigned int x) {
	if (buf_pos == 0)
		MemSet (put_buf, BUF_LEN, '\0');
	
	put_buf[buf_pos++] = x;

	/* flush on \n character or when BUF_LEN-1 (keep last \0 for
	   end-of-string indication) is reached */
	if (x == '\n' || buf_pos == BUF_LEN-1) {
		ioPutS(put_buf);
		buf_pos = 0;
	}

	return 0;
}
static char buf[500];
int printf(const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);

	flush_buf();
	ioPutS(buf);

	return i;
}

int fprintf(FILE* f, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	
	flush_buf();
	ioPutS(buf);

	return i;
}

int vfprintf(FILE *stream, const char *fmt, va_list ap)
{
	int i;

	i=vsprintf(buf,fmt,ap);

	flush_buf();
	ioPutS(buf);

	return i;
}
