/*
  This file contains some utilities related to PalmOS networking.
*/

#include "PalmOS.h"
#include "netutil.h"

extern Int32 AppNetTimeout; /* defined in Netsocket.c */

static int
netutil_millisecondsPerTick()
{
	/*  (1000 ms/s) / (n ticks/s) = (1000/n) ms/tick */
	return 1000 / SysTicksPerSecond();
}

void 
netutil_settimeout(long timeout)  /* Timeout in milliseconds */
{
	/* If SysTicksPerSecond() > 1000, then we need to change
	   the representation to from milliseconds to microseconds. */

	if (timeout < 0)
		AppNetTimeout = -1; /* wait forever */
	else
		AppNetTimeout = timeout / netutil_millisecondsPerTick();
}

Int32
netutil_gettimeout() /* timeout in milliseconds */
{
	if (AppNetTimeout == -1)
		return -1;
	return AppNetTimeout * netutil_millisecondsPerTick();
}
	
