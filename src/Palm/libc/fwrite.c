/* #include <Common.h> */
/* #include <System/SysAll.h> */
/* #include <UI/UIAll.h> */

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>

/* write a stream as a string of characters - ignore stream */
size_t fwrite(const void *ptr, size_t size, size_t nitems,
	      FILE *stream) {
	
	char *c = (char *)ptr;
	size_t nbytes = size*nitems;
	int j;

	for (j=0; j < nbytes; j++) {
		putchar(*c);
		c++;
	}
	return nitems;
}

