#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
void *  MemRealloc(void * p, UInt32 size);

void *realloc( void *ptr, size_t size ) {

	return MemRealloc(ptr, size);

}
