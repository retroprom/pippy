
#include "io.h"
#include <stdio.h>
#include "libc_segments.h"

extern void *malloc(long bytes);

#define BUF_LEN 100
static char put_buf[BUF_LEN]; /* roughly one line */
static UInt16 buf_pos = 0;

static int flush_buf() {
	if (buf_pos > 0) {
		put_buf[buf_pos]='\0';
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
	
	put_buf[buf_pos++] = x;

	/* flush on \n character or when BUF_LEN-1 (keep last \0 for
	   end-of-string indication) is reached */
	if (x == '\n' || buf_pos == BUF_LEN-1) {
		flush_buf();
	}

	return 0;
}

int printf(const char *fmt, ...)
{
	va_list args;
	int i;
	
	va_start(args, fmt);
	i = vfprintf(stdout, fmt, args);
	va_end(args);
	
	return i;
}

int fprintf(FILE* f, const char *fmt, ...)
{
	va_list args;
	int i;
	
	va_start(args, fmt);
	i = vfprintf(f, fmt, args);
	va_end(args);
	
	return i;
}

int vfprintf(FILE *stream, const char *fmt, va_list ap)
{
	int i,j;
	char *buf = (char *)malloc(500L);

	i=vsprintf(buf,fmt,ap);

	/*	flush_buf();
		ioPutS(buf);*/
	for (j=0; j<i;j++)
 	  putchar(buf[j]);
 	free(buf);

	return i;
}
