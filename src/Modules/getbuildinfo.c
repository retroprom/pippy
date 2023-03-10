#include "Python.h"

#ifdef macintosh
#include "macbuildno.h"
#endif

#ifndef DONT_HAVE_STDIO_H
#include <stdio.h>
#endif

#ifndef DATE
#ifdef __DATE__
#define DATE __DATE__
#else
#define DATE "xx/xx/xx"
#endif
#endif

#ifndef TIME
#ifdef __TIME__
#define TIME __TIME__
#else
#define TIME "xx:xx:xx"
#endif
#endif

#ifndef BUILD
#define BUILD 0
#endif

#ifndef SMALL_PLATFORM
const char *
Py_GetBuildInfo()
{
	static char buildinfo[50];
	sprintf(buildinfo, "#%d, %.20s, %.9s", BUILD, DATE, TIME);
	return buildinfo;
}

#else
const char *
Py_GetBuildInfo()
{
	return "Small Platform";
}
#endif /* SMALL_PLATFORM */
