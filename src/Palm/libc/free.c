#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
#include <stdio.h>

extern long malloc_ref_count;
void free( void *ptr ){
	Err res = MemPtrFree(ptr);
	if (res) 
		printf("free: error freeing memory = %d\n", res);
	else
		malloc_ref_count--;
}
