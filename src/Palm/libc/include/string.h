#ifndef _LINUX_STRING_H_
#define _LINUX_STRING_H_

#include <sys/types.h>	/* for size_t */
#include "libc_segments.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* #define strerror(N) ( NULL ) */

extern char * ___strtok;
extern char * strcpy(char *,const char *) SEG_LIBC;
extern char * strncpy(char *,const char *,size_t) SEG_LIBC;
extern char * strcat(char *, const char *) SEG_LIBC;
extern char * strncat(char *, const char *, size_t) SEG_LIBC;
extern char * strchr(const char *,int) SEG_LIBC;
extern char * strrchr(const char *,int) SEG_LIBC;
extern char * strpbrk(const char *,const char *) SEG_LIBC;
extern char * strtok(char *,const char *) SEG_LIBC;
extern char * strstr(const char *,const char *) SEG_LIBC;
extern size_t strlen(const char *) SEG_LIBC;
extern size_t strnlen(const char *,size_t) SEG_LIBC;
extern size_t strspn(const char *,const char *) SEG_LIBC;
extern int strcmp(const char *,const char *) SEG_LIBC;
extern int strncmp(const char *,const char *,size_t) SEG_LIBC;
extern unsigned long strtoul(const char *cp,char **endp,unsigned int base) SEG_LIBC;

extern signed long strtol(const char *cp,char **endp,unsigned int base) SEG_LIBC;
extern int intodec(char * dest,signed int arg,unsigned short places,unsigned int base) SEG_LIBC;

extern void * memset(void *,char,size_t) SEG_LIBC;
extern void * memcpy(void *,const void *,size_t) SEG_LIBC;
extern void * memmove(void *,const void *,size_t) SEG_LIBC;
extern void * memscan(void *,int,size_t) SEG_LIBC;
extern int memcmp(const void *,const void *,size_t) SEG_LIBC;
#define memchr(A, B, C) (NULL)

/*
 * Include machine specific inline routines
 */

#ifdef __cplusplus
}
#endif

#endif /* _LINUX_STRING_H_ */
