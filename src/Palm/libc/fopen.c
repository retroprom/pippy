/*
  fopen replacement for palmos

  Jeffery D. Collins

*/

#include <stdio.h>
#include <string.h>

FILE *fopen(const char *name, const char *mode) {

     /* dummy replacement for fopen */
     if ( strcmp(name, "stdin") ) 
	  return stdin;
     else if (strcmp(name, "stdout") )
	  return stdout;
     else if (strcmp(name, "stderr") )
	  return stderr;
     else
	  return NULL;
}

int fileno(FILE *name) {
     /* dummy replacement for fileno */

     if ( name == stdin )
	  return 0;
     else if (name == stdout)
	  return 1;
     else if (name == stderr )
	  return 2;
     else
	  return 0;
}
