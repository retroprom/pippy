#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
void *  MemCalloc(UInt32 nmemb, UInt32 size);

void *calloc( size_t nelem, size_t size ) {

	return MemCalloc(nelem, size);

}
