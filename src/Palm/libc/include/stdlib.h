#ifndef _332_STDLIB_H_
#define _332_STDLIB_H_

#include <sys/types.h>
#include "libc_segments.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern void *malloc (size_t size) SEG_LIBC;
extern void free (void *ptr) SEG_LIBC;
extern void *realloc( void *, size_t) SEG_LIBC;
extern void *calloc( size_t, size_t ) SEG_LIBC;

#ifdef __KERNEL__

void *kmalloc(int size, void *tcb) SEG_LIBC;
int kfree(void *p) SEG_LIBC;

struct mblock {
  struct mblock *next;
  struct mblock *prev;
  int size;
};

#endif /* __KERNEL__ */
#endif /* _332_STDLIB_H_ */
