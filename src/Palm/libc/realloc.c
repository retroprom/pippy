#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
void *  MemRealloc(void * p, UInt32 size);

extern long malloc_ref_count;
extern long bytes_allocated;

void *realloc( void *ptr, size_t size ) {

	size_t old_size = MemPtrSize(ptr);
	void *p;

	p = MemRealloc(ptr, size);
	if (p == NULL)
		return NULL;

	bytes_allocated += size;
	bytes_allocated -= old_size;

	DUMP_TO_HOST("r-", ptr, old_size);
	DUMP_TO_HOST("r+", p,   size);

	return p;
}
