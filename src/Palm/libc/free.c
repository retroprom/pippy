#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
#include <stdio.h>

extern long malloc_ref_count;
extern long bytes_deallocated;

void free( void *ptr ){
     size_t size = MemPtrSize(ptr);

     Err res = MemPtrFree(ptr);
     if (res) 
	  printf("free: error freeing memory = %d\n", res);
     
     malloc_ref_count--;
     bytes_deallocated += size;

     DUMP_TO_HOST("d", ptr, size);
	
}
