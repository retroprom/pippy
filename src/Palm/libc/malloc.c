#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
#include "libc_segments.h"

#include <stdio.h>


long malloc_ref_count = 0;
long bytes_allocated = 0;
long bytes_deallocated = 0;

static HostFILE *hostfile = NULL;

void dump_to_host(char *which, void *address, long value)
{
	if (hostfile == NULL) {
		hostfile = HostFOpen("malloc_dump.log", "w");
		if (hostfile == NULL){
			printf("problem opening log file\n");
			return;
		}
	}
	HostFPrintF(hostfile, "('%s', (%p, %ld))\n", which, address, value);
       	HostFFlush (hostfile);
}


void *malloc( size_t size ) {
	void *mem = MemPtrNew(size);

	if (mem == NULL)
	     return NULL;

	bytes_allocated += size;
	malloc_ref_count++;
	DUMP_TO_HOST("a", mem, size);
	
	return mem;
}

long get_malloc_ref_count() SEG_LIBC;
long get_malloc_ref_count() {
	/* for debugging */
	return malloc_ref_count;
}

void get_allocations(long *allocated, long *deallocated)
{

	if (allocated != NULL)
		*allocated = bytes_allocated;
	if (deallocated != NULL)
		*deallocated = bytes_deallocated;
}
