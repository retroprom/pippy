#include "config.h"
#include <stdlib.h>
#include <PalmOS.h>
#include <PalmCompatibility.h>
void *  MemCalloc(UInt32 nmemb, UInt32 size);
#ifndef USE_DLMALLOC
void *calloc( size_t nelem, size_t size ) {

	return MemCalloc(nelem, size);

}
#endif
