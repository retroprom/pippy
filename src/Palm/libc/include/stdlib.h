#ifndef _332_STDLIB_H_
#define _332_STDLIB_H_

#include <sys/types.h>
#include "libc_segments.h"

#ifndef NULL
#define NULL ((void *) 0)
#endif

extern void *malloc (size_t size);
extern void free (void *ptr);
extern void *realloc( void *, size_t) SEG_LIBC;
extern void *calloc( size_t, size_t ) SEG_LIBC;
/* extern void *realloc( void *, size_t); */
/* extern void *calloc( size_t, size_t ); */

/* extern void get_allocations(long *, long *) SEG_LIBC; */
/* extern size_t malloc_usable_size(void* mem) SEG_LIBC; */
extern void get_allocations(long *, long *);
extern void get_dlallocations(long *, long *) SEG_LIBC;
extern size_t malloc_usable_size(void* mem) SEG_LIBC;
extern void malloc_stats(void) SEG_LIBC;

/* need to move this macro into the config file */
#undef DEBUG_MALLOC

extern void dump_to_host(char *which, void *address, long value) SEG_LIBC;
#ifdef DEBUG_MALLOC
#define DUMP_TO_HOST(a,b,c) dump_to_host(a,b,c);
#else
#define DUMP_TO_HOST(a,b,c);
#endif

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
