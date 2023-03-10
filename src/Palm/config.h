/* config.h.  Generated automatically by configure.  */
/* config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define if on AIX 3.
   System headers sometimes define this.
   We just want to avoid a redefinition error message.  */
#ifndef _ALL_SOURCE
/* #undef _ALL_SOURCE */
#endif

/* Define if type char is unsigned and you are not using gcc.  */
#ifndef __CHAR_UNSIGNED__
/* #undef __CHAR_UNSIGNED__ */
#endif

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to `int' if <sys/types.h> doesn't define.  */
#define gid_t int

/* Define if your struct tm has tm_zone.  */
/* #undef HAVE_TM_ZONE */

/* Define if you don't have tm_zone but do have the external array
   tzname.  */
/* #undef HAVE_TZNAME */

/* Define if on MINIX.  */
/* #undef _MINIX */

/* Define to `int' if <sys/types.h> doesn't define.  */
#define mode_t int

/* Define to `long' if <sys/types.h> doesn't define.  */
#define off_t long

/* Define to `int' if <sys/types.h> doesn't define.  */
#define pid_t int

/* Define if the system does not provide POSIX.1 features except
   with this defined.  */
/* #undef _POSIX_1_SOURCE */

/* Define if you need to in order for stat and other things to work.  */
/* #undef _POSIX_SOURCE */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE int

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
/* #undef TIME_WITH_SYS_TIME */

/* Define if your <sys/time.h> declares struct tm.  */
#define TM_IN_SYS_TIME 1

/* Define to `int' if <sys/types.h> doesn't define.  */
#define uid_t int

/* Define if your <unistd.h> contains bad prototypes for exec*()
   (as it does on SGI IRIX 4.x) */
#define BAD_EXEC_PROTOTYPES 1

/* Define if your compiler botches static forward declarations
   (as it does on SCI ODT 3.0) */
/* #undef BAD_STATIC_FORWARD */

/* Define for AIX if your compiler is a genuine IBM xlC/xlC_r
   and you want support for AIX C++ shared extension modules. */
/* #undef AIX_GENUINE_CPLUSPLUS */

/* Define this if you have BeOS threads */
/* #undef BEOS_THREADS */

/* Define if you have the Mach cthreads package */
/* #undef C_THREADS */

/* Define to `long' if <time.h> doesn't define.  */
/* #undef clock_t */

/* Used for BeOS configuration */
/* #undef DL_EXPORT_HEADER */
#ifdef DL_EXPORT_HEADER
#include DL_EXPORT_HEADER
#endif

/* Define if getpgrp() must be called as getpgrp(0). */
/* #undef GETPGRP_HAVE_ARG */

/* Define if gettimeofday() does not have second (timezone) argument
   This is the case on Motorola V4 (R40V4.2) */
/* #undef GETTIMEOFDAY_NO_TZ */

/* Define this if your time.h defines altzone */
/* #undef HAVE_ALTZONE */

/* Define this if you have gethostbyname() */
/* #undef HAVE_GETHOSTBYNAME */

/* Define this if you have some version of gethostbyname_r() */
/* #undef HAVE_GETHOSTBYNAME_R */

/* Define this if you have the 3-arg version of gethostbyname_r() */
/* #undef HAVE_GETHOSTBYNAME_R_3_ARG */

/* Define this if you have the 5-arg version of gethostbyname_r() */
/* #undef HAVE_GETHOSTBYNAME_R_5_ARG */

/* Define this if you have the 6-arg version of gethostbyname_r() */
/* #undef HAVE_GETHOSTBYNAME_R_6_ARG */

/* Define this if you have the type long long */
/* #undef HAVE_LONG_LONG */

/* Define this if you have a K&R style C preprocessor */
/* #undef HAVE_OLD_CPP */

/* Define if your compiler supports function prototypes */
#define HAVE_PROTOTYPES 1

/* Define if your compiler supports variable length function prototypes
   (e.g. void fprintf(FILE *, char *, ...);) *and* <stdarg.h> */
#define HAVE_STDARG_PROTOTYPES 1

/* Define if malloc(0) returns a NULL pointer */
/* #undef MALLOC_ZERO_RETURNS_NULL */

/* Define if you have POSIX threads */
/* #undef _POSIX_THREADS */

/* Define to force use of thread-safe errno, h_errno, and other functions */
/* #undef _REENTRANT */

/* Define if setpgrp() must be called as setpgrp(0, 0). */
/* #undef SETPGRP_HAVE_ARG */

/* Define to empty if the keyword does not work.  */
/* #undef signed */

/* Define if  you can safely include both <sys/select.h> and <sys/time.h>
   (which you can't on SCO ODT 3.0). */
/* #undef SYS_SELECT_WITH_SYS_TIME */

/* Define if a va_list is an array of some kind */
/* #undef VA_LIST_IS_ARRAY */

/* Define to empty if the keyword does not work.  */
/* #undef volatile */

/* Define if you want SIGFPE handled (see Include/pyfpe.h). */
/* #undef WANT_SIGFPE_HANDLER */

/* Define if you want to use SGI (IRIX 4) dynamic linking.
   This requires the "dl" library by Jack Jansen,
   ftp://ftp.cwi.nl/pub/dynload/dl-1.6.tar.Z.
   Don't bother on IRIX 5, it already has dynamic linking using SunOS
   style shared libraries */ 
/* #undef WITH_SGI_DL */

/* Define if you want to emulate SGI (IRIX 4) dynamic linking.
   This is rumoured to work on VAX (Ultrix), Sun3 (SunOS 3.4),
   Sequent Symmetry (Dynix), and Atari ST.
   This requires the "dl-dld" library,
   ftp://ftp.cwi.nl/pub/dynload/dl-dld-1.1.tar.Z,
   as well as the "GNU dld" library,
   ftp://ftp.cwi.nl/pub/dynload/dld-3.2.3.tar.Z.
   Don't bother on SunOS 4 or 5, they already have dynamic linking using
   shared libraries */ 
/* #undef WITH_DL_DLD */

/* Define if you want to use the new-style (Openstep, Rhapsody, MacOS)
   dynamic linker (dyld) instead of the old-style (NextStep) dynamic
   linker (rld). Dyld is necessary to support frameworks. */
/* #undef WITH_DYLD */

/* Define if you want to compile in rudimentary thread support */
/* #undef WITH_THREAD */

/* Define if you want to produce an OpenStep/Rhapsody framework
   (shared library plus accessory files). */
/* #undef WITH_NEXT_FRAMEWORK */

/* The number of bytes in an off_t. */
/* #undef SIZEOF_OFF_T */

/* Defined to enable large file support when an off_t is bigger than a long
   and long long is available and at least as big as an off_t. You may need
   to add some flags for configuration and compilation to enable this mode.
   E.g, for Solaris 2.7:
   CFLAGS="-D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64" OPT="-O2 $CFLAGS" \
 configure
*/
/* #undef HAVE_LARGEFILE_SUPPORT */

/* Defined when any dynamic module loading is enabled */
/* #undef HAVE_DYNAMIC_LOADING */

/* The number of bytes in a int.  */
/* #undef SIZEOF_INT */

/* The number of bytes in a long.  */
/* #undef SIZEOF_LONG */

/* The number of bytes in a long long.  */
/* #undef SIZEOF_LONG_LONG */

/* The number of bytes in a void *.  */
/* #undef SIZEOF_VOID_P */

/* Define if you have the alarm function.  */
/* #undef HAVE_ALARM */

/* Define if you have the chown function.  */
/* #undef HAVE_CHOWN */

/* Define if you have the clock function.  */
/* #undef HAVE_CLOCK */

/* Define if you have the confstr function.  */
/* #undef HAVE_CONFSTR */

/* Define if you have the ctermid function.  */
/* #undef HAVE_CTERMID */

/* Define if you have the ctermid_r function.  */
/* #undef HAVE_CTERMID_R */

/* Define if you have the dlopen function.  */
/* #undef HAVE_DLOPEN */

/* Define if you have the dup2 function.  */
/* #undef HAVE_DUP2 */

/* Define if you have the execv function.  */
/* #undef HAVE_EXECV */

/* Define if you have the fdatasync function.  */
/* #undef HAVE_FDATASYNC */

/* Define if you have the flock function.  */
/* #undef HAVE_FLOCK */

/* Define if you have the fork function.  */
/* #undef HAVE_FORK */

/* Define if you have the fpathconf function.  */
/* #undef HAVE_FPATHCONF */

/* Define if you have the fseek64 function.  */
/* #undef HAVE_FSEEK64 */

/* Define if you have the fseeko function.  */
/* #undef HAVE_FSEEKO */

/* Define if you have the fstatvfs function.  */
/* #undef HAVE_FSTATVFS */

/* Define if you have the fsync function.  */
/* #undef HAVE_FSYNC */

/* Define if you have the ftell64 function.  */
/* #undef HAVE_FTELL64 */

/* Define if you have the ftello function.  */
/* #undef HAVE_FTELLO */

/* Define if you have the ftime function.  */
/* #undef HAVE_FTIME */

/* Define if you have the ftruncate function.  */
/* #undef HAVE_FTRUNCATE */

/* Define if you have the getcwd function.  */
/* #undef HAVE_GETCWD */

/* Define if you have the getgroups function.  */
/* #undef HAVE_GETGROUPS */

/* Define if you have the getlogin function.  */
/* #undef HAVE_GETLOGIN */

/* Define if you have the getpeername function.  */
#define HAVE_GETPEERNAME 1

/* Define if you have the getpgrp function.  */
/* #undef HAVE_GETPGRP */

/* Define if you have the getpid function.  */
/* #undef HAVE_GETPID */

/* Define if you have the getpwent function.  */
/* #undef HAVE_GETPWENT */

/* Define if you have the gettimeofday function.  */
/* #undef HAVE_GETTIMEOFDAY */

/* Define if you have the getwd function.  */
/* #undef HAVE_GETWD */

/* Define if you have the hypot function.  */
/* #undef HAVE_HYPOT */

/* Define if you have the kill function.  */
/* #undef HAVE_KILL */

/* Define if you have the link function.  */
/* #undef HAVE_LINK */

/* Define if you have the lstat function.  */
/* #undef HAVE_LSTAT */

/* Define if you have the memmove function.  */
/* #undef HAVE_MEMMOVE */

/* Define if you have the mkfifo function.  */
/* #undef HAVE_MKFIFO */

/* Define if you have the mktime function.  */
/* #undef HAVE_MKTIME */

/* Define if you have the nice function.  */
/* #undef HAVE_NICE */

/* Define if you have the pathconf function.  */
/* #undef HAVE_PATHCONF */

/* Define if you have the pause function.  */
/* #undef HAVE_PAUSE */

/* Define if you have the plock function.  */
/* #undef HAVE_PLOCK */

/* Define if you have the pthread_init function.  */
/* #undef HAVE_PTHREAD_INIT */

/* Define if you have the putenv function.  */
/* #undef HAVE_PUTENV */

/* Define if you have the readlink function.  */
/* #undef HAVE_READLINK */

/* Define if you have the select function.  */
/* #undef HAVE_SELECT */

/* Define if you have the setgid function.  */
/* #undef HAVE_SETGID */

/* Define if you have the setlocale function.  */
/* #undef HAVE_SETLOCALE */

/* Define if you have the setpgid function.  */
/* #undef HAVE_SETPGID */

/* Define if you have the setpgrp function.  */
/* #undef HAVE_SETPGRP */

/* Define if you have the setsid function.  */
/* #undef HAVE_SETSID */

/* Define if you have the setuid function.  */
/* #undef HAVE_SETUID */

/* Define if you have the setvbuf function.  */
/* #undef HAVE_SETVBUF */

/* Define if you have the sigaction function.  */
/* #undef HAVE_SIGACTION */

/* Define if you have the siginterrupt function.  */
/* #undef HAVE_SIGINTERRUPT */

/* Define if you have the sigrelse function.  */
/* #undef HAVE_SIGRELSE */

/* Define if you have the statvfs function.  */
/* #undef HAVE_STATVFS */

/* Define if you have the strdup function.  */
/* #undef HAVE_STRDUP */

/* Define if you have the strerror function.  */
/* #undef HAVE_STRERROR */

/* Define if you have the strftime function.  */
/* #undef HAVE_STRFTIME */

/* Define if you have the strptime function.  */
/* #undef HAVE_STRPTIME */

/* Define if you have the symlink function.  */
/* #undef HAVE_SYMLINK */

/* Define if you have the sysconf function.  */
/* #undef HAVE_SYSCONF */

/* Define if you have the tcgetpgrp function.  */
/* #undef HAVE_TCGETPGRP */

/* Define if you have the tcsetpgrp function.  */
/* #undef HAVE_TCSETPGRP */

/* Define if you have the tempnam function.  */
/* #undef HAVE_TEMPNAM */

/* Define if you have the timegm function.  */
/* #undef HAVE_TIMEGM */

/* Define if you have the times function.  */
/* #undef HAVE_TIMES */

/* Define if you have the tmpfile function.  */
/* #undef HAVE_TMPFILE */

/* Define if you have the tmpnam function.  */
/* #undef HAVE_TMPNAM */

/* Define if you have the tmpnam_r function.  */
/* #undef HAVE_TMPNAM_R */

/* Define if you have the truncate function.  */
/* #undef HAVE_TRUNCATE */

/* Define if you have the uname function.  */
/* #undef HAVE_UNAME */

/* Define if you have the waitpid function.  */
/* #undef HAVE_WAITPID */

/* Define if you have the <dirent.h> header file.  */
/* #undef HAVE_DIRENT_H */

/* Define if you have the <dlfcn.h> header file.  */
/* #undef HAVE_DLFCN_H */

/* Define if you have the <fcntl.h> header file.  */
/* #undef HAVE_FCNTL_H */

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <locale.h> header file.  */
/* #undef HAVE_LOCALE_H */

/* Define if you have the <ncurses.h> header file.  */
/* #undef HAVE_NCURSES_H */

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <pthread.h> header file.  */
/* #undef HAVE_PTHREAD_H */

/* Define if you have the <signal.h> header file.  */
/* #undef HAVE_SIGNAL_H */

/* Define if you have the <stdarg.h> header file.  */
#define HAVE_STDARG_H 1

/* Define if you have the <stddef.h> header file.  */
/* #undef HAVE_STDDEF_H */

/* Define if you have the <stdlib.h> header file.  */
#define HAVE_STDLIB_H 1

/* Define if you have the <sys/audioio.h> header file.  */
/* #undef HAVE_SYS_AUDIOIO_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/file.h> header file.  */
/* #undef HAVE_SYS_FILE_H */

/* Define if you have the <sys/lock.h> header file.  */
/* #undef HAVE_SYS_LOCK_H */

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
/* #undef HAVE_SYS_PARAM_H */

/* Define if you have the <sys/select.h> header file.  */
/* #undef HAVE_SYS_SELECT_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H

/* Define if you have the <sys/times.h> header file.  */
/* #undef HAVE_SYS_TIMES_H */

/* Define if you have the <sys/un.h> header file.  */
/* #undef HAVE_SYS_UN_H */

/* Define if you have the <sys/utsname.h> header file.  */
/* #undef HAVE_SYS_UTSNAME_H */

/* Define if you have the <sys/wait.h> header file.  */
/* #undef HAVE_SYS_WAIT_H */

/* Define if you have the <thread.h> header file.  */
/* #undef HAVE_THREAD_H */

/* Define if you have the <unistd.h> header file.  */
/* #undef HAVE_UNISTD_H */

/* Define if you have the <utime.h> header file.  */
/* #undef HAVE_UTIME_H */

/* Define if you have the dl library (-ldl).  */
/* #undef HAVE_LIBDL */

/* Define if you have the dld library (-ldld).  */
/* #undef HAVE_LIBDLD */

/* Define if you have the ieee library (-lieee).  */
/* #undef HAVE_LIBIEEE */


/*********************/
/* stuff jdc defines */
/*********************/

/* Remove complex objects */
#define WITHOUT_COMPLEX 1

/* Remove GETPATH */
#define WITHOUT_GETPATH 1

/* Remove float objects */
#define WITHOUT_FLOAT 1

/* Remove long objects */
#define WITHOUT_LONG 1

/* Dynamic linking */
#define NO_DYNAMIC_LINK 1

/* Python source compiler - for this, 
   1) set POBJS=  in Parser/Makefile
   2) remove graminit from Python/Makefile
*/
/* #define WITHOUT_COMPILER 1 */
/* #define WITHOUT_PARSER 1 */

/* Interrupts */
#define WITHOUT_INTERRUPTS 1

/* if don't have stat.h, then the cpp directive #define DONT_HAVE_STAT
   is included in import.c */

/* #undef HAVE_SYS_STAT_H */

#ifndef HAVE_SYS_STAT_H
#define DONT_HAVE_STAT
#define DONT_HAVE_FSTAT
#endif

/* Define if using PalmOS */
#define PALMOS 1
#ifdef PALMOS 

/* changed in /usr/local/palm/lib/gcc-lib/m68k-palmos-coff/2.95.2-kgpd/include/limits.h
   --> be sure to forward changes to sokolov 
*/


/* #undef INT_MAX */        /* this should be fixed in the palmos dev headers!! */
#define __INT_MAX__ 32767
#endif

/* Truncate doc strings to save global storage */
#define WITHOUT_DOC_STRINGS 1

/* other stuff - check src code for more info */
/* #undef OTHER_STUFF */

/* Function table lookup */
#define USE_FP_TABLE 1
#define PALM_GLIB_FIXES 1
#define WITHOUT_POSIXMODULE 1
#define WITHOUT_SIGNALMODULE 1
#define WITHOUT_PCREMODULE 1
#define WITH_GLOBAL_STRUCT 1
#define PALM_PLATFORM 1
/* #define SMALL_PLATFORM 1 */

#define DONT_SHARE_SHORT_STRINGS 1

/* These macros pertain to using the Palm Datamanager */

#define USE_PALMDM_REFS 1

#ifdef USE_PALMDM_REFS
#define PALMDM_REFCNT 32767
#define PALMDM_INTERN_STRINGS 
#define PALMDM_CODE_OBJECTS
#define PALMDM_DICT_OBJECTS
#else
#define SLOW_INTERN_STRINGS
#define PALMDM_REFCNT 32767
#endif /* USE_PALMDM_REFS */


/* the following has been extracted from stringobject.h to prevent
   the interning of strings */
#define CACHE_HASH 1

/* #define PALM_SERIAL_IO 1 */
/* #undef PALM_SERIAL_IO */
/* #define DEBUG_MESSAGES */

/* CUT_EXCESS_METHODS - eliminate unnecessary methods to reduce memory usage.
  Some these cuts could be included under other macro defs  */

#define CUT_EXCESS_METHODS

/* #define DEBUG_MESSAGES */
#define PALM_HOST_IO


#ifdef DEBUG_MESSAGES
#ifdef PALM_SERIAL_IO
#include "newPalmLibs/palm_lib.h"
#define DMESSAGE(mesg) { printSerial(mesg); }

#elif defined(PALM_HOST_IO)
#include "PalmGlue/host_lib.h"
#define DMESSAGE(mesg) {host_printf(mesg);}

#else  /* PALM_SERIAL_IO */
#define DMESSAGE(mesg) { ErrFatalDisplayIf(1, mesg); }
#endif /* PALM_SERIAL_IO */

#else /* DEBUG_MESSAGES */
#define DMESSAGE(mesg)
#endif /* DEBUG_MESSAGES */

#ifndef WITHOUT_DOC_STRINGS
#define DEF_DOC(varname, strng) static char varname[] = strng
#define USE_DOC(varname) varname
#else 
#define DEF_DOC(varname, strng)
#define USE_DOC(varname) NULL
#endif


/* defines for palm_socketmodule.c */
#define NO_DUP 1

/* use dl malloc instead of MemPtrNew directly*/
#define USE_DLMALLOC 1

