#ifndef _KLUDGE_H_
#define _KLUDGE_H_

/* Include this file before Python.h for extension modules requiring use of
   the PalmOS network library.  The PalmOS headers for this purpose are poorly
   designed and fixing them is a low priority of Palm's. */


#define __string_h  /* causes unix_string.h be ignored */
#include <PalmOS.h>
#include <sys/types.h>
#include <Core/System/MemoryMgr.h>
#include <sys_time.h>
#include <netdb.h>
#include <netinet_in.h>
#include <unix_fcntl.h>

/* trying a new methodology for including the crappy PalmOS includes */
#define __UNIX_SYS_TYPES_H__
typedef	NetFDSetType	fd_set;
#define	FD_SETSIZE		netFDSetSize

#define	FD_SET(n,p)	netFDSet(n,p)
#define	FD_CLR(n,p)	netFDClr(n,p)
#define	FD_ISSET(n,p)	netFDIsSet(n,p)
#define	FD_ZERO(p)      netFDZero(p)
#define	bcopy(b1,b2,len)MemMove(b2,b1,len)

#define _STDLIB
#ifndef CODEWARRIOR
#define __stdarg_h  /* causes unix_stdarg.h be effectively ignored */
#endif
#define __stdio_h  /* causes unix_stdio.h to be ignored */
#include <sys_socket.h>

#undef isatty
#undef abort
#undef getenv

#include "Python.h"
#endif /*_KLUDGE_H_*/
