#ifndef __332_STDIO_H
#define __332_STDIO_H

#include <stdarg.h>
#include <sys/types.h>
#include "libc_segments.h"

#ifndef NULL
/* #define NULL ((void *) 0) */
#define NULL 0
#endif

#define	BUFSIZ	1024

/* dummy file structure */
/* for convenience, for FILE *f; f->fileno == fileno */
typedef struct {
     int fileno;
} FILE;

/* FILE tmp_stdio[] = {{0},{1},{2}}; */

/* #define stdin (&tmp_stdio[0])   */
/* #define stdout (&tmp_stdio[1])  */
/* #define stderr (&tmp_stdio[2])  */

/* #define stdin ( (FILE *) 0 ) */
#define stdin NULL
#define stdout ( (FILE *) 1 )
#define stderr ( (FILE *) 2 )


#ifndef EOF
#define EOF     (-1)
#endif

extern int getchar () SEG_LIBC;
extern char *gets (char *buf) SEG_LIBC;
extern int puts (unsigned char *string) SEG_LIBC;
extern int putchar (unsigned int outch) SEG_LIBC;

extern int vsprintf(char *buf, const char *fmt, va_list args) SEG_LIBC;
extern int sprintf(char * buf, const char *fmt, ...) SEG_LIBC;
extern int printf(const char *fmt, ...);

extern int fprintf(FILE *, const char *fmt, ...);
extern int vfprintf(FILE *, const char *fmt, va_list args) SEG_LIBC;

#define fflush(A) (0); /* always assume success */

extern int sscanf(const char *, const char *, ...) SEG_LIBC;
extern FILE *fopen(const char *, const char *) SEG_LIBC;
extern int fclose(FILE *stream) SEG_LIBC;/* Python uses fclose as a function pointer - can't #define instead*/


extern size_t fwrite(const void *ptr, size_t size, size_t nitems,
	      FILE *stream) SEG_LIBC;

#define putc( A, B ) ( putchar(A) )
#define setbuf(A, B) (0)

/* since we don't expect stream input for now, define fgets as such */
#define fgets(s, n, stream) ('\0')
#define fputc(A, stream) (putchar(A))

/* always say that an EOF condition has been been reached */
#define feof(stream) (1)

/* always fail on an fseek */
#define fseek(a,b,c) (-1)

#define ftell(A) (-1)

/* indicate where fwrite is attempting to write a string */
/* #define fwrite(data, size, number, stream) printf("fwrite a string\n") */
#define fread(data, size, number, stream) (0)
#define ferror(stream) (0)
#define fgetc(stream) (1)
#define getc(stream) (1)
#define fputs(s, stream) puts(s)
#define clearerr(stream) 

#define isatty(fildes) (0)

/* fileno probably should return the correct file number for stdin,
   stdout, stderr ---> assume stdout for now */
/* #define fileno(stream) (1) */
extern int fileno(FILE *stream) SEG_LIBC;


/* the following really belongs in stdlib.h, but since we are using the
   system version, we'll define it here.  Another note:  this should
   be turned into a function returning the appropriate environment
   variable value via string comparison */
#define getenv(env) '\0'
/* abort should probably shut down the system somehow - need to
   add PalmOS hook */
#define abort() printf("ABORT, ABORT...\n\n");
#define exit(status)printf("EXIT, EXIT...\n\n");

/* belongs in unistd.h, but it's not actually included in the sources - 
   hence we define unlink (which we don't actually use) here to fail */
#define unlink(path) (-1)


#endif /* __332_STDIO_H */
