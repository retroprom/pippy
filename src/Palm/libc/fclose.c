/*
  fclose replacement for palmos

  Jeffery D. Collins

*/


#include <stdio.h>



int fclose(FILE *stream) {
	printf("in fclose\n");
     /* dummy replacement for fclose */
  return 0;

}
