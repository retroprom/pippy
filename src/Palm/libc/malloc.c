#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
#include "libc_segments.h"

long malloc_ref_count = 0;
void *malloc( size_t size ) {
	void *mem = MemPtrNew(size);
	if (mem!=NULL) malloc_ref_count++;
	return mem;
}

long get_malloc_ref_count() SEG_LIBC;
long get_malloc_ref_count() {
	/* for debugging */
	return malloc_ref_count;
}
