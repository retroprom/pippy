/***********************************************************
Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
The Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI or Corporation for National Research Initiatives or
CNRI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

While CWI is the initial source for this software, a modified version
is made available by the Corporation for National Research Initiatives
(CNRI) at the Internet address ftp://ftp.python.org.

STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

******************************************************************/

/* POSIX module implementation */

/* This file is also used for Windows NT and MS-Win.  In that case the module
   actually calls itself 'nt', not 'posix', and a few functions are
   either unimplemented or implemented differently.  The source
   assumes that for Windows NT, the macro 'MS_WIN32' is defined independent
   of the compiler used.  Different compilers define their own feature
   test macro, e.g. '__BORLANDC__' or '_MSC_VER'. */

/* See also ../Dos/dosmodule.c */

static char posix__doc__ [] =
"This module provides access to operating system functionality that is\n\
standardized by the C Standard and the POSIX standard (a thinly\n\
disguised Unix interface).  Refer to the library manual and\n\
corresponding Unix manual entries for more information on calls.";

#include "Python.h"

#if defined(PYOS_OS2)
#define  INCL_DOS
#define  INCL_DOSERRORS
#define  INCL_DOSPROCESS
#define  INCL_NOPMAPI
#include <os2.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>		/* For WNOHANG */
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif

#include "mytime.h"		/* For clock_t on some systems */

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif /* HAVE_FCNTL_H */

/* Various compilers have only certain posix functions */
/* XXX Gosh I wish these were all moved into config.h */
#if defined(PYCC_VACPP) && defined(PYOS_OS2)
#include <process.h>
#else
#if defined(__WATCOMC__) && !defined(__QNX__)		/* Watcom compiler */
#define HAVE_GETCWD     1
#define HAVE_OPENDIR    1
#define HAVE_SYSTEM	1
#if defined(__OS2__)
#define HAVE_EXECV      1
#define HAVE_WAIT       1
#endif
#include <process.h>
#else
#ifdef __BORLANDC__		/* Borland compiler */
#define HAVE_EXECV      1
#define HAVE_GETCWD     1
#define HAVE_GETEGID    1
#define HAVE_GETEUID    1
#define HAVE_GETGID     1
#define HAVE_GETPPID    1
#define HAVE_GETUID     1
#define HAVE_KILL       1
#define HAVE_OPENDIR    1
#define HAVE_PIPE       1
#define HAVE_POPEN      1
#define HAVE_SYSTEM	1
#define HAVE_WAIT       1
#else
#ifdef _MSC_VER		/* Microsoft compiler */
#define HAVE_GETCWD     1
#ifdef MS_WIN32
#define HAVE_SPAWNV	1
#define HAVE_EXECV      1
#define HAVE_PIPE       1
#define HAVE_POPEN      1
#define HAVE_SYSTEM	1
#else /* 16-bit Windows */
#endif /* !MS_WIN32 */
#else			/* all other compilers */
/* Unix functions that the configure script doesn't check for */
#define HAVE_EXECV      1
#define HAVE_FORK       1
#define HAVE_GETCWD     1
#define HAVE_GETEGID    1
#define HAVE_GETEUID    1
#define HAVE_GETGID     1
#define HAVE_GETPPID    1
#define HAVE_GETUID     1
#define HAVE_KILL       1
#define HAVE_OPENDIR    1
#define HAVE_PIPE       1
#define HAVE_POPEN      1
#define HAVE_SYSTEM	1
#define HAVE_WAIT       1
#define HAVE_TTYNAME	1
#endif  /* _MSC_VER */
#endif  /* __BORLANDC__ */
#endif  /* ! __WATCOMC__ || __QNX__ */
#endif /* ! __IBMC__ */

#ifndef _MSC_VER

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef NeXT
/* NeXT's <unistd.h> and <utime.h> aren't worth much */
#undef HAVE_UNISTD_H
#undef HAVE_UTIME_H
#define HAVE_WAITPID
/* #undef HAVE_GETCWD */
#define UNION_WAIT /* This should really be checked for by autoconf */
#endif

#ifdef HAVE_UNISTD_H
/* XXX These are for SunOS4.1.3 but shouldn't hurt elsewhere */
extern int rename();
extern int pclose();
extern int lstat();
extern int symlink();
extern int fsync();
#else /* !HAVE_UNISTD_H */
#if defined(PYCC_VACPP)
extern int mkdir Py_PROTO((char *));
#else
#if ( defined(__WATCOMC__) || defined(_MSC_VER) ) && !defined(__QNX__)
extern int mkdir Py_PROTO((const char *));
#else
extern int mkdir Py_PROTO((const char *, mode_t));
#endif
#endif
#if defined(__IBMC__) || defined(__IBMCPP__)
extern int chdir Py_PROTO((char *));
extern int rmdir Py_PROTO((char *));
#else
extern int chdir Py_PROTO((const char *));
extern int rmdir Py_PROTO((const char *));
#endif
extern int chmod Py_PROTO((const char *, mode_t));
extern int chown Py_PROTO((const char *, uid_t, gid_t));
extern char *getcwd Py_PROTO((char *, int));
extern char *strerror Py_PROTO((int));
extern int link Py_PROTO((const char *, const char *));
extern int rename Py_PROTO((const char *, const char *));
extern int stat Py_PROTO((const char *, struct stat *));
extern int unlink Py_PROTO((const char *));
extern int pclose Py_PROTO((FILE *));
#ifdef HAVE_SYMLINK
extern int symlink Py_PROTO((const char *, const char *));
#endif /* HAVE_SYMLINK */
#ifdef HAVE_LSTAT
extern int lstat Py_PROTO((const char *, struct stat *));
#endif /* HAVE_LSTAT */
#endif /* !HAVE_UNISTD_H */

#endif /* !_MSC_VER */

#ifdef HAVE_UTIME_H
#include <utime.h>
#endif /* HAVE_UTIME_H */

#ifdef HAVE_SYS_UTIME_H
#include <sys/utime.h>
#define HAVE_UTIME_H /* pretend we do for the rest of this file */
#endif /* HAVE_SYS_UTIME_H */

#ifdef HAVE_SYS_TIMES_H
#include <sys/times.h>
#endif /* HAVE_SYS_TIMES_H */

#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif /* HAVE_SYS_PARAM_H */

#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif /* HAVE_SYS_UTSNAME_H */

#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif /* MAXPATHLEN */

#ifdef HAVE_DIRENT_H
#include <dirent.h>
#define NAMLEN(dirent) strlen((dirent)->d_name)
#else
#if defined(__WATCOMC__) && !defined(__QNX__)
#include <direct.h>
#define NAMLEN(dirent) strlen((dirent)->d_name)
#else
#define dirent direct
#define NAMLEN(dirent) (dirent)->d_namlen
#endif
#ifdef HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#endif
#ifdef HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif
#ifdef HAVE_NDIR_H
#include <ndir.h>
#endif
#endif

#ifdef _MSC_VER
#include <direct.h>
#include <io.h>
#include <process.h>
#include <windows.h>
#ifdef MS_WIN32
#define popen	_popen
#define pclose	_pclose
#else /* 16-bit Windows */
#include <dos.h>
#include <ctype.h>
#endif /* MS_WIN32 */
#endif /* _MSC_VER */

#if defined(PYCC_VACPP) && defined(PYOS_OS2)
#include <io.h>
#endif /* OS2 */

#ifdef UNION_WAIT
/* Emulate some macros on systems that have a union instead of macros */

#ifndef WIFEXITED
#define WIFEXITED(u_wait) (!(u_wait).w_termsig && !(u_wait).w_coredump)
#endif

#ifndef WEXITSTATUS
#define WEXITSTATUS(u_wait) (WIFEXITED(u_wait)?((u_wait).w_retcode):-1)
#endif

#ifndef WTERMSIG
#define WTERMSIG(u_wait) ((u_wait).w_termsig)
#endif

#endif /* UNION_WAIT */

/* Don't use the "_r" form if we don't need it (also, won't have a
   prototype for it, at least on Solaris -- maybe others as well?). */
#if defined(HAVE_CTERMID_R) && defined(WITH_THREAD)
#define USE_CTERMID_R
#endif

#if defined(HAVE_TMPNAM_R) && defined(WITH_THREAD)
#define USE_TMPNAM_R
#endif

/* Return a dictionary corresponding to the POSIX environment table */

#if !defined(_MSC_VER) && ( !defined(__WATCOMC__) || defined(__QNX__) )
extern char **environ;
#endif /* !_MSC_VER */

static PyObject *
convertenviron()
{
	PyObject *d;
	char **e;
	d = PyDict_New();
	if (d == NULL)
		return NULL;
	if (environ == NULL)
		return d;
	/* This part ignores errors */
	for (e = environ; *e != NULL; e++) {
		PyObject *k;
		PyObject *v;
		char *p = strchr(*e, '=');
		if (p == NULL)
			continue;
		k = PyString_FromStringAndSize(*e, (int)(p-*e));
		if (k == NULL) {
			PyErr_Clear();
			continue;
		}
		v = PyString_FromString(p+1);
		if (v == NULL) {
			PyErr_Clear();
			Py_DECREF(k);
			continue;
		}
		if (PyDict_GetItem(d, k) == NULL) {
			if (PyDict_SetItem(d, k, v) != 0)
				PyErr_Clear();
		}
		Py_DECREF(k);
		Py_DECREF(v);
	}
#if defined(PYOS_OS2)
    {
        APIRET rc;
        char   buffer[1024]; /* OS/2 Provides a Documented Max of 1024 Chars */

        rc = DosQueryExtLIBPATH(buffer, BEGIN_LIBPATH);
        if (rc == NO_ERROR) { /* (not a type, envname is NOT 'BEGIN_LIBPATH') */
            PyObject *v = PyString_FromString(buffer);
		    PyDict_SetItemString(d, "BEGINLIBPATH", v);
            Py_DECREF(v);
        }
        rc = DosQueryExtLIBPATH(buffer, END_LIBPATH);
        if (rc == NO_ERROR) { /* (not a typo, envname is NOT 'END_LIBPATH') */
            PyObject *v = PyString_FromString(buffer);
		    PyDict_SetItemString(d, "ENDLIBPATH", v);
            Py_DECREF(v);
        }
    }
#endif
	return d;
}


/* Set a POSIX-specific error from errno, and return NULL */

static PyObject *
posix_error()
{
	return PyErr_SetFromErrno(PyExc_OSError);
}
static PyObject *
posix_error_with_filename(name)
	char* name;
{
	return PyErr_SetFromErrnoWithFilename(PyExc_OSError, name);
}


#if defined(PYOS_OS2)
/**********************************************************************
 *         Helper Function to Trim and Format OS/2 Messages
 **********************************************************************/
    static void
os2_formatmsg(char *msgbuf, int msglen, char *reason)
{
    msgbuf[msglen] = '\0'; /* OS/2 Doesn't Guarantee a Terminator */

    if (strlen(msgbuf) > 0) { /* If Non-Empty Msg, Trim CRLF */
        char *lastc = &msgbuf[ strlen(msgbuf)-1 ];

        while (lastc > msgbuf && isspace(*lastc))
            *lastc-- = '\0'; /* Trim Trailing Whitespace (CRLF) */
    }

    /* Add Optional Reason Text */
    if (reason) {
        strcat(msgbuf, " : ");
        strcat(msgbuf, reason);
    }
}

/**********************************************************************
 *             Decode an OS/2 Operating System Error Code
 *
 * A convenience function to lookup an OS/2 error code and return a
 * text message we can use to raise a Python exception.
 *
 * Notes:
 *   The messages for errors returned from the OS/2 kernel reside in
 *   the file OSO001.MSG in the \OS2 directory hierarchy.
 *
 **********************************************************************/
    static char *
os2_strerror(char *msgbuf, int msgbuflen, int errorcode, char *reason)
{
    APIRET rc;
    ULONG  msglen;

    /* Retrieve Kernel-Related Error Message from OSO001.MSG File */
    Py_BEGIN_ALLOW_THREADS
    rc = DosGetMessage(NULL, 0, msgbuf, msgbuflen,
                       errorcode, "oso001.msg", &msglen);
    Py_END_ALLOW_THREADS

    if (rc == NO_ERROR)
        os2_formatmsg(msgbuf, msglen, reason);
    else
        sprintf(msgbuf, "unknown OS error #%d", errorcode);

    return msgbuf;
}

/* Set an OS/2-specific error and return NULL.  OS/2 kernel
   errors are not in a global variable e.g. 'errno' nor are
   they congruent with posix error numbers. */

static PyObject * os2_error(int code)
{
    char text[1024];
    PyObject *v;

    os2_strerror(text, sizeof(text), code, "");

    v = Py_BuildValue("(is)", code, text);
    if (v != NULL) {
        PyErr_SetObject(PyExc_OSError, v);
        Py_DECREF(v);
    }
    return NULL; /* Signal to Python that an Exception is Pending */
}

#endif /* OS2 */

/* POSIX generic methods */

static PyObject *
posix_int(args, format, func)
        PyObject *args;
        char *format;
	int (*func) Py_FPROTO((int));
{
	int fd;
	int res;
	if (!PyArg_ParseTuple(args,  format, &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*func)(fd);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}


static PyObject *
posix_1str(args, format, func)
	PyObject *args;
        char *format;
	int (*func) Py_FPROTO((const char *));
{
	char *path1;
	int res;
	if (!PyArg_ParseTuple(args, format, &path1))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*func)(path1);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error_with_filename(path1);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
posix_2str(args, format, func)
	PyObject *args;
        char *format;
	int (*func) Py_FPROTO((const char *, const char *));
{
	char *path1, *path2;
	int res;
	if (!PyArg_ParseTuple(args, format, &path1, &path2))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*func)(path1, path2);
	Py_END_ALLOW_THREADS
	if (res != 0)
		/* XXX how to report both path1 and path2??? */
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
posix_strint(args, format, func)
	PyObject *args;
        char *format;
	int (*func) Py_FPROTO((const char *, int));
{
	char *path;
	int i;
	int res;
	if (!PyArg_ParseTuple(args, format, &path, &i))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*func)(path, i);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error_with_filename(path);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
posix_strintint(args, format, func)
	PyObject *args;
        char *format;
	int (*func) Py_FPROTO((const char *, int, int));
{
	char *path;
	int i,i2;
	int res;
	if (!PyArg_ParseTuple(args, format, &path, &i, &i2))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*func)(path, i, i2);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error_with_filename(path);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
posix_do_stat(self, args, format, statfunc)
	PyObject *self;
	PyObject *args;
        char *format;
	int (*statfunc) Py_FPROTO((const char *, struct stat *));
{
	struct stat st;
	char *path;
	int res;
	if (!PyArg_ParseTuple(args, format, &path))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = (*statfunc)(path, &st);
	Py_END_ALLOW_THREADS
	if (res != 0)
		return posix_error_with_filename(path);
#if !defined(HAVE_LARGEFILE_SUPPORT)
	return Py_BuildValue("(llllllllll)",
			     (long)st.st_mode,
			     (long)st.st_ino,
			     (long)st.st_dev,
			     (long)st.st_nlink,
			     (long)st.st_uid,
			     (long)st.st_gid,
			     (long)st.st_size,
			     (long)st.st_atime,
			     (long)st.st_mtime,
			     (long)st.st_ctime);
#else
	return Py_BuildValue("(lLllllLlll)",
			     (long)st.st_mode,
			     (LONG_LONG)st.st_ino,
			     (long)st.st_dev,
			     (long)st.st_nlink,
			     (long)st.st_uid,
			     (long)st.st_gid,
			     (LONG_LONG)st.st_size,
			     (long)st.st_atime,
			     (long)st.st_mtime,
			     (long)st.st_ctime);
#endif
}


/* POSIX methods */

static char posix_access__doc__[] =
"access(path, mode) -> 1 if granted, 0 otherwise\n\
Test for access to a file.";

static PyObject *
posix_access(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	int mode;
	int res;

	if (!PyArg_ParseTuple(args, "si:access", &path, &mode))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = access(path, mode);
	Py_END_ALLOW_THREADS
	return(PyInt_FromLong(res == 0 ? 1L : 0L));
}

#ifndef F_OK
#define F_OK 0
#endif
#ifndef R_OK
#define R_OK 4
#endif
#ifndef W_OK
#define W_OK 2
#endif
#ifndef X_OK
#define X_OK 1
#endif

#ifdef HAVE_TTYNAME
static char posix_ttyname__doc__[] =
"ttyname(fd) -> String\n\
Return the name of the terminal device connected to 'fd'.";

static PyObject *
posix_ttyname(self, args)
	PyObject *self;
	PyObject *args;
{
	int id;
	char *ret;

	if (!PyArg_ParseTuple(args, "i:ttyname", &id))
		return NULL;

	ret = ttyname(id);
	if (ret == NULL)
		return(posix_error());
	return(PyString_FromString(ret));
}
#endif

#ifdef HAVE_CTERMID
static char posix_ctermid__doc__[] =
"ctermid() -> String\n\
Return the name of the controlling terminal for this process.";

static PyObject *
posix_ctermid(self, args)
	PyObject *self;
	PyObject *args;
{
        char *ret;
        char buffer[L_ctermid];

	if (!PyArg_ParseTuple(args, ":ctermid"))
		return NULL;

#ifdef USE_CTERMID_R
	ret = ctermid_r(buffer);
#else
        ret = ctermid(buffer);
#endif
	if (ret == NULL)
		return(posix_error());
	return(PyString_FromString(buffer));
}
#endif

static char posix_chdir__doc__[] =
"chdir(path) -> None\n\
Change the current working directory to the specified path.";

static PyObject *
posix_chdir(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_1str(args, "s:chdir", chdir);
}


static char posix_chmod__doc__[] =
"chmod(path, mode) -> None\n\
Change the access permissions of a file.";

static PyObject *
posix_chmod(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_strint(args, "si:chmod", chmod);
}


#ifdef HAVE_FSYNC
static char posix_fsync__doc__[] =
"fsync(fildes) -> None\n\
force write of file with filedescriptor to disk.";

static PyObject *
posix_fsync(self, args)
       PyObject *self;
       PyObject *args;
{
       return posix_int(args, "i:fsync", fsync);
}
#endif /* HAVE_FSYNC */

#ifdef HAVE_FDATASYNC
static char posix_fdatasync__doc__[] =
"fdatasync(fildes) -> None\n\
force write of file with filedescriptor to disk.\n\
 does not force update of metadata.";

extern int fdatasync(int); /* Prototype just in case */

static PyObject *
posix_fdatasync(self, args)
       PyObject *self;
       PyObject *args;
{
       return posix_int(args, "i:fdatasync", fdatasync);
}
#endif /* HAVE_FDATASYNC */


#ifdef HAVE_CHOWN
static char posix_chown__doc__[] =
"chown(path, uid, gid) -> None\n\
Change the owner and group id of path to the numeric uid and gid.";

static PyObject *
posix_chown(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_strintint(args, "sii:chown", chown);
}
#endif /* HAVE_CHOWN */


#ifdef HAVE_GETCWD
static char posix_getcwd__doc__[] =
"getcwd() -> path\n\
Return a string representing the current working directory.";

static PyObject *
posix_getcwd(self, args)
	PyObject *self;
	PyObject *args;
{
	char buf[1026];
	char *res;
	if (!PyArg_ParseTuple(args, ":getcwd"))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = getcwd(buf, sizeof buf);
	Py_END_ALLOW_THREADS
	if (res == NULL)
		return posix_error();
	return PyString_FromString(buf);
}
#endif


#ifdef HAVE_LINK
static char posix_link__doc__[] =
"link(src, dst) -> None\n\
Create a hard link to a file.";

static PyObject *
posix_link(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_2str(args, "ss:link", link);
}
#endif /* HAVE_LINK */


static char posix_listdir__doc__[] =
"listdir(path) -> list_of_strings\n\
Return a list containing the names of the entries in the directory.\n\
\n\
	path: path of directory to list\n\
\n\
The list is in arbitrary order.  It does not include the special\n\
entries '.' and '..' even if they are present in the directory.";

static PyObject *
posix_listdir(self, args)
	PyObject *self;
	PyObject *args;
{
	/* XXX Should redo this putting the (now four) versions of opendir
	   in separate files instead of having them all here... */
#if defined(MS_WIN32) && !defined(HAVE_OPENDIR)

	char *name;
	int len;
	PyObject *d, *v;
	HANDLE hFindFile;
	WIN32_FIND_DATA FileData;
	char namebuf[MAX_PATH+5];

	if (!PyArg_ParseTuple(args, "t#:listdir", &name, &len))
		return NULL;
	if (len >= MAX_PATH) {
		PyErr_SetString(PyExc_ValueError, "path too long");
		return NULL;
	}
	strcpy(namebuf, name);
	if (namebuf[len-1] != '/' && namebuf[len-1] != '\\')
		namebuf[len++] = '/';
	strcpy(namebuf + len, "*.*");

	if ((d = PyList_New(0)) == NULL)
		return NULL;

	hFindFile = FindFirstFile(namebuf, &FileData);
	if (hFindFile == INVALID_HANDLE_VALUE) {
		errno = GetLastError();
		if (errno == ERROR_FILE_NOT_FOUND)
			return PyList_New(0);
		return posix_error_with_filename(name);
	}
	do {
		if (FileData.cFileName[0] == '.' &&
		    (FileData.cFileName[1] == '\0' ||
		     FileData.cFileName[1] == '.' &&
		     FileData.cFileName[2] == '\0'))
			continue;
		v = PyString_FromString(FileData.cFileName);
		if (v == NULL) {
			Py_DECREF(d);
			d = NULL;
			break;
		}
		if (PyList_Append(d, v) != 0) {
			Py_DECREF(v);
			Py_DECREF(d);
			d = NULL;
			break;
		}
		Py_DECREF(v);
	} while (FindNextFile(hFindFile, &FileData) == TRUE);

	if (FindClose(hFindFile) == FALSE) {
		errno = GetLastError();
		return posix_error_with_filename(&name);
	}

	return d;

#else /* !MS_WIN32 */
#ifdef _MSC_VER /* 16-bit Windows */

#ifndef MAX_PATH
#define MAX_PATH	250
#endif
	char *name, *pt;
	int len;
	PyObject *d, *v;
	char namebuf[MAX_PATH+5];
	struct _find_t ep;

	if (!PyArg_ParseTuple(args, "t#:listdir", &name, &len))
		return NULL;
	if (len >= MAX_PATH) {
		PyErr_SetString(PyExc_ValueError, "path too long");
		return NULL;
	}
	strcpy(namebuf, name);
	for (pt = namebuf; *pt; pt++)
		if (*pt == '/')
			*pt = '\\';
	if (namebuf[len-1] != '\\')
		namebuf[len++] = '\\';
	strcpy(namebuf + len, "*.*");

	if ((d = PyList_New(0)) == NULL)
		return NULL;

	if (_dos_findfirst(namebuf, _A_RDONLY |
			   _A_HIDDEN | _A_SYSTEM | _A_SUBDIR, &ep) != 0)
        {
		errno = ENOENT;
		return posix_error_with_filename(name);
	}
	do {
		if (ep.name[0] == '.' &&
		    (ep.name[1] == '\0' ||
		     ep.name[1] == '.' &&
		     ep.name[2] == '\0'))
			continue;
		strcpy(namebuf, ep.name);
		for (pt = namebuf; *pt; pt++)
			if (isupper(*pt))
				*pt = tolower(*pt);
		v = PyString_FromString(namebuf);
		if (v == NULL) {
			Py_DECREF(d);
			d = NULL;
			break;
		}
		if (PyList_Append(d, v) != 0) {
			Py_DECREF(v);
			Py_DECREF(d);
			d = NULL;
			break;
		}
		Py_DECREF(v);
	} while (_dos_findnext(&ep) == 0);

	return d;

#else
#if defined(PYOS_OS2)

#ifndef MAX_PATH
#define MAX_PATH    CCHMAXPATH
#endif
    char *name, *pt;
    int len;
    PyObject *d, *v;
    char namebuf[MAX_PATH+5];
    HDIR  hdir = 1;
    ULONG srchcnt = 1;
    FILEFINDBUF3   ep;
    APIRET rc;

    if (!PyArg_ParseTuple(args, "t#:listdir", &name, &len))
        return NULL;
    if (len >= MAX_PATH) {
		PyErr_SetString(PyExc_ValueError, "path too long");
        return NULL;
    }
    strcpy(namebuf, name);
    for (pt = namebuf; *pt; pt++)
        if (*pt == '/')
            *pt = '\\';
    if (namebuf[len-1] != '\\')
        namebuf[len++] = '\\';
    strcpy(namebuf + len, "*.*");

	if ((d = PyList_New(0)) == NULL)
        return NULL;

    rc = DosFindFirst(namebuf,         /* Wildcard Pattern to Match */
                      &hdir,           /* Handle to Use While Search Directory */
                      FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | FILE_DIRECTORY,
                      &ep, sizeof(ep), /* Structure to Receive Directory Entry */
                      &srchcnt,        /* Max and Actual Count of Entries Per Iteration */
                      FIL_STANDARD);   /* Format of Entry (EAs or Not) */

    if (rc != NO_ERROR) {
        errno = ENOENT;
        return posix_error_with_filename(name);
    }

    if (srchcnt > 0) { /* If Directory is NOT Totally Empty, */
        do {
            if (ep.achName[0] == '.'
            && (ep.achName[1] == '\0' || ep.achName[1] == '.' && ep.achName[2] == '\0'))
                continue; /* Skip Over "." and ".." Names */

            strcpy(namebuf, ep.achName);

            /* Leave Case of Name Alone -- In Native Form */
            /* (Removed Forced Lowercasing Code) */

            v = PyString_FromString(namebuf);
            if (v == NULL) {
                Py_DECREF(d);
                d = NULL;
                break;
            }
            if (PyList_Append(d, v) != 0) {
                Py_DECREF(v);
                Py_DECREF(d);
                d = NULL;
                break;
            }
            Py_DECREF(v);
        } while (DosFindNext(hdir, &ep, sizeof(ep), &srchcnt) == NO_ERROR && srchcnt > 0);
    }

    return d;
#else

	char *name;
	PyObject *d, *v;
	DIR *dirp;
	struct dirent *ep;
	if (!PyArg_ParseTuple(args, "s:listdir", &name))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	if ((dirp = opendir(name)) == NULL) {
		Py_BLOCK_THREADS
		return posix_error_with_filename(name);
	}
	if ((d = PyList_New(0)) == NULL) {
		closedir(dirp);
		Py_BLOCK_THREADS
		return NULL;
	}
	while ((ep = readdir(dirp)) != NULL) {
		if (ep->d_name[0] == '.' &&
		    (NAMLEN(ep) == 1 ||
		     (ep->d_name[1] == '.' && NAMLEN(ep) == 2)))
			continue;
		v = PyString_FromStringAndSize(ep->d_name, NAMLEN(ep));
		if (v == NULL) {
			Py_DECREF(d);
			d = NULL;
			break;
		}
		if (PyList_Append(d, v) != 0) {
			Py_DECREF(v);
			Py_DECREF(d);
			d = NULL;
			break;
		}
		Py_DECREF(v);
	}
	closedir(dirp);
	Py_END_ALLOW_THREADS

	return d;

#endif /* !PYOS_OS2 */
#endif /* !_MSC_VER */
#endif /* !MS_WIN32 */
}

static char posix_mkdir__doc__[] =
"mkdir(path [, mode=0777]) -> None\n\
Create a directory.";

static PyObject *
posix_mkdir(self, args)
	PyObject *self;
	PyObject *args;
{
	int res;
	char *path;
	int mode = 0777;
	if (!PyArg_ParseTuple(args, "s|i:mkdir", &path, &mode))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
#if ( defined(__WATCOMC__) || defined(_MSC_VER) || defined(PYCC_VACPP) ) && !defined(__QNX__)
	res = mkdir(path);
#else
	res = mkdir(path, mode);
#endif
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error_with_filename(path);
	Py_INCREF(Py_None);
	return Py_None;
}


#ifdef HAVE_NICE
static char posix_nice__doc__[] =
"nice(inc) -> new_priority\n\
Decrease the priority of process and return new priority.";

static PyObject *
posix_nice(self, args)
	PyObject *self;
	PyObject *args;
{
	int increment, value;

	if (!PyArg_ParseTuple(args, "i:nice", &increment))
		return NULL;
	value = nice(increment);
	if (value == -1)
		return posix_error();
	return PyInt_FromLong((long) value);
}
#endif /* HAVE_NICE */


static char posix_rename__doc__[] =
"rename(old, new) -> None\n\
Rename a file or directory.";

static PyObject *
posix_rename(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_2str(args, "ss:rename", rename);
}


static char posix_rmdir__doc__[] =
"rmdir(path) -> None\n\
Remove a directory.";

static PyObject *
posix_rmdir(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_1str(args, "s:rmdir", rmdir);
}


static char posix_stat__doc__[] =
"stat(path) -> (mode,ino,dev,nlink,uid,gid,size,atime,mtime,ctime)\n\
Perform a stat system call on the given path.";

static PyObject *
posix_stat(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_do_stat(self, args, "s:stat", stat);
}


#ifdef HAVE_SYSTEM
static char posix_system__doc__[] =
"system(command) -> exit_status\n\
Execute the command (a string) in a subshell.";

static PyObject *
posix_system(self, args)
	PyObject *self;
	PyObject *args;
{
	char *command;
	long sts;
	if (!PyArg_ParseTuple(args, "s:system", &command))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	sts = system(command);
	Py_END_ALLOW_THREADS
	return PyInt_FromLong(sts);
}
#endif


static char posix_umask__doc__[] =
"umask(new_mask) -> old_mask\n\
Set the current numeric umask and return the previous umask.";

static PyObject *
posix_umask(self, args)
	PyObject *self;
	PyObject *args;
{
	int i;
	if (!PyArg_ParseTuple(args, "i:umask", &i))
		return NULL;
	i = umask(i);
	if (i < 0)
		return posix_error();
	return PyInt_FromLong((long)i);
}


static char posix_unlink__doc__[] =
"unlink(path) -> None\n\
Remove a file (same as remove(path)).";

static char posix_remove__doc__[] =
"remove(path) -> None\n\
Remove a file (same as unlink(path)).";

static PyObject *
posix_unlink(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_1str(args, "s:remove", unlink);
}


#ifdef HAVE_UNAME
static char posix_uname__doc__[] =
"uname() -> (sysname, nodename, release, version, machine)\n\
Return a tuple identifying the current operating system.";

static PyObject *
posix_uname(self, args)
	PyObject *self;
	PyObject *args;
{
	struct utsname u;
	int res;
	if (!PyArg_ParseTuple(args, ":uname"))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = uname(&u);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();
	return Py_BuildValue("(sssss)",
			     u.sysname,
			     u.nodename,
			     u.release,
			     u.version,
			     u.machine);
}
#endif /* HAVE_UNAME */


static char posix_utime__doc__[] =
"utime(path, (atime, utime)) -> None\n\
Set the access and modified time of the file to the given values.";

static PyObject *
posix_utime(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	long atime, mtime;
	int res;

/* XXX should define struct utimbuf instead, above */
#ifdef HAVE_UTIME_H
	struct utimbuf buf;
#define ATIME buf.actime
#define MTIME buf.modtime
#define UTIME_ARG &buf
#else /* HAVE_UTIME_H */
	time_t buf[2];
#define ATIME buf[0]
#define MTIME buf[1]
#define UTIME_ARG buf
#endif /* HAVE_UTIME_H */

	if (!PyArg_ParseTuple(args, "s(ll):utime", &path, &atime, &mtime))
		return NULL;
	ATIME = atime;
	MTIME = mtime;
	Py_BEGIN_ALLOW_THREADS
	res = utime(path, UTIME_ARG);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error_with_filename(path);
	Py_INCREF(Py_None);
	return Py_None;
#undef UTIME_ARG
#undef ATIME
#undef MTIME
}


/* Process operations */

static char posix__exit__doc__[] =
"_exit(status)\n\
Exit to the system with specified status, without normal exit processing.";

static PyObject *
posix__exit(self, args)
	PyObject *self;
	PyObject *args;
{
	int sts;
	if (!PyArg_ParseTuple(args, "i:_exit", &sts))
		return NULL;
	_exit(sts);
	return NULL; /* Make gcc -Wall happy */
}


#ifdef HAVE_EXECV
static char posix_execv__doc__[] =
"execv(path, args)\n\
Execute an executable path with arguments, replacing current process.\n\
\n\
	path: path of executable file\n\
	args: tuple or list of strings";

static PyObject *
posix_execv(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	PyObject *argv;
	char **argvlist;
	int i, argc;
	PyObject *(*getitem) Py_PROTO((PyObject *, int));

	/* execv has two arguments: (path, argv), where
	   argv is a list or tuple of strings. */

	if (!PyArg_ParseTuple(args, "sO:execv", &path, &argv))
		return NULL;
	if (PyList_Check(argv)) {
		argc = PyList_Size(argv);
		getitem = PyList_GetItem;
	}
	else if (PyTuple_Check(argv)) {
		argc = PyTuple_Size(argv);
		getitem = PyTuple_GetItem;
	}
	else {
 badarg:
		PyErr_BadArgument();
		return NULL;
	}

	argvlist = PyMem_NEW(char *, argc+1);
	if (argvlist == NULL)
		return NULL;
	for (i = 0; i < argc; i++) {
		if (!PyArg_Parse((*getitem)(argv, i), "s", &argvlist[i])) {
			PyMem_DEL(argvlist);
			goto badarg;
		}
	}
	argvlist[argc] = NULL;

#ifdef BAD_EXEC_PROTOTYPES
	execv(path, (const char **) argvlist);
#else /* BAD_EXEC_PROTOTYPES */
	execv(path, argvlist);
#endif /* BAD_EXEC_PROTOTYPES */

	/* If we get here it's definitely an error */

	PyMem_DEL(argvlist);
	return posix_error();
}


static char posix_execve__doc__[] =
"execve(path, args, env)\n\
Execute a path with arguments and environment, replacing current process.\n\
\n\
	path: path of executable file\n\
	args: tuple or list of arguments\n\
	env: dictonary of strings mapping to strings";

static PyObject *
posix_execve(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	PyObject *argv, *env;
	char **argvlist;
	char **envlist;
	PyObject *key, *val, *keys=NULL, *vals=NULL;
	int i, pos, argc, envc;
	PyObject *(*getitem) Py_PROTO((PyObject *, int));

	/* execve has three arguments: (path, argv, env), where
	   argv is a list or tuple of strings and env is a dictionary
	   like posix.environ. */

	if (!PyArg_ParseTuple(args, "sOO:execve", &path, &argv, &env))
		return NULL;
	if (PyList_Check(argv)) {
		argc = PyList_Size(argv);
		getitem = PyList_GetItem;
	}
	else if (PyTuple_Check(argv)) {
		argc = PyTuple_Size(argv);
		getitem = PyTuple_GetItem;
	}
	else {
		PyErr_SetString(PyExc_TypeError, "argv must be tuple or list");
		return NULL;
	}
	if (!PyMapping_Check(env)) {
		PyErr_SetString(PyExc_TypeError, "env must be mapping object");
		return NULL;
	}

	argvlist = PyMem_NEW(char *, argc+1);
	if (argvlist == NULL) {
		PyErr_NoMemory();
		return NULL;
	}
	for (i = 0; i < argc; i++) {
		if (!PyArg_Parse((*getitem)(argv, i),
				 "s;argv must be list of strings",
				 &argvlist[i]))
		{
			goto fail_1;
		}
	}
	argvlist[argc] = NULL;

	i = PyMapping_Length(env);
	envlist = PyMem_NEW(char *, i + 1);
	if (envlist == NULL) {
		PyErr_NoMemory();
		goto fail_1;
	}
	envc = 0;
	keys = PyMapping_Keys(env);
	vals = PyMapping_Values(env);
	if (!keys || !vals)
		goto fail_2;
	
	for (pos = 0; pos < i; pos++) {
		char *p, *k, *v;

		key = PyList_GetItem(keys, pos);
		val = PyList_GetItem(vals, pos);
		if (!key || !val)
			goto fail_2;
		
		if (!PyArg_Parse(key, "s;non-string key in env", &k) ||
		    !PyArg_Parse(val, "s;non-string value in env", &v))
		{
			goto fail_2;
		}

#if defined(PYOS_OS2)
        /* Omit Pseudo-Env Vars that Would Confuse Programs if Passed On */
        if (stricmp(k, "BEGINLIBPATH") != 0 && stricmp(k, "ENDLIBPATH") != 0) {
#endif
		p = PyMem_NEW(char, PyString_Size(key)+PyString_Size(val) + 2);
		if (p == NULL) {
			PyErr_NoMemory();
			goto fail_2;
		}
		sprintf(p, "%s=%s", k, v);
		envlist[envc++] = p;
#if defined(PYOS_OS2)
    }
#endif
	}
	envlist[envc] = 0;


#ifdef BAD_EXEC_PROTOTYPES
	execve(path, (const char **)argvlist, envlist);
#else /* BAD_EXEC_PROTOTYPES */
	execve(path, argvlist, envlist);
#endif /* BAD_EXEC_PROTOTYPES */
	
	/* If we get here it's definitely an error */

	(void) posix_error();

 fail_2:
	while (--envc >= 0)
		PyMem_DEL(envlist[envc]);
	PyMem_DEL(envlist);
 fail_1:
	PyMem_DEL(argvlist);
	Py_XDECREF(vals);
	Py_XDECREF(keys);
	return NULL;
}
#endif /* HAVE_EXECV */


#ifdef HAVE_SPAWNV
static char posix_spawnv__doc__[] =
"spawnv(mode, path, args)\n\
Execute an executable path with arguments, replacing current process.\n\
\n\
	mode: mode of process creation\n\
	path: path of executable file\n\
	args: tuple or list of strings";

static PyObject *
posix_spawnv(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	PyObject *argv;
	char **argvlist;
	int mode, i, argc;
	PyObject *(*getitem) Py_PROTO((PyObject *, int));

	/* spawnv has three arguments: (mode, path, argv), where
	   argv is a list or tuple of strings. */

	if (!PyArg_ParseTuple(args, "isO:spawnv", &mode, &path, &argv))
		return NULL;
	if (PyList_Check(argv)) {
		argc = PyList_Size(argv);
		getitem = PyList_GetItem;
	}
	else if (PyTuple_Check(argv)) {
		argc = PyTuple_Size(argv);
		getitem = PyTuple_GetItem;
	}
	else {
 badarg:
		PyErr_BadArgument();
		return NULL;
	}

	argvlist = PyMem_NEW(char *, argc+1);
	if (argvlist == NULL)
		return NULL;
	for (i = 0; i < argc; i++) {
		if (!PyArg_Parse((*getitem)(argv, i), "s", &argvlist[i])) {
			PyMem_DEL(argvlist);
			goto badarg;
		}
	}
	argvlist[argc] = NULL;

	if (mode == _OLD_P_OVERLAY)
		mode = _P_OVERLAY;
	i = _spawnv(mode, path, argvlist);

	PyMem_DEL(argvlist);

	if (i == -1)
		return posix_error();
	else
		return Py_BuildValue("i", i);
}


static char posix_spawnve__doc__[] =
"spawnve(mode, path, args, env)\n\
Execute a path with arguments and environment, replacing current process.\n\
\n\
	mode: mode of process creation\n\
	path: path of executable file\n\
	args: tuple or list of arguments\n\
	env: dictonary of strings mapping to strings";

static PyObject *
posix_spawnve(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	PyObject *argv, *env;
	char **argvlist;
	char **envlist;
	PyObject *key, *val, *keys=NULL, *vals=NULL, *res=NULL;
	int mode, i, pos, argc, envc;
	PyObject *(*getitem) Py_PROTO((PyObject *, int));

	/* spawnve has four arguments: (mode, path, argv, env), where
	   argv is a list or tuple of strings and env is a dictionary
	   like posix.environ. */

	if (!PyArg_ParseTuple(args, "isOO:spawnve", &mode, &path, &argv, &env))
		return NULL;
	if (PyList_Check(argv)) {
		argc = PyList_Size(argv);
		getitem = PyList_GetItem;
	}
	else if (PyTuple_Check(argv)) {
		argc = PyTuple_Size(argv);
		getitem = PyTuple_GetItem;
	}
	else {
		PyErr_SetString(PyExc_TypeError, "argv must be tuple or list");
		return NULL;
	}
	if (!PyMapping_Check(env)) {
		PyErr_SetString(PyExc_TypeError, "env must be mapping object");
		return NULL;
	}

	argvlist = PyMem_NEW(char *, argc+1);
	if (argvlist == NULL) {
		PyErr_NoMemory();
		return NULL;
	}
	for (i = 0; i < argc; i++) {
		if (!PyArg_Parse((*getitem)(argv, i),
				 "s;argv must be list of strings",
				 &argvlist[i]))
		{
			goto fail_1;
		}
	}
	argvlist[argc] = NULL;

	i = PyMapping_Length(env);
	envlist = PyMem_NEW(char *, i + 1);
	if (envlist == NULL) {
		PyErr_NoMemory();
		goto fail_1;
	}
	envc = 0;
	keys = PyMapping_Keys(env);
	vals = PyMapping_Values(env);
	if (!keys || !vals)
		goto fail_2;
	
	for (pos = 0; pos < i; pos++) {
		char *p, *k, *v;

		key = PyList_GetItem(keys, pos);
		val = PyList_GetItem(vals, pos);
		if (!key || !val)
			goto fail_2;
		
		if (!PyArg_Parse(key, "s;non-string key in env", &k) ||
		    !PyArg_Parse(val, "s;non-string value in env", &v))
		{
			goto fail_2;
		}
		p = PyMem_NEW(char, PyString_Size(key)+PyString_Size(val) + 2);
		if (p == NULL) {
			PyErr_NoMemory();
			goto fail_2;
		}
		sprintf(p, "%s=%s", k, v);
		envlist[envc++] = p;
	}
	envlist[envc] = 0;

	if (mode == _OLD_P_OVERLAY)
		mode = _P_OVERLAY;
	i = _spawnve(mode, path, argvlist, envlist);
	if (i == -1)
		(void) posix_error();
	else
		res = Py_BuildValue("i", i);

 fail_2:
	while (--envc >= 0)
		PyMem_DEL(envlist[envc]);
	PyMem_DEL(envlist);
 fail_1:
	PyMem_DEL(argvlist);
	Py_XDECREF(vals);
	Py_XDECREF(keys);
	return res;
}
#endif /* HAVE_SPAWNV */


#ifdef HAVE_FORK
static char posix_fork__doc__[] =
"fork() -> pid\n\
Fork a child process.\n\
\n\
Return 0 to child process and PID of child to parent process.";

static PyObject *
posix_fork(self, args)
	PyObject *self;
	PyObject *args;
{
	int pid;
	if (!PyArg_ParseTuple(args, ":fork"))
		return NULL;
	pid = fork();
	if (pid == -1)
		return posix_error();
	PyOS_AfterFork();
	return PyInt_FromLong((long)pid);
}
#endif


#ifdef HAVE_GETEGID
static char posix_getegid__doc__[] =
"getegid() -> egid\n\
Return the current process's effective group id.";

static PyObject *
posix_getegid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getegid"))
		return NULL;
	return PyInt_FromLong((long)getegid());
}
#endif


#ifdef HAVE_GETEUID
static char posix_geteuid__doc__[] =
"geteuid() -> euid\n\
Return the current process's effective user id.";

static PyObject *
posix_geteuid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":geteuid"))
		return NULL;
	return PyInt_FromLong((long)geteuid());
}
#endif


#ifdef HAVE_GETGID
static char posix_getgid__doc__[] =
"getgid() -> gid\n\
Return the current process's group id.";

static PyObject *
posix_getgid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getgid"))
		return NULL;
	return PyInt_FromLong((long)getgid());
}
#endif


static char posix_getpid__doc__[] =
"getpid() -> pid\n\
Return the current process id";

static PyObject *
posix_getpid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getpid"))
		return NULL;
	return PyInt_FromLong((long)getpid());
}


#ifdef HAVE_GETGROUPS
static char posix_getgroups__doc__[] = "\
getgroups() -> list of group IDs\n\
Return list of supplemental group IDs for the process.";

static PyObject *
posix_getgroups(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;

    if (PyArg_ParseTuple(args, ":getgroups")) {
#ifdef NGROUPS_MAX
#define MAX_GROUPS NGROUPS_MAX
#else
        /* defined to be 16 on Solaris7, so this should be a small number */
#define MAX_GROUPS 64
#endif
        gid_t grouplist[MAX_GROUPS];
        int n;

        n = getgroups(MAX_GROUPS, grouplist);
        if (n < 0)
            posix_error();
        else {
            result = PyList_New(n);
            if (result != NULL) {
                PyObject *o;
                int i;
                for (i = 0; i < n; ++i) {
                    o = PyInt_FromLong((long)grouplist[i]);
                    if (o == NULL) {
                        Py_DECREF(result);
                        result = NULL;
                        break;
                    }
                    PyList_SET_ITEM(result, i, o);
                }
            }
        }
    }
    return result;
}
#endif

#ifdef HAVE_GETPGRP
static char posix_getpgrp__doc__[] =
"getpgrp() -> pgrp\n\
Return the current process group id.";

static PyObject *
posix_getpgrp(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getpgrp"))
		return NULL;
#ifdef GETPGRP_HAVE_ARG
	return PyInt_FromLong((long)getpgrp(0));
#else /* GETPGRP_HAVE_ARG */
	return PyInt_FromLong((long)getpgrp());
#endif /* GETPGRP_HAVE_ARG */
}
#endif /* HAVE_GETPGRP */


#ifdef HAVE_SETPGRP
static char posix_setpgrp__doc__[] =
"setpgrp() -> None\n\
Make this process a session leader.";

static PyObject *
posix_setpgrp(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":setpgrp"))
		return NULL;
#ifdef SETPGRP_HAVE_ARG
	if (setpgrp(0, 0) < 0)
#else /* SETPGRP_HAVE_ARG */
	if (setpgrp() < 0)
#endif /* SETPGRP_HAVE_ARG */
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}

#endif /* HAVE_SETPGRP */

#ifdef HAVE_GETPPID
static char posix_getppid__doc__[] =
"getppid() -> ppid\n\
Return the parent's process id.";

static PyObject *
posix_getppid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getppid"))
		return NULL;
	return PyInt_FromLong((long)getppid());
}
#endif


#ifdef HAVE_GETLOGIN
static char posix_getlogin__doc__[] = "\
getlogin() -> string\n\
Return the actual login name.";

static PyObject *
posix_getlogin(self, args)
	PyObject *self;
	PyObject *args;
{
    PyObject *result = NULL;

    if (PyArg_ParseTuple(args, ":getlogin")) {
        char *name = getlogin();

        if (name == NULL)
            posix_error();
        else
            result = PyString_FromString(name);
    }
    return result;
}
#endif

#ifdef HAVE_GETUID
static char posix_getuid__doc__[] =
"getuid() -> uid\n\
Return the current process's user id.";

static PyObject *
posix_getuid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":getuid"))
		return NULL;
	return PyInt_FromLong((long)getuid());
}
#endif


#ifdef HAVE_KILL
static char posix_kill__doc__[] =
"kill(pid, sig) -> None\n\
Kill a process with a signal.";

static PyObject *
posix_kill(self, args)
	PyObject *self;
	PyObject *args;
{
	int pid, sig;
	if (!PyArg_ParseTuple(args, "ii:kill", &pid, &sig))
		return NULL;
#if defined(PYOS_OS2)
    if (sig == XCPT_SIGNAL_INTR || sig == XCPT_SIGNAL_BREAK) {
        APIRET rc;
        if ((rc = DosSendSignalException(pid, sig)) != NO_ERROR)
            return os2_error(rc);

    } else if (sig == XCPT_SIGNAL_KILLPROC) {
        APIRET rc;
        if ((rc = DosKillProcess(DKP_PROCESS, pid)) != NO_ERROR)
            return os2_error(rc);

    } else
        return NULL; /* Unrecognized Signal Requested */
#else
	if (kill(pid, sig) == -1)
		return posix_error();
#endif
	Py_INCREF(Py_None);
	return Py_None;
}
#endif

#ifdef HAVE_PLOCK

#ifdef HAVE_SYS_LOCK_H
#include <sys/lock.h>
#endif

static char posix_plock__doc__[] =
"plock(op) -> None\n\
Lock program segments into memory.";

static PyObject *
posix_plock(self, args)
	PyObject *self;
	PyObject *args;
{
	int op;
	if (!PyArg_ParseTuple(args, "i:plock", &op))
		return NULL;
	if (plock(op) == -1)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif


#ifdef HAVE_POPEN
static char posix_popen__doc__[] =
"popen(command [, mode='r' [, bufsize]]) -> pipe\n\
Open a pipe to/from a command returning a file object.";

#if defined(PYOS_OS2)
static int
async_system(const char *command)
{
    char        *p, errormsg[256], args[1024];
    RESULTCODES  rcodes;
    APIRET       rc;
    char        *shell = getenv("COMSPEC");
    if (!shell)
        shell = "cmd";

    strcpy(args, shell);
    p = &args[ strlen(args)+1 ];
    strcpy(p, "/c ");
    strcat(p, command);
    p += strlen(p) + 1;
    *p = '\0';

    rc = DosExecPgm(errormsg, sizeof(errormsg),
                    EXEC_ASYNC, /* Execute Async w/o Wait for Results */
                    args,
                    NULL,       /* Inherit Parent's Environment */
                    &rcodes, shell);
    return rc;
}

static FILE *
popen(const char *command, const char *mode, int pipesize, int *err)
{
    HFILE    rhan, whan;
    FILE    *retfd = NULL;
    APIRET   rc = DosCreatePipe(&rhan, &whan, pipesize);

    if (rc != NO_ERROR) {
	*err = rc;
        return NULL; /* ERROR - Unable to Create Anon Pipe */
    }

    if (strchr(mode, 'r') != NULL) { /* Treat Command as a Data Source */
        int oldfd = dup(1);      /* Save STDOUT Handle in Another Handle */

        DosEnterCritSec();      /* Stop Other Threads While Changing Handles */
        close(1);                /* Make STDOUT Available for Reallocation */

        if (dup2(whan, 1) == 0) {      /* Connect STDOUT to Pipe Write Side */
            DosClose(whan);            /* Close Now-Unused Pipe Write Handle */

            if (async_system(command) == NO_ERROR)
                retfd = fdopen(rhan, mode); /* And Return Pipe Read Handle */
        }

        dup2(oldfd, 1);          /* Reconnect STDOUT to Original Handle */
        DosExitCritSec();        /* Now Allow Other Threads to Run */

        close(oldfd);            /* And Close Saved STDOUT Handle */
        return retfd;            /* Return fd of Pipe or NULL if Error */

    } else if (strchr(mode, 'w')) { /* Treat Command as a Data Sink */
        int oldfd = dup(0);      /* Save STDIN Handle in Another Handle */

        DosEnterCritSec();      /* Stop Other Threads While Changing Handles */
        close(0);                /* Make STDIN Available for Reallocation */

        if (dup2(rhan, 0) == 0)     { /* Connect STDIN to Pipe Read Side */
            DosClose(rhan);           /* Close Now-Unused Pipe Read Handle */

            if (async_system(command) == NO_ERROR)
                retfd = fdopen(whan, mode); /* And Return Pipe Write Handle */
        }

        dup2(oldfd, 0);          /* Reconnect STDIN to Original Handle */
        DosExitCritSec();        /* Now Allow Other Threads to Run */

        close(oldfd);            /* And Close Saved STDIN Handle */
        return retfd;            /* Return fd of Pipe or NULL if Error */

    } else {
	*err = ERROR_INVALID_ACCESS;
        return NULL; /* ERROR - Invalid Mode (Neither Read nor Write) */
    }
}

static PyObject *
posix_popen(self, args)
	PyObject *self;
	PyObject *args;
{
	char *name;
	char *mode = "r";
	int   err, bufsize = -1;
	FILE *fp;
	PyObject *f;
	if (!PyArg_ParseTuple(args, "s|si:popen", &name, &mode, &bufsize))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	fp = popen(name, mode, (bufsize > 0) ? bufsize : 4096, &err);
	Py_END_ALLOW_THREADS
	if (fp == NULL)
		return os2_error(err);

	f = PyFile_FromFile(fp, name, mode, fclose);
	if (f != NULL)
		PyFile_SetBufSize(f, bufsize);
	return f;
}

#else
static PyObject *
posix_popen(self, args)
	PyObject *self;
	PyObject *args;
{
	char *name;
	char *mode = "r";
	int bufsize = -1;
	FILE *fp;
	PyObject *f;
	if (!PyArg_ParseTuple(args, "s|si:popen", &name, &mode, &bufsize))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	fp = popen(name, mode);
	Py_END_ALLOW_THREADS
	if (fp == NULL)
		return posix_error();
	f = PyFile_FromFile(fp, name, mode, pclose);
	if (f != NULL)
		PyFile_SetBufSize(f, bufsize);
	return f;
}
#endif

#endif /* HAVE_POPEN */


#ifdef HAVE_SETUID
static char posix_setuid__doc__[] =
"setuid(uid) -> None\n\
Set the current process's user id.";
static PyObject *
posix_setuid(self, args)
	PyObject *self;
	PyObject *args;
{
	int uid;
	if (!PyArg_ParseTuple(args, "i:setuid", &uid))
		return NULL;
	if (setuid(uid) < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif /* HAVE_SETUID */


#ifdef HAVE_SETGID
static char posix_setgid__doc__[] =
"setgid(gid) -> None\n\
Set the current process's group id.";

static PyObject *
posix_setgid(self, args)
	PyObject *self;
	PyObject *args;
{
	int gid;
	if (!PyArg_ParseTuple(args, "i:setgid", &gid))
		return NULL;
	if (setgid(gid) < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif /* HAVE_SETGID */


#ifdef HAVE_WAITPID
static char posix_waitpid__doc__[] =
"waitpid(pid, options) -> (pid, status)\n\
Wait for completion of a give child process.";

static PyObject *
posix_waitpid(self, args)
	PyObject *self;
	PyObject *args;
{
	int pid, options;
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;

	if (!PyArg_ParseTuple(args, "ii:waitpid", &pid, &options))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
#ifdef NeXT
	pid = wait4(pid, &status, options, NULL);
#else
	pid = waitpid(pid, &status, options);
#endif
	Py_END_ALLOW_THREADS
	if (pid == -1)
		return posix_error();
	else
		return Py_BuildValue("ii", pid, status_i);
}
#endif /* HAVE_WAITPID */


#ifdef HAVE_WAIT
static char posix_wait__doc__[] =
"wait() -> (pid, status)\n\
Wait for completion of a child process.";

static PyObject *
posix_wait(self, args)
	PyObject *self;
	PyObject *args;
{
	int pid;
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
        if (!PyArg_ParseTuple(args, ":wait"))
                return NULL;
	status_i = 0;
	Py_BEGIN_ALLOW_THREADS
	pid = wait(&status);
	Py_END_ALLOW_THREADS
	if (pid == -1)
		return posix_error();
	else
		return Py_BuildValue("ii", pid, status_i);
#undef status_i
}
#endif


static char posix_lstat__doc__[] =
"lstat(path) -> (mode,ino,dev,nlink,uid,gid,size,atime,mtime,ctime)\n\
Like stat(path), but do not follow symbolic links.";

static PyObject *
posix_lstat(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef HAVE_LSTAT
	return posix_do_stat(self, args, "s:lstat", lstat);
#else /* !HAVE_LSTAT */
	return posix_do_stat(self, args, "s:lstat", stat);
#endif /* !HAVE_LSTAT */
}


#ifdef HAVE_READLINK
static char posix_readlink__doc__[] =
"readlink(path) -> path\n\
Return a string representing the path to which the symbolic link points.";

static PyObject *
posix_readlink(self, args)
	PyObject *self;
	PyObject *args;
{
	char buf[MAXPATHLEN];
	char *path;
	int n;
	if (!PyArg_ParseTuple(args, "s:readlink", &path))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	n = readlink(path, buf, (int) sizeof buf);
	Py_END_ALLOW_THREADS
	if (n < 0)
		return posix_error_with_filename(path);
	return PyString_FromStringAndSize(buf, n);
}
#endif /* HAVE_READLINK */


#ifdef HAVE_SYMLINK
static char posix_symlink__doc__[] =
"symlink(src, dst) -> None\n\
Create a symbolic link.";

static PyObject *
posix_symlink(self, args)
	PyObject *self;
	PyObject *args;
{
	return posix_2str(args, "ss:symlink", symlink);
}
#endif /* HAVE_SYMLINK */


#ifdef HAVE_TIMES
#ifndef HZ
#define HZ 60 /* Universal constant :-) */
#endif /* HZ */
	
#if defined(PYCC_VACPP) && defined(PYOS_OS2)
static long
system_uptime()
{
    ULONG     value = 0;

    Py_BEGIN_ALLOW_THREADS
    DosQuerySysInfo(QSV_MS_COUNT, QSV_MS_COUNT, &value, sizeof(value));
    Py_END_ALLOW_THREADS

    return value;
}

static PyObject *
posix_times(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":times"))
		return NULL;

    /* Currently Only Uptime is Provided -- Others Later */
	return Py_BuildValue("ddddd",
			     (double)0 /* t.tms_utime / HZ */,
			     (double)0 /* t.tms_stime / HZ */,
			     (double)0 /* t.tms_cutime / HZ */,
			     (double)0 /* t.tms_cstime / HZ */,
			     (double)system_uptime() / 1000);
}
#else /* not OS2 */
static PyObject *
posix_times(self, args)
	PyObject *self;
	PyObject *args;
{
	struct tms t;
	clock_t c;
	if (!PyArg_ParseTuple(args, ":times"))
		return NULL;
	errno = 0;
	c = times(&t);
	if (c == (clock_t) -1)
		return posix_error();
	return Py_BuildValue("ddddd",
			     (double)t.tms_utime / HZ,
			     (double)t.tms_stime / HZ,
			     (double)t.tms_cutime / HZ,
			     (double)t.tms_cstime / HZ,
			     (double)c / HZ);
}
#endif /* not OS2 */
#endif /* HAVE_TIMES */


#ifdef MS_WIN32
#define HAVE_TIMES	/* so the method table will pick it up */
static PyObject *
posix_times(self, args)
	PyObject *self;
	PyObject *args;
{
	FILETIME create, exit, kernel, user;
	HANDLE hProc;
	if (!PyArg_ParseTuple(args, ":times"))
		return NULL;
	hProc = GetCurrentProcess();
	GetProcessTimes(hProc, &create, &exit, &kernel, &user);
	/* The fields of a FILETIME structure are the hi and lo part
	   of a 64-bit value expressed in 100 nanosecond units.
	   1e7 is one second in such units; 1e-7 the inverse.
	   429.4967296 is 2**32 / 1e7 or 2**32 * 1e-7.
	*/
	return Py_BuildValue(
		"ddddd",
		(double)(kernel.dwHighDateTime*429.4967296 +
		         kernel.dwLowDateTime*1e-7),
		(double)(user.dwHighDateTime*429.4967296 +
		         user.dwLowDateTime*1e-7),
		(double)0,
		(double)0,
		(double)0);
}
#endif /* MS_WIN32 */

#ifdef HAVE_TIMES
static char posix_times__doc__[] =
"times() -> (utime, stime, cutime, cstime, elapsed_time)\n\
Return a tuple of floating point numbers indicating process times.";
#endif


#ifdef HAVE_SETSID
static char posix_setsid__doc__[] =
"setsid() -> None\n\
Call the system call setsid().";

static PyObject *
posix_setsid(self, args)
	PyObject *self;
	PyObject *args;
{
	if (!PyArg_ParseTuple(args, ":setsid"))
		return NULL;
	if (setsid() < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif /* HAVE_SETSID */

#ifdef HAVE_SETPGID
static char posix_setpgid__doc__[] =
"setpgid(pid, pgrp) -> None\n\
Call the system call setpgid().";

static PyObject *
posix_setpgid(self, args)
	PyObject *self;
	PyObject *args;
{
	int pid, pgrp;
	if (!PyArg_ParseTuple(args, "ii:setpgid", &pid, &pgrp))
		return NULL;
	if (setpgid(pid, pgrp) < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif /* HAVE_SETPGID */


#ifdef HAVE_TCGETPGRP
static char posix_tcgetpgrp__doc__[] =
"tcgetpgrp(fd) -> pgid\n\
Return the process group associated with the terminal given by a fd.";

static PyObject *
posix_tcgetpgrp(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, pgid;
	if (!PyArg_ParseTuple(args, "i:tcgetpgrp", &fd))
		return NULL;
	pgid = tcgetpgrp(fd);
	if (pgid < 0)
		return posix_error();
	return PyInt_FromLong((long)pgid);
}
#endif /* HAVE_TCGETPGRP */


#ifdef HAVE_TCSETPGRP
static char posix_tcsetpgrp__doc__[] =
"tcsetpgrp(fd, pgid) -> None\n\
Set the process group associated with the terminal given by a fd.";

static PyObject *
posix_tcsetpgrp(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, pgid;
	if (!PyArg_ParseTuple(args, "ii:tcsetpgrp", &fd, &pgid))
		return NULL;
	if (tcsetpgrp(fd, pgid) < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif /* HAVE_TCSETPGRP */

/* Functions acting on file descriptors */

static char posix_open__doc__[] =
"open(filename, flag [, mode=0777]) -> fd\n\
Open a file (for low level IO).";

static PyObject *
posix_open(self, args)
	PyObject *self;
	PyObject *args;
{
	char *file;
	int flag;
	int mode = 0777;
	int fd;
	if (!PyArg_ParseTuple(args, "si|i", &file, &flag, &mode))
		return NULL;

	Py_BEGIN_ALLOW_THREADS
	fd = open(file, flag, mode);
	Py_END_ALLOW_THREADS
	if (fd < 0)
		return posix_error_with_filename(file);
	return PyInt_FromLong((long)fd);
}


static char posix_close__doc__[] =
"close(fd) -> None\n\
Close a file descriptor (for low level IO).";

static PyObject *
posix_close(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, res;
	if (!PyArg_ParseTuple(args, "i:close", &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = close(fd);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}


static char posix_dup__doc__[] =
"dup(fd) -> fd2\n\
Return a duplicate of a file descriptor.";

static PyObject *
posix_dup(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd;
	if (!PyArg_ParseTuple(args, "i:dup", &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	fd = dup(fd);
	Py_END_ALLOW_THREADS
	if (fd < 0)
		return posix_error();
	return PyInt_FromLong((long)fd);
}


static char posix_dup2__doc__[] =
"dup2(fd, fd2) -> None\n\
Duplicate file descriptor.";

static PyObject *
posix_dup2(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, fd2, res;
	if (!PyArg_ParseTuple(args, "ii:dup2", &fd, &fd2))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = dup2(fd, fd2);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}


static char posix_lseek__doc__[] =
"lseek(fd, pos, how) -> newpos\n\
Set the current position of a file descriptor.";

static PyObject *
posix_lseek(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, how;
	off_t pos, res;
	PyObject *posobj;
	if (!PyArg_ParseTuple(args, "iOi:lseek", &fd, &posobj, &how))
		return NULL;
#ifdef SEEK_SET
	/* Turn 0, 1, 2 into SEEK_{SET,CUR,END} */
	switch (how) {
	case 0: how = SEEK_SET; break;
	case 1: how = SEEK_CUR; break;
	case 2: how = SEEK_END; break;
	}
#endif /* SEEK_END */

#if !defined(HAVE_LARGEFILE_SUPPORT)
	pos = PyInt_AsLong(posobj);
#else
	pos = PyLong_Check(posobj) ?
		PyLong_AsLongLong(posobj) : PyInt_AsLong(posobj);
#endif
	if (PyErr_Occurred())
		return NULL;

	Py_BEGIN_ALLOW_THREADS
	res = lseek(fd, pos, how);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();

#if !defined(HAVE_LARGEFILE_SUPPORT)
	return PyInt_FromLong(res);
#else
	return PyLong_FromLongLong(res);
#endif
}


static char posix_read__doc__[] =
"read(fd, buffersize) -> string\n\
Read a file descriptor.";

static PyObject *
posix_read(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, size, n;
	PyObject *buffer;
	if (!PyArg_ParseTuple(args, "ii:read", &fd, &size))
		return NULL;
	buffer = PyString_FromStringAndSize((char *)NULL, size);
	if (buffer == NULL)
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	n = read(fd, PyString_AsString(buffer), size);
	Py_END_ALLOW_THREADS
	if (n < 0) {
		Py_DECREF(buffer);
		return posix_error();
	}
	if (n != size)
		_PyString_Resize(&buffer, n);
	return buffer;
}


static char posix_write__doc__[] =
"write(fd, string) -> byteswritten\n\
Write a string to a file descriptor.";

static PyObject *
posix_write(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, size;
	char *buffer;
	if (!PyArg_ParseTuple(args, "is#:write", &fd, &buffer, &size))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	size = write(fd, buffer, size);
	Py_END_ALLOW_THREADS
	if (size < 0)
		return posix_error();
	return PyInt_FromLong((long)size);
}


static char posix_fstat__doc__[]=
"fstat(fd) -> (mode, ino, dev, nlink, uid, gid, size, atime, mtime, ctime)\n\
Like stat(), but for an open file descriptor.";

static PyObject *
posix_fstat(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd;
	struct stat st;
	int res;
	if (!PyArg_ParseTuple(args, "i:fstat", &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = fstat(fd, &st);
	Py_END_ALLOW_THREADS
	if (res != 0)
		return posix_error();
#if !defined(HAVE_LARGEFILE_SUPPORT)
	return Py_BuildValue("(llllllllll)",
			     (long)st.st_mode,
			     (long)st.st_ino,
			     (long)st.st_dev,
			     (long)st.st_nlink,
			     (long)st.st_uid,
			     (long)st.st_gid,
			     (long)st.st_size,
			     (long)st.st_atime,
			     (long)st.st_mtime,
			     (long)st.st_ctime);
#else
	return Py_BuildValue("(lLllllLlll)",
			     (long)st.st_mode,
			     (LONG_LONG)st.st_ino,
			     (long)st.st_dev,
			     (long)st.st_nlink,
			     (long)st.st_uid,
			     (long)st.st_gid,
			     (LONG_LONG)st.st_size,
			     (long)st.st_atime,
			     (long)st.st_mtime,
			     (long)st.st_ctime);
#endif
}


static char posix_fdopen__doc__[] =
"fdopen(fd, [, mode='r' [, bufsize]]) -> file_object\n\
Return an open file object connected to a file descriptor.";

static PyObject *
posix_fdopen(self, args)
	PyObject *self;
	PyObject *args;
{
	extern int fclose Py_PROTO((FILE *));
	int fd;
	char *mode = "r";
	int bufsize = -1;
	FILE *fp;
	PyObject *f;
	if (!PyArg_ParseTuple(args, "i|si", &fd, &mode, &bufsize))
		return NULL;

	Py_BEGIN_ALLOW_THREADS
	fp = fdopen(fd, mode);
	Py_END_ALLOW_THREADS
	if (fp == NULL)
		return posix_error();
	f = PyFile_FromFile(fp, "(fdopen)", mode, fclose);
	if (f != NULL)
		PyFile_SetBufSize(f, bufsize);
	return f;
}


#ifdef HAVE_PIPE
static char posix_pipe__doc__[] =
"pipe() -> (read_end, write_end)\n\
Create a pipe.";

static PyObject *
posix_pipe(self, args)
	PyObject *self;
	PyObject *args;
{
#if defined(PYOS_OS2)
    HFILE read, write;
    APIRET rc;

    if (!PyArg_ParseTuple(args, ":pipe"))
        return NULL;

	Py_BEGIN_ALLOW_THREADS
    rc = DosCreatePipe( &read, &write, 4096);
	Py_END_ALLOW_THREADS
    if (rc != NO_ERROR)
        return os2_error(rc);

    return Py_BuildValue("(ii)", read, write);
#else
#if !defined(MS_WIN32)
	int fds[2];
	int res;
	if (!PyArg_ParseTuple(args, ":pipe"))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = pipe(fds);
	Py_END_ALLOW_THREADS
	if (res != 0)
		return posix_error();
	return Py_BuildValue("(ii)", fds[0], fds[1]);
#else /* MS_WIN32 */
	HANDLE read, write;
	int read_fd, write_fd;
	BOOL ok;
	if (!PyArg_ParseTuple(args, ":pipe"))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	ok = CreatePipe(&read, &write, NULL, 0);
	Py_END_ALLOW_THREADS
	if (!ok)
		return posix_error();
	read_fd = _open_osfhandle((long)read, 0);
	write_fd = _open_osfhandle((long)write, 1);
	return Py_BuildValue("(ii)", read_fd, write_fd);
#endif /* MS_WIN32 */
#endif
}
#endif  /* HAVE_PIPE */


#ifdef HAVE_MKFIFO
static char posix_mkfifo__doc__[] =
"mkfifo(file, [, mode=0666]) -> None\n\
Create a FIFO (a POSIX named pipe).";

static PyObject *
posix_mkfifo(self, args)
	PyObject *self;
	PyObject *args;
{
	char *file;
	int mode = 0666;
	int res;
	if (!PyArg_ParseTuple(args, "s|i:mkfifo", &file, &mode))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = mkfifo(file, mode);
	Py_END_ALLOW_THREADS
	if (res < 0)
		return posix_error();
	Py_INCREF(Py_None);
	return Py_None;
}
#endif


#ifdef HAVE_FTRUNCATE
static char posix_ftruncate__doc__[] =
"ftruncate(fd, length) -> None\n\
Truncate a file to a specified length.";

static PyObject *
posix_ftruncate(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	int fd;
	off_t length;
	int res;
	PyObject *lenobj;

	if (!PyArg_ParseTuple(args, "iO:ftruncate", &fd, &lenobj))
		return NULL;

#if !defined(HAVE_LARGEFILE_SUPPORT)
	length = PyInt_AsLong(lenobj);
#else
	length = PyLong_Check(lenobj) ?
		PyLong_AsLongLong(lenobj) : PyInt_AsLong(lenobj);
#endif
	if (PyErr_Occurred())
		return NULL;

	Py_BEGIN_ALLOW_THREADS
	res = ftruncate(fd, length);
	Py_END_ALLOW_THREADS
	if (res < 0) {
		PyErr_SetFromErrno(PyExc_IOError);
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}
#endif

#ifdef NeXT
#define HAVE_PUTENV
/* Steve Spicklemire got this putenv from NeXTAnswers */
static int
putenv(char *newval)
{
	extern char **environ;

	static int firstTime = 1;
	char **ep;
	char *cp;
	int esiz;
	char *np;

	if (!(np = strchr(newval, '=')))
		return 1;
	*np = '\0';

	/* look it up */
	for (ep=environ ; *ep ; ep++)
	{
		/* this should always be true... */
		if (cp = strchr(*ep, '='))
		{
			*cp = '\0';
			if (!strcmp(*ep, newval))
			{
				/* got it! */
				*cp = '=';
				break;
			}
			*cp = '=';
		}
		else
		{
			*np = '=';
			return 1;
		}
	}

	*np = '=';
	if (*ep)
	{
		/* the string was already there:
		   just replace it with the new one */
		*ep = newval;
		return 0;
	}

	/* expand environ by one */
	for (esiz=2, ep=environ ; *ep ; ep++)
		esiz++;
	if (firstTime)
	{
		char **epp;
		char **newenv;
		if (!(newenv = malloc(esiz * sizeof(char *))))
			return 1;
   
		for (ep=environ, epp=newenv ; *ep ;)
			*epp++ = *ep++;
		*epp++ = newval;
		*epp = (char *) 0;
		environ = newenv;
	}
	else
	{
		if (!(environ = realloc(environ, esiz * sizeof(char *))))
			return 1;
		environ[esiz - 2] = newval;
		environ[esiz - 1] = (char *) 0;
		firstTime = 0;
	}

	return 0;
}
#endif /* NeXT */


#ifdef HAVE_PUTENV
static char posix_putenv__doc__[] =
"putenv(key, value) -> None\n\
Change or add an environment variable.";

#ifdef __BEOS__
/* We have putenv(), but not in the headers (as of PR2). - [cjh] */
int putenv( const char *str );
#endif

/* Save putenv() parameters as values here, so we can collect them when they
 * get re-set with another call for the same key. */
static PyObject *posix_putenv_garbage;

static PyObject * 
posix_putenv(self, args)
	PyObject *self;
	PyObject *args;
{
        char *s1, *s2;
        char *new;
	PyObject *newstr;

	if (!PyArg_ParseTuple(args, "ss:putenv", &s1, &s2))
		return NULL;

#if defined(PYOS_OS2)
    if (stricmp(s1, "BEGINLIBPATH") == 0) {
        APIRET rc;

        if (strlen(s2) == 0)  /* If New Value is an Empty String */
            s2 = NULL;        /* Then OS/2 API Wants a NULL to Undefine It */

        rc = DosSetExtLIBPATH(s2, BEGIN_LIBPATH);
        if (rc != NO_ERROR)
            return os2_error(rc);

    } else if (stricmp(s1, "ENDLIBPATH") == 0) {
        APIRET rc;

        if (strlen(s2) == 0)  /* If New Value is an Empty String */
            s2 = NULL;        /* Then OS/2 API Wants a NULL to Undefine It */

        rc = DosSetExtLIBPATH(s2, END_LIBPATH);
        if (rc != NO_ERROR)
            return os2_error(rc);
    } else {
#endif

	/* XXX This can leak memory -- not easy to fix :-( */
	newstr = PyString_FromStringAndSize(NULL, strlen(s1) + strlen(s2) + 2);
	if (newstr == NULL)
		return PyErr_NoMemory();
	new = PyString_AS_STRING(newstr);
	(void) sprintf(new, "%s=%s", s1, s2);
	if (putenv(new)) {
                posix_error();
                return NULL;
	}
	/* Install the first arg and newstr in posix_putenv_garbage;
	 * this will cause previous value to be collected.  This has to
	 * happen after the real putenv() call because the old value
	 * was still accessible until then. */
	if (PyDict_SetItem(posix_putenv_garbage,
			   PyTuple_GET_ITEM(args, 0), newstr)) {
		/* really not much we can do; just leak */
		PyErr_Clear();
	}
	else {
		Py_DECREF(newstr);
	}

#if defined(PYOS_OS2)
    }
#endif
	Py_INCREF(Py_None);
        return Py_None;
}
#endif /* putenv */

#ifdef HAVE_STRERROR
static char posix_strerror__doc__[] =
"strerror(code) -> string\n\
Translate an error code to a message string.";

PyObject *
posix_strerror(self, args)
	PyObject *self;
	PyObject *args;
{
	int code;
	char *message;
	if (!PyArg_ParseTuple(args, "i:strerror", &code))
		return NULL;
	message = strerror(code);
	if (message == NULL) {
		PyErr_SetString(PyExc_ValueError,
				"strerror code out of range");
		return NULL;
	}
	return PyString_FromString(message);
}
#endif /* strerror */


#ifdef HAVE_SYS_WAIT_H

#ifdef WIFSTOPPED
static char posix_WIFSTOPPED__doc__[] =
"WIFSTOPPED(status) -> Boolean\n\
Return true if the process returning 'status' was stopped.";

static PyObject *
posix_WIFSTOPPED(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WIFSTOPPED", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WIFSTOPPED(status));
#undef status_i
}
#endif /* WIFSTOPPED */

#ifdef WIFSIGNALED
static char posix_WIFSIGNALED__doc__[] =
"WIFSIGNALED(status) -> Boolean\n\
Return true if the process returning 'status' was terminated by a signal.";

static PyObject *
posix_WIFSIGNALED(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WIFSIGNALED", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WIFSIGNALED(status));
#undef status_i
}
#endif /* WIFSIGNALED */

#ifdef WIFEXITED
static char posix_WIFEXITED__doc__[] =
"WIFEXITED(status) -> Boolean\n\
Return true if the process returning 'status' exited using the exit()\n\
system call.";

static PyObject *
posix_WIFEXITED(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WIFEXITED", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WIFEXITED(status));
#undef status_i
}
#endif /* WIFEXITED */

#ifdef WEXITSTATUS
static char posix_WEXITSTATUS__doc__[] =
"WEXITSTATUS(status) -> integer\n\
Return the process return code from 'status'.";

static PyObject *
posix_WEXITSTATUS(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WEXITSTATUS", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WEXITSTATUS(status));
#undef status_i
}
#endif /* WEXITSTATUS */

#ifdef WTERMSIG
static char posix_WTERMSIG__doc__[] =
"WTERMSIG(status) -> integer\n\
Return the signal that terminated the process that provided the 'status'\n\
value.";

static PyObject *
posix_WTERMSIG(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WTERMSIG", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WTERMSIG(status));
#undef status_i
}
#endif /* WTERMSIG */

#ifdef WSTOPSIG
static char posix_WSTOPSIG__doc__[] =
"WSTOPSIG(status) -> integer\n\
Return the signal that stopped the process that provided the 'status' value.";

static PyObject *
posix_WSTOPSIG(self, args)
	PyObject *self;
	PyObject *args;
{
#ifdef UNION_WAIT
	union wait status;
#define status_i (status.w_status)
#else
	int status;
#define status_i status
#endif
	status_i = 0;
   
	if (!PyArg_ParseTuple(args, "i:WSTOPSIG", &status_i))
	{
		return NULL;
	}
   
	return Py_BuildValue("i", WSTOPSIG(status));
#undef status_i
}
#endif /* WSTOPSIG */

#endif /* HAVE_SYS_WAIT_H */


#if defined(HAVE_FSTATVFS)
#ifdef _SCO_DS
/* SCO OpenServer 5.0 and later requires _SVID3 before it reveals the
   needed definitions in sys/statvfs.h */
#define _SVID3
#endif
#include <sys/statvfs.h>

static char posix_fstatvfs__doc__[] =
"fstatvfs(fd) -> \n\
 (bsize, frsize, blocks, bfree, bavail, files, ffree, favail, flag, namemax)\n\
Perform an fstatvfs system call on the given fd.";

static PyObject *
posix_fstatvfs(self, args)
	PyObject *self;
	PyObject *args;
{
	int fd, res;
	struct statvfs st;
	if (!PyArg_ParseTuple(args, "i:fstatvfs", &fd))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = fstatvfs(fd, &st);
	Py_END_ALLOW_THREADS
	if (res != 0)
		return posix_error();
#if !defined(HAVE_LARGEFILE_SUPPORT)
	return Py_BuildValue("(llllllllll)",
		    (long) st.f_bsize,
		    (long) st.f_frsize,
		    (long) st.f_blocks,
		    (long) st.f_bfree,
		    (long) st.f_bavail,
		    (long) st.f_files,
		    (long) st.f_ffree,
		    (long) st.f_favail,
		    (long) st.f_flag,
		    (long) st.f_namemax);
#else
	return Py_BuildValue("(llLLLLLLll)",
		    (long) st.f_bsize,
		    (long) st.f_frsize,
		    (LONG_LONG) st.f_blocks,
		    (LONG_LONG) st.f_bfree,
		    (LONG_LONG) st.f_bavail,
		    (LONG_LONG) st.f_files,
		    (LONG_LONG) st.f_ffree,
		    (LONG_LONG) st.f_favail,
		    (long) st.f_flag,
		    (long) st.f_namemax);
#endif
}
#endif /* HAVE_FSTATVFS */


#if defined(HAVE_STATVFS)
#include <sys/statvfs.h>

static char posix_statvfs__doc__[] =
"statvfs(path) -> \n\
 (bsize, frsize, blocks, bfree, bavail, files, ffree, favail, flag, namemax)\n\
Perform a statvfs system call on the given path.";

static PyObject *
posix_statvfs(self, args)
	PyObject *self;
	PyObject *args;
{
	char *path;
	int res;
	struct statvfs st;
	if (!PyArg_ParseTuple(args, "s:statvfs", &path))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	res = statvfs(path, &st);
	Py_END_ALLOW_THREADS
	if (res != 0)
		return posix_error_with_filename(path);
#if !defined(HAVE_LARGEFILE_SUPPORT)
	return Py_BuildValue("(llllllllll)",
		    (long) st.f_bsize,
		    (long) st.f_frsize,
		    (long) st.f_blocks,
		    (long) st.f_bfree,
		    (long) st.f_bavail,
		    (long) st.f_files,
		    (long) st.f_ffree,
		    (long) st.f_favail,
		    (long) st.f_flag,
		    (long) st.f_namemax);
#else	/* HAVE_LARGEFILE_SUPPORT */
	return Py_BuildValue("(llLLLLLLll)",
		    (long) st.f_bsize,
		    (long) st.f_frsize,
		    (LONG_LONG) st.f_blocks,
		    (LONG_LONG) st.f_bfree,
		    (LONG_LONG) st.f_bavail,
		    (LONG_LONG) st.f_files,
		    (LONG_LONG) st.f_ffree,
		    (LONG_LONG) st.f_favail,
		    (long) st.f_flag,
		    (long) st.f_namemax);
#endif
}
#endif /* HAVE_STATVFS */


#ifdef HAVE_TEMPNAM
static char posix_tempnam__doc__[] = "\
tempnam([dir[, prefix]]) -> string\n\
Return a unique name for a temporary file.\n\
The directory and a short may be specified as strings; they may be omitted\n\
or None if not needed.";

static PyObject *
posix_tempnam(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;
    char *dir = NULL;
    char *pfx = NULL;
    char *name;

    if (!PyArg_ParseTuple(args, "|zz:tempnam", &dir, &pfx))
        return NULL;
    name = tempnam(dir, pfx);
    if (name == NULL)
        return PyErr_NoMemory();
    result = PyString_FromString(name);
    free(name);
    return result;
}
#endif


#ifdef HAVE_TMPFILE
static char posix_tmpfile__doc__[] = "\
tmpfile() -> file object\n\
Create a temporary file with no directory entries.";

static PyObject *
posix_tmpfile(self, args)
     PyObject *self;
     PyObject *args;
{
    FILE *fp;

    if (!PyArg_ParseTuple(args, ":tmpfile"))
        return NULL;
    fp = tmpfile();
    if (fp == NULL)
        return posix_error();
    return PyFile_FromFile(fp, "<tmpfile>", "w+", fclose);
}
#endif


#ifdef HAVE_TMPNAM
static char posix_tmpnam__doc__[] = "\
tmpnam() -> string\n\
Return a unique name for a temporary file.";

static PyObject *
posix_tmpnam(self, args)
     PyObject *self;
     PyObject *args;
{
    char buffer[L_tmpnam];
    char *name;

    if (!PyArg_ParseTuple(args, ":tmpnam"))
        return NULL;
#ifdef USE_TMPNAM_R
    name = tmpnam_r(buffer);
#else
    name = tmpnam(buffer);
#endif
    if (name == NULL) {
        PyErr_SetObject(PyExc_OSError,
                        Py_BuildValue("is", 0,
#ifdef USE_TMPNAM_R
                                      "unexpected NULL from tmpnam_r"
#else
                                      "unexpected NULL from tmpnam"
#endif
                                      ));
        return NULL;
    }
    return PyString_FromString(buffer);
}
#endif


/* This is used for fpathconf(), pathconf(), confstr() and sysconf().
 * It maps strings representing configuration variable names to
 * integer values, allowing those functions to be called with the
 * magic names instead of poluting the module's namespace with tons of
 * rarely-used constants.  There are three separate tables that use
 * these definitions.
 *
 * This code is always included, even if none of the interfaces that
 * need it are included.  The #if hackery needed to avoid it would be
 * sufficiently pervasive that it's not worth the loss of readability.
 */
struct constdef {
    char *name;
    long value;
};

static int
conv_confname(arg, valuep, table, tablesize)
     PyObject *arg;
     int *valuep;
     struct constdef *table;
     size_t tablesize;
{
    if (PyInt_Check(arg)) {
        *valuep = PyInt_AS_LONG(arg);
        return 1;
    }
    if (PyString_Check(arg)) {
        /* look up the value in the table using a binary search */
        int lo = 0;
        int hi = tablesize;
        int cmp, mid;
        char *confname = PyString_AS_STRING(arg);
        while (lo < hi) {
            mid = (lo + hi) / 2;
            cmp = strcmp(confname, table[mid].name);
            if (cmp < 0)
                hi = mid;
            else if (cmp > 0)
                lo = mid + 1;
            else {
                *valuep = table[mid].value;
                return 1;
            }
        }
        PyErr_SetString(PyExc_ValueError, "unrecognized configuration name");
    }
    else
        PyErr_SetString(PyExc_TypeError,
                        "configuration names must be strings or integers");
    return 0;
}


#if defined(HAVE_FPATHCONF) || defined(HAVE_PATHCONF)
static struct constdef  posix_constants_pathconf[] = {
#ifdef _PC_ABI_AIO_XFER_MAX
    {"PC_ABI_AIO_XFER_MAX",	_PC_ABI_AIO_XFER_MAX},
#endif
#ifdef _PC_ABI_ASYNC_IO
    {"PC_ABI_ASYNC_IO",	_PC_ABI_ASYNC_IO},
#endif
#ifdef _PC_ASYNC_IO
    {"PC_ASYNC_IO",	_PC_ASYNC_IO},
#endif
#ifdef _PC_CHOWN_RESTRICTED
    {"PC_CHOWN_RESTRICTED",	_PC_CHOWN_RESTRICTED},
#endif
#ifdef _PC_FILESIZEBITS
    {"PC_FILESIZEBITS",	_PC_FILESIZEBITS},
#endif
#ifdef _PC_LAST
    {"PC_LAST",	_PC_LAST},
#endif
#ifdef _PC_LINK_MAX
    {"PC_LINK_MAX",	_PC_LINK_MAX},
#endif
#ifdef _PC_MAX_CANON
    {"PC_MAX_CANON",	_PC_MAX_CANON},
#endif
#ifdef _PC_MAX_INPUT
    {"PC_MAX_INPUT",	_PC_MAX_INPUT},
#endif
#ifdef _PC_NAME_MAX
    {"PC_NAME_MAX",	_PC_NAME_MAX},
#endif
#ifdef _PC_NO_TRUNC
    {"PC_NO_TRUNC",	_PC_NO_TRUNC},
#endif
#ifdef _PC_PATH_MAX
    {"PC_PATH_MAX",	_PC_PATH_MAX},
#endif
#ifdef _PC_PIPE_BUF
    {"PC_PIPE_BUF",	_PC_PIPE_BUF},
#endif
#ifdef _PC_PRIO_IO
    {"PC_PRIO_IO",	_PC_PRIO_IO},
#endif
#ifdef _PC_SOCK_MAXBUF
    {"PC_SOCK_MAXBUF",	_PC_SOCK_MAXBUF},
#endif
#ifdef _PC_SYNC_IO
    {"PC_SYNC_IO",	_PC_SYNC_IO},
#endif
#ifdef _PC_VDISABLE
    {"PC_VDISABLE",	_PC_VDISABLE},
#endif
};

static int
conv_path_confname(arg, valuep)
     PyObject *arg;
     int *valuep;
{
    return conv_confname(arg, valuep, posix_constants_pathconf,
                         sizeof(posix_constants_pathconf)
                           / sizeof(struct constdef));
}
#endif

#ifdef HAVE_FPATHCONF
static char posix_fpathconf__doc__[] = "\
fpathconf(fd, name) -> integer\n\
Return the configuration limit name for the file descriptor fd.\n\
If there is no limit, return -1.";

static PyObject *
posix_fpathconf(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;
    int name, fd;

    if (PyArg_ParseTuple(args, "iO&:fpathconf", &fd,
                         conv_path_confname, &name)) {
        long limit;

        errno = 0;
        limit = fpathconf(fd, name);
        if (limit == -1 && errno != 0)
            posix_error();
        else
            result = PyInt_FromLong(limit);
    }
    return result;
}
#endif


#ifdef HAVE_PATHCONF
static char posix_pathconf__doc__[] = "\
pathconf(path, name) -> integer\n\
Return the configuration limit name for the file or directory path.\n\
If there is no limit, return -1.";

static PyObject *
posix_pathconf(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;
    int name;
    char *path;

    if (PyArg_ParseTuple(args, "sO&:pathconf", &path,
                         conv_path_confname, &name)) {
        long limit;

        errno = 0;
        limit = pathconf(path, name);
        if (limit == -1 && errno != 0) {
            if (errno == EINVAL)
                /* could be a path or name problem */
                posix_error();
            else
                posix_error_with_filename(path);
        }
        else
            result = PyInt_FromLong(limit);
    }
    return result;
}
#endif

#ifdef HAVE_CONFSTR
static struct constdef posix_constants_confstr[] = {
#ifdef _CS_ARCHITECTURE
    {"CS_ARCHITECTURE",	_CS_ARCHITECTURE},
#endif
#ifdef _CS_HOSTNAME
    {"CS_HOSTNAME",	_CS_HOSTNAME},
#endif
#ifdef _CS_HW_PROVIDER
    {"CS_HW_PROVIDER",	_CS_HW_PROVIDER},
#endif
#ifdef _CS_HW_SERIAL
    {"CS_HW_SERIAL",	_CS_HW_SERIAL},
#endif
#ifdef _CS_INITTAB_NAME
    {"CS_INITTAB_NAME",	_CS_INITTAB_NAME},
#endif
#ifdef _CS_LFS64_CFLAGS
    {"CS_LFS64_CFLAGS",	_CS_LFS64_CFLAGS},
#endif
#ifdef _CS_LFS64_LDFLAGS
    {"CS_LFS64_LDFLAGS",	_CS_LFS64_LDFLAGS},
#endif
#ifdef _CS_LFS64_LIBS
    {"CS_LFS64_LIBS",	_CS_LFS64_LIBS},
#endif
#ifdef _CS_LFS64_LINTFLAGS
    {"CS_LFS64_LINTFLAGS",	_CS_LFS64_LINTFLAGS},
#endif
#ifdef _CS_LFS_CFLAGS
    {"CS_LFS_CFLAGS",	_CS_LFS_CFLAGS},
#endif
#ifdef _CS_LFS_LDFLAGS
    {"CS_LFS_LDFLAGS",	_CS_LFS_LDFLAGS},
#endif
#ifdef _CS_LFS_LIBS
    {"CS_LFS_LIBS",	_CS_LFS_LIBS},
#endif
#ifdef _CS_LFS_LINTFLAGS
    {"CS_LFS_LINTFLAGS",	_CS_LFS_LINTFLAGS},
#endif
#ifdef _CS_MACHINE
    {"CS_MACHINE",	_CS_MACHINE},
#endif
#ifdef _CS_PATH
    {"CS_PATH",	_CS_PATH},
#endif
#ifdef _CS_RELEASE
    {"CS_RELEASE",	_CS_RELEASE},
#endif
#ifdef _CS_SRPC_DOMAIN
    {"CS_SRPC_DOMAIN",	_CS_SRPC_DOMAIN},
#endif
#ifdef _CS_SYSNAME
    {"CS_SYSNAME",	_CS_SYSNAME},
#endif
#ifdef _CS_VERSION
    {"CS_VERSION",	_CS_VERSION},
#endif
#ifdef _CS_XBS5_ILP32_OFF32_CFLAGS
    {"CS_XBS5_ILP32_OFF32_CFLAGS",	_CS_XBS5_ILP32_OFF32_CFLAGS},
#endif
#ifdef _CS_XBS5_ILP32_OFF32_LDFLAGS
    {"CS_XBS5_ILP32_OFF32_LDFLAGS",	_CS_XBS5_ILP32_OFF32_LDFLAGS},
#endif
#ifdef _CS_XBS5_ILP32_OFF32_LIBS
    {"CS_XBS5_ILP32_OFF32_LIBS",	_CS_XBS5_ILP32_OFF32_LIBS},
#endif
#ifdef _CS_XBS5_ILP32_OFF32_LINTFLAGS
    {"CS_XBS5_ILP32_OFF32_LINTFLAGS",	_CS_XBS5_ILP32_OFF32_LINTFLAGS},
#endif
#ifdef _CS_XBS5_ILP32_OFFBIG_CFLAGS
    {"CS_XBS5_ILP32_OFFBIG_CFLAGS",	_CS_XBS5_ILP32_OFFBIG_CFLAGS},
#endif
#ifdef _CS_XBS5_ILP32_OFFBIG_LDFLAGS
    {"CS_XBS5_ILP32_OFFBIG_LDFLAGS",	_CS_XBS5_ILP32_OFFBIG_LDFLAGS},
#endif
#ifdef _CS_XBS5_ILP32_OFFBIG_LIBS
    {"CS_XBS5_ILP32_OFFBIG_LIBS",	_CS_XBS5_ILP32_OFFBIG_LIBS},
#endif
#ifdef _CS_XBS5_ILP32_OFFBIG_LINTFLAGS
    {"CS_XBS5_ILP32_OFFBIG_LINTFLAGS",	_CS_XBS5_ILP32_OFFBIG_LINTFLAGS},
#endif
#ifdef _CS_XBS5_LP64_OFF64_CFLAGS
    {"CS_XBS5_LP64_OFF64_CFLAGS",	_CS_XBS5_LP64_OFF64_CFLAGS},
#endif
#ifdef _CS_XBS5_LP64_OFF64_LDFLAGS
    {"CS_XBS5_LP64_OFF64_LDFLAGS",	_CS_XBS5_LP64_OFF64_LDFLAGS},
#endif
#ifdef _CS_XBS5_LP64_OFF64_LIBS
    {"CS_XBS5_LP64_OFF64_LIBS",	_CS_XBS5_LP64_OFF64_LIBS},
#endif
#ifdef _CS_XBS5_LP64_OFF64_LINTFLAGS
    {"CS_XBS5_LP64_OFF64_LINTFLAGS",	_CS_XBS5_LP64_OFF64_LINTFLAGS},
#endif
#ifdef _CS_XBS5_LPBIG_OFFBIG_CFLAGS
    {"CS_XBS5_LPBIG_OFFBIG_CFLAGS",	_CS_XBS5_LPBIG_OFFBIG_CFLAGS},
#endif
#ifdef _CS_XBS5_LPBIG_OFFBIG_LDFLAGS
    {"CS_XBS5_LPBIG_OFFBIG_LDFLAGS",	_CS_XBS5_LPBIG_OFFBIG_LDFLAGS},
#endif
#ifdef _CS_XBS5_LPBIG_OFFBIG_LIBS
    {"CS_XBS5_LPBIG_OFFBIG_LIBS",	_CS_XBS5_LPBIG_OFFBIG_LIBS},
#endif
#ifdef _CS_XBS5_LPBIG_OFFBIG_LINTFLAGS
    {"CS_XBS5_LPBIG_OFFBIG_LINTFLAGS",	_CS_XBS5_LPBIG_OFFBIG_LINTFLAGS},
#endif
#ifdef _MIPS_CS_AVAIL_PROCESSORS
    {"MIPS_CS_AVAIL_PROCESSORS",	_MIPS_CS_AVAIL_PROCESSORS},
#endif
#ifdef _MIPS_CS_BASE
    {"MIPS_CS_BASE",	_MIPS_CS_BASE},
#endif
#ifdef _MIPS_CS_HOSTID
    {"MIPS_CS_HOSTID",	_MIPS_CS_HOSTID},
#endif
#ifdef _MIPS_CS_HW_NAME
    {"MIPS_CS_HW_NAME",	_MIPS_CS_HW_NAME},
#endif
#ifdef _MIPS_CS_NUM_PROCESSORS
    {"MIPS_CS_NUM_PROCESSORS",	_MIPS_CS_NUM_PROCESSORS},
#endif
#ifdef _MIPS_CS_OSREL_MAJ
    {"MIPS_CS_OSREL_MAJ",	_MIPS_CS_OSREL_MAJ},
#endif
#ifdef _MIPS_CS_OSREL_MIN
    {"MIPS_CS_OSREL_MIN",	_MIPS_CS_OSREL_MIN},
#endif
#ifdef _MIPS_CS_OSREL_PATCH
    {"MIPS_CS_OSREL_PATCH",	_MIPS_CS_OSREL_PATCH},
#endif
#ifdef _MIPS_CS_OS_NAME
    {"MIPS_CS_OS_NAME",	_MIPS_CS_OS_NAME},
#endif
#ifdef _MIPS_CS_OS_PROVIDER
    {"MIPS_CS_OS_PROVIDER",	_MIPS_CS_OS_PROVIDER},
#endif
#ifdef _MIPS_CS_PROCESSORS
    {"MIPS_CS_PROCESSORS",	_MIPS_CS_PROCESSORS},
#endif
#ifdef _MIPS_CS_SERIAL
    {"MIPS_CS_SERIAL",	_MIPS_CS_SERIAL},
#endif
#ifdef _MIPS_CS_VENDOR
    {"MIPS_CS_VENDOR",	_MIPS_CS_VENDOR},
#endif
};

static int
conv_confstr_confname(arg, valuep)
     PyObject *arg;
     int *valuep;
{
    return conv_confname(arg, valuep, posix_constants_confstr,
                         sizeof(posix_constants_confstr)
                           / sizeof(struct constdef));
}

static char posix_confstr__doc__[] = "\
confstr(name) -> string\n\
Return a string-valued system configuration variable.";

static PyObject *
posix_confstr(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;
    int name;
    char buffer[64];

    if (PyArg_ParseTuple(args, "O&:confstr", conv_confstr_confname, &name)) {
        int len = confstr(name, buffer, sizeof(buffer));

        errno = 0;
        if (len == 0) {
            if (errno != 0)
                posix_error();
            else
                result = PyString_FromString("");
        }
        else {
            if (len >= sizeof(buffer)) {
                result = PyString_FromStringAndSize(NULL, len);
                if (result != NULL)
                    confstr(name, PyString_AS_STRING(result), len+1);
            }
            else
                result = PyString_FromString(buffer);
        }
    }
    return result;
}
#endif


#ifdef HAVE_SYSCONF
static struct constdef posix_constants_sysconf[] = {
#ifdef _SC_2_CHAR_TERM
    {"SC_2_CHAR_TERM",	_SC_2_CHAR_TERM},
#endif
#ifdef _SC_2_C_BIND
    {"SC_2_C_BIND",	_SC_2_C_BIND},
#endif
#ifdef _SC_2_C_DEV
    {"SC_2_C_DEV",	_SC_2_C_DEV},
#endif
#ifdef _SC_2_C_VERSION
    {"SC_2_C_VERSION",	_SC_2_C_VERSION},
#endif
#ifdef _SC_2_FORT_DEV
    {"SC_2_FORT_DEV",	_SC_2_FORT_DEV},
#endif
#ifdef _SC_2_FORT_RUN
    {"SC_2_FORT_RUN",	_SC_2_FORT_RUN},
#endif
#ifdef _SC_2_LOCALEDEF
    {"SC_2_LOCALEDEF",	_SC_2_LOCALEDEF},
#endif
#ifdef _SC_2_SW_DEV
    {"SC_2_SW_DEV",	_SC_2_SW_DEV},
#endif
#ifdef _SC_2_UPE
    {"SC_2_UPE",	_SC_2_UPE},
#endif
#ifdef _SC_2_VERSION
    {"SC_2_VERSION",	_SC_2_VERSION},
#endif
#ifdef _SC_ABI_ASYNCHRONOUS_IO
    {"SC_ABI_ASYNCHRONOUS_IO",	_SC_ABI_ASYNCHRONOUS_IO},
#endif
#ifdef _SC_ACL
    {"SC_ACL",	_SC_ACL},
#endif
#ifdef _SC_AIO_LISTIO_MAX
    {"SC_AIO_LISTIO_MAX",	_SC_AIO_LISTIO_MAX},
#endif
#ifdef _SC_AIO_MAX
    {"SC_AIO_MAX",	_SC_AIO_MAX},
#endif
#ifdef _SC_AIO_PRIO_DELTA_MAX
    {"SC_AIO_PRIO_DELTA_MAX",	_SC_AIO_PRIO_DELTA_MAX},
#endif
#ifdef _SC_ARG_MAX
    {"SC_ARG_MAX",	_SC_ARG_MAX},
#endif
#ifdef _SC_ASYNCHRONOUS_IO
    {"SC_ASYNCHRONOUS_IO",	_SC_ASYNCHRONOUS_IO},
#endif
#ifdef _SC_ATEXIT_MAX
    {"SC_ATEXIT_MAX",	_SC_ATEXIT_MAX},
#endif
#ifdef _SC_AUDIT
    {"SC_AUDIT",	_SC_AUDIT},
#endif
#ifdef _SC_AVPHYS_PAGES
    {"SC_AVPHYS_PAGES",	_SC_AVPHYS_PAGES},
#endif
#ifdef _SC_BC_BASE_MAX
    {"SC_BC_BASE_MAX",	_SC_BC_BASE_MAX},
#endif
#ifdef _SC_BC_DIM_MAX
    {"SC_BC_DIM_MAX",	_SC_BC_DIM_MAX},
#endif
#ifdef _SC_BC_SCALE_MAX
    {"SC_BC_SCALE_MAX",	_SC_BC_SCALE_MAX},
#endif
#ifdef _SC_BC_STRING_MAX
    {"SC_BC_STRING_MAX",	_SC_BC_STRING_MAX},
#endif
#ifdef _SC_CAP
    {"SC_CAP",	_SC_CAP},
#endif
#ifdef _SC_CHARCLASS_NAME_MAX
    {"SC_CHARCLASS_NAME_MAX",	_SC_CHARCLASS_NAME_MAX},
#endif
#ifdef _SC_CHAR_BIT
    {"SC_CHAR_BIT",	_SC_CHAR_BIT},
#endif
#ifdef _SC_CHAR_MAX
    {"SC_CHAR_MAX",	_SC_CHAR_MAX},
#endif
#ifdef _SC_CHAR_MIN
    {"SC_CHAR_MIN",	_SC_CHAR_MIN},
#endif
#ifdef _SC_CHILD_MAX
    {"SC_CHILD_MAX",	_SC_CHILD_MAX},
#endif
#ifdef _SC_CLK_TCK
    {"SC_CLK_TCK",	_SC_CLK_TCK},
#endif
#ifdef _SC_COHER_BLKSZ
    {"SC_COHER_BLKSZ",	_SC_COHER_BLKSZ},
#endif
#ifdef _SC_COLL_WEIGHTS_MAX
    {"SC_COLL_WEIGHTS_MAX",	_SC_COLL_WEIGHTS_MAX},
#endif
#ifdef _SC_DCACHE_ASSOC
    {"SC_DCACHE_ASSOC",	_SC_DCACHE_ASSOC},
#endif
#ifdef _SC_DCACHE_BLKSZ
    {"SC_DCACHE_BLKSZ",	_SC_DCACHE_BLKSZ},
#endif
#ifdef _SC_DCACHE_LINESZ
    {"SC_DCACHE_LINESZ",	_SC_DCACHE_LINESZ},
#endif
#ifdef _SC_DCACHE_SZ
    {"SC_DCACHE_SZ",	_SC_DCACHE_SZ},
#endif
#ifdef _SC_DCACHE_TBLKSZ
    {"SC_DCACHE_TBLKSZ",	_SC_DCACHE_TBLKSZ},
#endif
#ifdef _SC_DELAYTIMER_MAX
    {"SC_DELAYTIMER_MAX",	_SC_DELAYTIMER_MAX},
#endif
#ifdef _SC_EQUIV_CLASS_MAX
    {"SC_EQUIV_CLASS_MAX",	_SC_EQUIV_CLASS_MAX},
#endif
#ifdef _SC_EXPR_NEST_MAX
    {"SC_EXPR_NEST_MAX",	_SC_EXPR_NEST_MAX},
#endif
#ifdef _SC_FSYNC
    {"SC_FSYNC",	_SC_FSYNC},
#endif
#ifdef _SC_GETGR_R_SIZE_MAX
    {"SC_GETGR_R_SIZE_MAX",	_SC_GETGR_R_SIZE_MAX},
#endif
#ifdef _SC_GETPW_R_SIZE_MAX
    {"SC_GETPW_R_SIZE_MAX",	_SC_GETPW_R_SIZE_MAX},
#endif
#ifdef _SC_ICACHE_ASSOC
    {"SC_ICACHE_ASSOC",	_SC_ICACHE_ASSOC},
#endif
#ifdef _SC_ICACHE_BLKSZ
    {"SC_ICACHE_BLKSZ",	_SC_ICACHE_BLKSZ},
#endif
#ifdef _SC_ICACHE_LINESZ
    {"SC_ICACHE_LINESZ",	_SC_ICACHE_LINESZ},
#endif
#ifdef _SC_ICACHE_SZ
    {"SC_ICACHE_SZ",	_SC_ICACHE_SZ},
#endif
#ifdef _SC_INF
    {"SC_INF",	_SC_INF},
#endif
#ifdef _SC_INT_MAX
    {"SC_INT_MAX",	_SC_INT_MAX},
#endif
#ifdef _SC_INT_MIN
    {"SC_INT_MIN",	_SC_INT_MIN},
#endif
#ifdef _SC_IOV_MAX
    {"SC_IOV_MAX",	_SC_IOV_MAX},
#endif
#ifdef _SC_IP_SECOPTS
    {"SC_IP_SECOPTS",	_SC_IP_SECOPTS},
#endif
#ifdef _SC_JOB_CONTROL
    {"SC_JOB_CONTROL",	_SC_JOB_CONTROL},
#endif
#ifdef _SC_KERN_POINTERS
    {"SC_KERN_POINTERS",	_SC_KERN_POINTERS},
#endif
#ifdef _SC_KERN_SIM
    {"SC_KERN_SIM",	_SC_KERN_SIM},
#endif
#ifdef _SC_LINE_MAX
    {"SC_LINE_MAX",	_SC_LINE_MAX},
#endif
#ifdef _SC_LOGIN_NAME_MAX
    {"SC_LOGIN_NAME_MAX",	_SC_LOGIN_NAME_MAX},
#endif
#ifdef _SC_LOGNAME_MAX
    {"SC_LOGNAME_MAX",	_SC_LOGNAME_MAX},
#endif
#ifdef _SC_LONG_BIT
    {"SC_LONG_BIT",	_SC_LONG_BIT},
#endif
#ifdef _SC_MAC
    {"SC_MAC",	_SC_MAC},
#endif
#ifdef _SC_MAPPED_FILES
    {"SC_MAPPED_FILES",	_SC_MAPPED_FILES},
#endif
#ifdef _SC_MAXPID
    {"SC_MAXPID",	_SC_MAXPID},
#endif
#ifdef _SC_MB_LEN_MAX
    {"SC_MB_LEN_MAX",	_SC_MB_LEN_MAX},
#endif
#ifdef _SC_MEMLOCK
    {"SC_MEMLOCK",	_SC_MEMLOCK},
#endif
#ifdef _SC_MEMLOCK_RANGE
    {"SC_MEMLOCK_RANGE",	_SC_MEMLOCK_RANGE},
#endif
#ifdef _SC_MEMORY_PROTECTION
    {"SC_MEMORY_PROTECTION",	_SC_MEMORY_PROTECTION},
#endif
#ifdef _SC_MESSAGE_PASSING
    {"SC_MESSAGE_PASSING",	_SC_MESSAGE_PASSING},
#endif
#ifdef _SC_MMAP_FIXED_ALIGNMENT
    {"SC_MMAP_FIXED_ALIGNMENT",	_SC_MMAP_FIXED_ALIGNMENT},
#endif
#ifdef _SC_MQ_OPEN_MAX
    {"SC_MQ_OPEN_MAX",	_SC_MQ_OPEN_MAX},
#endif
#ifdef _SC_MQ_PRIO_MAX
    {"SC_MQ_PRIO_MAX",	_SC_MQ_PRIO_MAX},
#endif
#ifdef _SC_NACLS_MAX
    {"SC_NACLS_MAX",	_SC_NACLS_MAX},
#endif
#ifdef _SC_NGROUPS_MAX
    {"SC_NGROUPS_MAX",	_SC_NGROUPS_MAX},
#endif
#ifdef _SC_NL_ARGMAX
    {"SC_NL_ARGMAX",	_SC_NL_ARGMAX},
#endif
#ifdef _SC_NL_LANGMAX
    {"SC_NL_LANGMAX",	_SC_NL_LANGMAX},
#endif
#ifdef _SC_NL_MSGMAX
    {"SC_NL_MSGMAX",	_SC_NL_MSGMAX},
#endif
#ifdef _SC_NL_NMAX
    {"SC_NL_NMAX",	_SC_NL_NMAX},
#endif
#ifdef _SC_NL_SETMAX
    {"SC_NL_SETMAX",	_SC_NL_SETMAX},
#endif
#ifdef _SC_NL_TEXTMAX
    {"SC_NL_TEXTMAX",	_SC_NL_TEXTMAX},
#endif
#ifdef _SC_NPROCESSORS_CONF
    {"SC_NPROCESSORS_CONF",	_SC_NPROCESSORS_CONF},
#endif
#ifdef _SC_NPROCESSORS_ONLN
    {"SC_NPROCESSORS_ONLN",	_SC_NPROCESSORS_ONLN},
#endif
#ifdef _SC_NPROC_CONF
    {"SC_NPROC_CONF",	_SC_NPROC_CONF},
#endif
#ifdef _SC_NPROC_ONLN
    {"SC_NPROC_ONLN",	_SC_NPROC_ONLN},
#endif
#ifdef _SC_NZERO
    {"SC_NZERO",	_SC_NZERO},
#endif
#ifdef _SC_OPEN_MAX
    {"SC_OPEN_MAX",	_SC_OPEN_MAX},
#endif
#ifdef _SC_PAGESIZE
    {"SC_PAGESIZE",	_SC_PAGESIZE},
#endif
#ifdef _SC_PAGE_SIZE
    {"SC_PAGE_SIZE",	_SC_PAGE_SIZE},
#endif
#ifdef _SC_PASS_MAX
    {"SC_PASS_MAX",	_SC_PASS_MAX},
#endif
#ifdef _SC_PHYS_PAGES
    {"SC_PHYS_PAGES",	_SC_PHYS_PAGES},
#endif
#ifdef _SC_PII
    {"SC_PII",	_SC_PII},
#endif
#ifdef _SC_PII_INTERNET
    {"SC_PII_INTERNET",	_SC_PII_INTERNET},
#endif
#ifdef _SC_PII_INTERNET_DGRAM
    {"SC_PII_INTERNET_DGRAM",	_SC_PII_INTERNET_DGRAM},
#endif
#ifdef _SC_PII_INTERNET_STREAM
    {"SC_PII_INTERNET_STREAM",	_SC_PII_INTERNET_STREAM},
#endif
#ifdef _SC_PII_OSI
    {"SC_PII_OSI",	_SC_PII_OSI},
#endif
#ifdef _SC_PII_OSI_CLTS
    {"SC_PII_OSI_CLTS",	_SC_PII_OSI_CLTS},
#endif
#ifdef _SC_PII_OSI_COTS
    {"SC_PII_OSI_COTS",	_SC_PII_OSI_COTS},
#endif
#ifdef _SC_PII_OSI_M
    {"SC_PII_OSI_M",	_SC_PII_OSI_M},
#endif
#ifdef _SC_PII_SOCKET
    {"SC_PII_SOCKET",	_SC_PII_SOCKET},
#endif
#ifdef _SC_PII_XTI
    {"SC_PII_XTI",	_SC_PII_XTI},
#endif
#ifdef _SC_POLL
    {"SC_POLL",	_SC_POLL},
#endif
#ifdef _SC_PRIORITIZED_IO
    {"SC_PRIORITIZED_IO",	_SC_PRIORITIZED_IO},
#endif
#ifdef _SC_PRIORITY_SCHEDULING
    {"SC_PRIORITY_SCHEDULING",	_SC_PRIORITY_SCHEDULING},
#endif
#ifdef _SC_REALTIME_SIGNALS
    {"SC_REALTIME_SIGNALS",	_SC_REALTIME_SIGNALS},
#endif
#ifdef _SC_RE_DUP_MAX
    {"SC_RE_DUP_MAX",	_SC_RE_DUP_MAX},
#endif
#ifdef _SC_RTSIG_MAX
    {"SC_RTSIG_MAX",	_SC_RTSIG_MAX},
#endif
#ifdef _SC_SAVED_IDS
    {"SC_SAVED_IDS",	_SC_SAVED_IDS},
#endif
#ifdef _SC_SCHAR_MAX
    {"SC_SCHAR_MAX",	_SC_SCHAR_MAX},
#endif
#ifdef _SC_SCHAR_MIN
    {"SC_SCHAR_MIN",	_SC_SCHAR_MIN},
#endif
#ifdef _SC_SELECT
    {"SC_SELECT",	_SC_SELECT},
#endif
#ifdef _SC_SEMAPHORES
    {"SC_SEMAPHORES",	_SC_SEMAPHORES},
#endif
#ifdef _SC_SEM_NSEMS_MAX
    {"SC_SEM_NSEMS_MAX",	_SC_SEM_NSEMS_MAX},
#endif
#ifdef _SC_SEM_VALUE_MAX
    {"SC_SEM_VALUE_MAX",	_SC_SEM_VALUE_MAX},
#endif
#ifdef _SC_SHARED_MEMORY_OBJECTS
    {"SC_SHARED_MEMORY_OBJECTS",	_SC_SHARED_MEMORY_OBJECTS},
#endif
#ifdef _SC_SHRT_MAX
    {"SC_SHRT_MAX",	_SC_SHRT_MAX},
#endif
#ifdef _SC_SHRT_MIN
    {"SC_SHRT_MIN",	_SC_SHRT_MIN},
#endif
#ifdef _SC_SIGQUEUE_MAX
    {"SC_SIGQUEUE_MAX",	_SC_SIGQUEUE_MAX},
#endif
#ifdef _SC_SIGRT_MAX
    {"SC_SIGRT_MAX",	_SC_SIGRT_MAX},
#endif
#ifdef _SC_SIGRT_MIN
    {"SC_SIGRT_MIN",	_SC_SIGRT_MIN},
#endif
#ifdef _SC_SOFTPOWER
    {"SC_SOFTPOWER",	_SC_SOFTPOWER},
#endif
#ifdef _SC_SPLIT_CACHE
    {"SC_SPLIT_CACHE",	_SC_SPLIT_CACHE},
#endif
#ifdef _SC_SSIZE_MAX
    {"SC_SSIZE_MAX",	_SC_SSIZE_MAX},
#endif
#ifdef _SC_STACK_PROT
    {"SC_STACK_PROT",	_SC_STACK_PROT},
#endif
#ifdef _SC_STREAM_MAX
    {"SC_STREAM_MAX",	_SC_STREAM_MAX},
#endif
#ifdef _SC_SYNCHRONIZED_IO
    {"SC_SYNCHRONIZED_IO",	_SC_SYNCHRONIZED_IO},
#endif
#ifdef _SC_THREADS
    {"SC_THREADS",	_SC_THREADS},
#endif
#ifdef _SC_THREAD_ATTR_STACKADDR
    {"SC_THREAD_ATTR_STACKADDR",	_SC_THREAD_ATTR_STACKADDR},
#endif
#ifdef _SC_THREAD_ATTR_STACKSIZE
    {"SC_THREAD_ATTR_STACKSIZE",	_SC_THREAD_ATTR_STACKSIZE},
#endif
#ifdef _SC_THREAD_DESTRUCTOR_ITERATIONS
    {"SC_THREAD_DESTRUCTOR_ITERATIONS",	_SC_THREAD_DESTRUCTOR_ITERATIONS},
#endif
#ifdef _SC_THREAD_KEYS_MAX
    {"SC_THREAD_KEYS_MAX",	_SC_THREAD_KEYS_MAX},
#endif
#ifdef _SC_THREAD_PRIORITY_SCHEDULING
    {"SC_THREAD_PRIORITY_SCHEDULING",	_SC_THREAD_PRIORITY_SCHEDULING},
#endif
#ifdef _SC_THREAD_PRIO_INHERIT
    {"SC_THREAD_PRIO_INHERIT",	_SC_THREAD_PRIO_INHERIT},
#endif
#ifdef _SC_THREAD_PRIO_PROTECT
    {"SC_THREAD_PRIO_PROTECT",	_SC_THREAD_PRIO_PROTECT},
#endif
#ifdef _SC_THREAD_PROCESS_SHARED
    {"SC_THREAD_PROCESS_SHARED",	_SC_THREAD_PROCESS_SHARED},
#endif
#ifdef _SC_THREAD_SAFE_FUNCTIONS
    {"SC_THREAD_SAFE_FUNCTIONS",	_SC_THREAD_SAFE_FUNCTIONS},
#endif
#ifdef _SC_THREAD_STACK_MIN
    {"SC_THREAD_STACK_MIN",	_SC_THREAD_STACK_MIN},
#endif
#ifdef _SC_THREAD_THREADS_MAX
    {"SC_THREAD_THREADS_MAX",	_SC_THREAD_THREADS_MAX},
#endif
#ifdef _SC_TIMERS
    {"SC_TIMERS",	_SC_TIMERS},
#endif
#ifdef _SC_TIMER_MAX
    {"SC_TIMER_MAX",	_SC_TIMER_MAX},
#endif
#ifdef _SC_TTY_NAME_MAX
    {"SC_TTY_NAME_MAX",	_SC_TTY_NAME_MAX},
#endif
#ifdef _SC_TZNAME_MAX
    {"SC_TZNAME_MAX",	_SC_TZNAME_MAX},
#endif
#ifdef _SC_T_IOV_MAX
    {"SC_T_IOV_MAX",	_SC_T_IOV_MAX},
#endif
#ifdef _SC_UCHAR_MAX
    {"SC_UCHAR_MAX",	_SC_UCHAR_MAX},
#endif
#ifdef _SC_UINT_MAX
    {"SC_UINT_MAX",	_SC_UINT_MAX},
#endif
#ifdef _SC_UIO_MAXIOV
    {"SC_UIO_MAXIOV",	_SC_UIO_MAXIOV},
#endif
#ifdef _SC_ULONG_MAX
    {"SC_ULONG_MAX",	_SC_ULONG_MAX},
#endif
#ifdef _SC_USHRT_MAX
    {"SC_USHRT_MAX",	_SC_USHRT_MAX},
#endif
#ifdef _SC_VERSION
    {"SC_VERSION",	_SC_VERSION},
#endif
#ifdef _SC_WORD_BIT
    {"SC_WORD_BIT",	_SC_WORD_BIT},
#endif
#ifdef _SC_XBS5_ILP32_OFF32
    {"SC_XBS5_ILP32_OFF32",	_SC_XBS5_ILP32_OFF32},
#endif
#ifdef _SC_XBS5_ILP32_OFFBIG
    {"SC_XBS5_ILP32_OFFBIG",	_SC_XBS5_ILP32_OFFBIG},
#endif
#ifdef _SC_XBS5_LP64_OFF64
    {"SC_XBS5_LP64_OFF64",	_SC_XBS5_LP64_OFF64},
#endif
#ifdef _SC_XBS5_LPBIG_OFFBIG
    {"SC_XBS5_LPBIG_OFFBIG",	_SC_XBS5_LPBIG_OFFBIG},
#endif
#ifdef _SC_XOPEN_CRYPT
    {"SC_XOPEN_CRYPT",	_SC_XOPEN_CRYPT},
#endif
#ifdef _SC_XOPEN_ENH_I18N
    {"SC_XOPEN_ENH_I18N",	_SC_XOPEN_ENH_I18N},
#endif
#ifdef _SC_XOPEN_LEGACY
    {"SC_XOPEN_LEGACY",	_SC_XOPEN_LEGACY},
#endif
#ifdef _SC_XOPEN_REALTIME
    {"SC_XOPEN_REALTIME",	_SC_XOPEN_REALTIME},
#endif
#ifdef _SC_XOPEN_REALTIME_THREADS
    {"SC_XOPEN_REALTIME_THREADS",	_SC_XOPEN_REALTIME_THREADS},
#endif
#ifdef _SC_XOPEN_SHM
    {"SC_XOPEN_SHM",	_SC_XOPEN_SHM},
#endif
#ifdef _SC_XOPEN_UNIX
    {"SC_XOPEN_UNIX",	_SC_XOPEN_UNIX},
#endif
#ifdef _SC_XOPEN_VERSION
    {"SC_XOPEN_VERSION",	_SC_XOPEN_VERSION},
#endif
#ifdef _SC_XOPEN_XCU_VERSION
    {"SC_XOPEN_XCU_VERSION",	_SC_XOPEN_XCU_VERSION},
#endif
#ifdef _SC_XOPEN_XPG2
    {"SC_XOPEN_XPG2",	_SC_XOPEN_XPG2},
#endif
#ifdef _SC_XOPEN_XPG3
    {"SC_XOPEN_XPG3",	_SC_XOPEN_XPG3},
#endif
#ifdef _SC_XOPEN_XPG4
    {"SC_XOPEN_XPG4",	_SC_XOPEN_XPG4},
#endif
};

static int
conv_sysconf_confname(arg, valuep)
     PyObject *arg;
     int *valuep;
{
    return conv_confname(arg, valuep, posix_constants_sysconf,
                         sizeof(posix_constants_sysconf)
                           / sizeof(struct constdef));
}

static char posix_sysconf__doc__[] = "\
sysconf(name) -> integer\n\
Return an integer-valued system configuration variable.";

static PyObject *
posix_sysconf(self, args)
     PyObject *self;
     PyObject *args;
{
    PyObject *result = NULL;
    int name;

    if (PyArg_ParseTuple(args, "O&:sysconf", conv_sysconf_confname, &name)) {
        int value;

        errno = 0;
        value = sysconf(name);
        if (value == -1 && errno != 0)
            posix_error();
        else
            result = PyInt_FromLong(value);
    }
    return result;
}
#endif


/* This code is used to ensure that the tables of configuration value names
 * are in sorted order as required by conv_confname(), and also to build the
 * the exported dictionaries that are used to publish information about the
 * names available on the host platform.
 *
 * Sorting the table at runtime ensures that the table is properly ordered
 * when used, even for platforms we're not able to test on.  It also makes
 * it easier to add additional entries to the tables.
 */

static int
cmp_constdefs(v1, v2)
     const void *v1;
     const void *v2;
{
    const struct constdef *c1 =
        (const struct constdef *) v1;
    const struct constdef *c2 =
        (const struct constdef *) v2;

    return strcmp(c1->name, c2->name);
}

static int
setup_confname_table(table, tablesize, tablename, moddict)
     struct constdef *table;
     size_t tablesize;
     char * tablename;
     PyObject *moddict;
{
    PyObject *d = NULL;

    qsort(table, tablesize, sizeof(struct constdef), cmp_constdefs);
    d = PyDict_New();
    if (d != NULL) {
        PyObject *o;
        size_t i = 0;

        for (; i < tablesize; ++i) {
            o = PyInt_FromLong(table[i].value);
            if (o == NULL
                || PyDict_SetItemString(d, table[i].name, o) == -1) {
                Py_DECREF(d);
                d = NULL;
                return -1;
            }
        }
        if (PyDict_SetItemString(moddict, tablename, d) == -1)
            return -1;
        return 0;
    }
    return -1;
}

/* Return -1 on failure, 0 on success. */
static int
setup_confname_tables(moddict)
     PyObject *moddict;
{
#if defined(HAVE_FPATHCONF) || defined(HAVE_PATHCONF)
    if (setup_confname_table(posix_constants_pathconf,
                             sizeof(posix_constants_pathconf)
                               / sizeof(struct constdef),
                             "pathconf_names", moddict))
        return -1;
#endif
#ifdef HAVE_CONFSTR
    if (setup_confname_table(posix_constants_confstr,
                             sizeof(posix_constants_confstr)
                               / sizeof(struct constdef),
                             "confstr_names", moddict))
        return -1;
#endif
#ifdef HAVE_SYSCONF
    if (setup_confname_table(posix_constants_sysconf,
                             sizeof(posix_constants_sysconf)
                               / sizeof(struct constdef),
                             "sysconf_names", moddict))
        return -1;
#endif
    return 0;
}


static char posix_abort__doc__[] = "\
abort() -> does not return!\n\
Abort the interpreter immediately.  This 'dumps core' or otherwise fails\n\
in the hardest way possible on the hosting operating system.";

static PyObject *
posix_abort(self, args)
     PyObject *self;
     PyObject *args;
{
    if (!PyArg_ParseTuple(args, ":abort"))
        return NULL;
    abort();
    /*NOTREACHED*/
    Py_FatalError("abort() called from Python code didn't abort!");
    return NULL;
}


static PyMethodDef posix_methods[] = {
	{"access",	posix_access, METH_VARARGS, posix_access__doc__},
#ifdef HAVE_TTYNAME
	{"ttyname",	posix_ttyname, METH_VARARGS, posix_ttyname__doc__},
#endif
	{"chdir",	posix_chdir, METH_VARARGS, posix_chdir__doc__},
	{"chmod",	posix_chmod, METH_VARARGS, posix_chmod__doc__},
#ifdef HAVE_CHOWN
	{"chown",	posix_chown, METH_VARARGS, posix_chown__doc__},
#endif /* HAVE_CHOWN */
#ifdef HAVE_CTERMID
	{"ctermid",	posix_ctermid, METH_VARARGS, posix_ctermid__doc__},
#endif
#ifdef HAVE_GETCWD
	{"getcwd",	posix_getcwd, METH_VARARGS, posix_getcwd__doc__},
#endif
#ifdef HAVE_LINK
	{"link",	posix_link, METH_VARARGS, posix_link__doc__},
#endif /* HAVE_LINK */
	{"listdir",	posix_listdir, METH_VARARGS, posix_listdir__doc__},
	{"lstat",	posix_lstat, METH_VARARGS, posix_lstat__doc__},
	{"mkdir",	posix_mkdir, METH_VARARGS, posix_mkdir__doc__},
#ifdef HAVE_NICE
	{"nice",	posix_nice, METH_VARARGS, posix_nice__doc__},
#endif /* HAVE_NICE */
#ifdef HAVE_READLINK
	{"readlink",	posix_readlink, METH_VARARGS, posix_readlink__doc__},
#endif /* HAVE_READLINK */
	{"rename",	posix_rename, METH_VARARGS, posix_rename__doc__},
	{"rmdir",	posix_rmdir, METH_VARARGS, posix_rmdir__doc__},
	{"stat",	posix_stat, METH_VARARGS, posix_stat__doc__},
#ifdef HAVE_SYMLINK
	{"symlink",	posix_symlink, METH_VARARGS, posix_symlink__doc__},
#endif /* HAVE_SYMLINK */
#ifdef HAVE_SYSTEM
	{"system",	posix_system, METH_VARARGS, posix_system__doc__},
#endif
	{"umask",	posix_umask, METH_VARARGS, posix_umask__doc__},
#ifdef HAVE_UNAME
	{"uname",	posix_uname, METH_VARARGS, posix_uname__doc__},
#endif /* HAVE_UNAME */
	{"unlink",	posix_unlink, METH_VARARGS, posix_unlink__doc__},
	{"remove",	posix_unlink, METH_VARARGS, posix_remove__doc__},
	{"utime",	posix_utime, METH_VARARGS, posix_utime__doc__},
#ifdef HAVE_TIMES
	{"times",	posix_times, METH_VARARGS, posix_times__doc__},
#endif /* HAVE_TIMES */
	{"_exit",	posix__exit, METH_VARARGS, posix__exit__doc__},
#ifdef HAVE_EXECV
	{"execv",	posix_execv, METH_VARARGS, posix_execv__doc__},
	{"execve",	posix_execve, METH_VARARGS, posix_execve__doc__},
#endif /* HAVE_EXECV */
#ifdef HAVE_SPAWNV
	{"spawnv",	posix_spawnv, METH_VARARGS, posix_spawnv__doc__},
	{"spawnve",	posix_spawnve, METH_VARARGS, posix_spawnve__doc__},
#endif /* HAVE_SPAWNV */
#ifdef HAVE_FORK
	{"fork",	posix_fork, METH_VARARGS, posix_fork__doc__},
#endif /* HAVE_FORK */
#ifdef HAVE_GETEGID
	{"getegid",	posix_getegid, METH_VARARGS, posix_getegid__doc__},
#endif /* HAVE_GETEGID */
#ifdef HAVE_GETEUID
	{"geteuid",	posix_geteuid, METH_VARARGS, posix_geteuid__doc__},
#endif /* HAVE_GETEUID */
#ifdef HAVE_GETGID
	{"getgid",	posix_getgid, METH_VARARGS, posix_getgid__doc__},
#endif /* HAVE_GETGID */
#ifdef HAVE_GETGROUPS
	{"getgroups",	posix_getgroups, METH_VARARGS, posix_getgroups__doc__},
#endif
	{"getpid",	posix_getpid, METH_VARARGS, posix_getpid__doc__},
#ifdef HAVE_GETPGRP
	{"getpgrp",	posix_getpgrp, METH_VARARGS, posix_getpgrp__doc__},
#endif /* HAVE_GETPGRP */
#ifdef HAVE_GETPPID
	{"getppid",	posix_getppid, METH_VARARGS, posix_getppid__doc__},
#endif /* HAVE_GETPPID */
#ifdef HAVE_GETUID
	{"getuid",	posix_getuid, METH_VARARGS, posix_getuid__doc__},
#endif /* HAVE_GETUID */
#ifdef HAVE_GETLOGIN
	{"getlogin",	posix_getlogin, METH_VARARGS, posix_getlogin__doc__},
#endif
#ifdef HAVE_KILL
	{"kill",	posix_kill, METH_VARARGS, posix_kill__doc__},
#endif /* HAVE_KILL */
#ifdef HAVE_PLOCK
	{"plock",	posix_plock, METH_VARARGS, posix_plock__doc__},
#endif /* HAVE_PLOCK */
#ifdef HAVE_POPEN
	{"popen",	posix_popen, METH_VARARGS, posix_popen__doc__},
#endif /* HAVE_POPEN */
#ifdef HAVE_SETUID
	{"setuid",	posix_setuid, METH_VARARGS, posix_setuid__doc__},
#endif /* HAVE_SETUID */
#ifdef HAVE_SETGID
	{"setgid",	posix_setgid, METH_VARARGS, posix_setgid__doc__},
#endif /* HAVE_SETGID */
#ifdef HAVE_SETPGRP
	{"setpgrp",	posix_setpgrp, METH_VARARGS, posix_setpgrp__doc__},
#endif /* HAVE_SETPGRP */
#ifdef HAVE_WAIT
	{"wait",	posix_wait, METH_VARARGS, posix_wait__doc__},
#endif /* HAVE_WAIT */
#ifdef HAVE_WAITPID
	{"waitpid",	posix_waitpid, METH_VARARGS, posix_waitpid__doc__},
#endif /* HAVE_WAITPID */
#ifdef HAVE_SETSID
	{"setsid",	posix_setsid, METH_VARARGS, posix_setsid__doc__},
#endif /* HAVE_SETSID */
#ifdef HAVE_SETPGID
	{"setpgid",	posix_setpgid, METH_VARARGS, posix_setpgid__doc__},
#endif /* HAVE_SETPGID */
#ifdef HAVE_TCGETPGRP
	{"tcgetpgrp",	posix_tcgetpgrp, METH_VARARGS, posix_tcgetpgrp__doc__},
#endif /* HAVE_TCGETPGRP */
#ifdef HAVE_TCSETPGRP
	{"tcsetpgrp",	posix_tcsetpgrp, METH_VARARGS, posix_tcsetpgrp__doc__},
#endif /* HAVE_TCSETPGRP */
	{"open",	posix_open, METH_VARARGS, posix_open__doc__},
	{"close",	posix_close, METH_VARARGS, posix_close__doc__},
	{"dup",		posix_dup, METH_VARARGS, posix_dup__doc__},
	{"dup2",	posix_dup2, METH_VARARGS, posix_dup2__doc__},
	{"lseek",	posix_lseek, METH_VARARGS, posix_lseek__doc__},
	{"read",	posix_read, METH_VARARGS, posix_read__doc__},
	{"write",	posix_write, METH_VARARGS, posix_write__doc__},
	{"fstat",	posix_fstat, METH_VARARGS, posix_fstat__doc__},
	{"fdopen",	posix_fdopen, METH_VARARGS, posix_fdopen__doc__},
#ifdef HAVE_PIPE
	{"pipe",	posix_pipe, METH_VARARGS, posix_pipe__doc__},
#endif
#ifdef HAVE_MKFIFO
	{"mkfifo",	posix_mkfifo, METH_VARARGS, posix_mkfifo__doc__},
#endif
#ifdef HAVE_FTRUNCATE
	{"ftruncate",	posix_ftruncate, METH_VARARGS, posix_ftruncate__doc__},
#endif
#ifdef HAVE_PUTENV
	{"putenv",	posix_putenv, METH_VARARGS, posix_putenv__doc__},
#endif
#ifdef HAVE_STRERROR
	{"strerror",	posix_strerror, METH_VARARGS, posix_strerror__doc__},
#endif
#ifdef HAVE_FSYNC
	{"fsync",       posix_fsync, METH_VARARGS, posix_fsync__doc__},
#endif
#ifdef HAVE_FDATASYNC
	{"fdatasync",   posix_fdatasync,  METH_VARARGS, posix_fdatasync__doc__},
#endif
#ifdef HAVE_SYS_WAIT_H
#ifdef WIFSTOPPED
        {"WIFSTOPPED",	posix_WIFSTOPPED, METH_VARARGS, posix_WIFSTOPPED__doc__},
#endif /* WIFSTOPPED */
#ifdef WIFSIGNALED
        {"WIFSIGNALED",	posix_WIFSIGNALED, METH_VARARGS, posix_WIFSIGNALED__doc__},
#endif /* WIFSIGNALED */
#ifdef WIFEXITED
        {"WIFEXITED",	posix_WIFEXITED, METH_VARARGS, posix_WIFEXITED__doc__},
#endif /* WIFEXITED */
#ifdef WEXITSTATUS
        {"WEXITSTATUS",	posix_WEXITSTATUS, METH_VARARGS, posix_WEXITSTATUS__doc__},
#endif /* WEXITSTATUS */
#ifdef WTERMSIG
        {"WTERMSIG",	posix_WTERMSIG, METH_VARARGS, posix_WTERMSIG__doc__},
#endif /* WTERMSIG */
#ifdef WSTOPSIG
        {"WSTOPSIG",	posix_WSTOPSIG, METH_VARARGS, posix_WSTOPSIG__doc__},
#endif /* WSTOPSIG */
#endif /* HAVE_SYS_WAIT_H */
#ifdef HAVE_FSTATVFS
	{"fstatvfs",	posix_fstatvfs, METH_VARARGS, posix_fstatvfs__doc__},
#endif
#ifdef HAVE_STATVFS
	{"statvfs",	posix_statvfs, METH_VARARGS, posix_statvfs__doc__},
#endif
#ifdef HAVE_TMPNAM
	{"tmpfile",	posix_tmpfile, METH_VARARGS, posix_tmpfile__doc__},
#endif
#ifdef HAVE_TEMPNAM
	{"tempnam",	posix_tempnam, METH_VARARGS, posix_tempnam__doc__},
#endif
#ifdef HAVE_TMPNAM
	{"tmpnam",	posix_tmpnam, METH_VARARGS, posix_tmpnam__doc__},
#endif
#ifdef HAVE_CONFSTR
	{"confstr",	posix_confstr, METH_VARARGS, posix_confstr__doc__},
#endif
#ifdef HAVE_SYSCONF
	{"sysconf",	posix_sysconf, METH_VARARGS, posix_sysconf__doc__},
#endif
#ifdef HAVE_FPATHCONF
	{"fpathconf",	posix_fpathconf, METH_VARARGS, posix_fpathconf__doc__},
#endif
#ifdef HAVE_PATHCONF
	{"pathconf",	posix_pathconf, METH_VARARGS, posix_pathconf__doc__},
#endif
	{"abort",	posix_abort, METH_VARARGS, posix_abort__doc__},
	{NULL,		NULL}		 /* Sentinel */
};


static int
ins(d, symbol, value)
        PyObject* d;
        char* symbol;
        long value;
{
        PyObject* v = PyInt_FromLong(value);
        if (!v || PyDict_SetItemString(d, symbol, v) < 0)
                return -1;                   /* triggers fatal error */

        Py_DECREF(v);
        return 0;
}

#if defined(PYOS_OS2)
/* Insert Platform-Specific Constant Values (Strings & Numbers) of Common Use */
static int insertvalues(PyObject *d)
{
    APIRET    rc;
    ULONG     values[QSV_MAX+1];
    PyObject *v;
    char     *ver, tmp[10];

    Py_BEGIN_ALLOW_THREADS
    rc = DosQuerySysInfo(1, QSV_MAX, &values[1], sizeof(values));
    Py_END_ALLOW_THREADS

    if (rc != NO_ERROR) {
        os2_error(rc);
        return -1;
    }

    if (ins(d, "meminstalled", values[QSV_TOTPHYSMEM])) return -1;
    if (ins(d, "memkernel",    values[QSV_TOTRESMEM])) return -1;
    if (ins(d, "memvirtual",   values[QSV_TOTAVAILMEM])) return -1;
    if (ins(d, "maxpathlen",   values[QSV_MAX_PATH_LENGTH])) return -1;
    if (ins(d, "maxnamelen",   values[QSV_MAX_COMP_LENGTH])) return -1;
    if (ins(d, "revision",     values[QSV_VERSION_REVISION])) return -1;
    if (ins(d, "timeslice",    values[QSV_MIN_SLICE])) return -1;

    switch (values[QSV_VERSION_MINOR]) {
    case 0:  ver = "2.00"; break;
    case 10: ver = "2.10"; break;
    case 11: ver = "2.11"; break;
    case 30: ver = "3.00"; break;
    case 40: ver = "4.00"; break;
    case 50: ver = "5.00"; break;
    default:
        sprintf(tmp, "%d-%d", values[QSV_VERSION_MAJOR],
                              values[QSV_VERSION_MINOR]);
        ver = &tmp[0];
    }

    /* Add Indicator of the Version of the Operating System */
    v = PyString_FromString(ver);
    if (!v || PyDict_SetItemString(d, "version", v) < 0)
        return -1;
    Py_DECREF(v);

    /* Add Indicator of Which Drive was Used to Boot the System */
    tmp[0] = 'A' + values[QSV_BOOT_DRIVE] - 1;
    tmp[1] = ':';
    tmp[2] = '\0';

    v = PyString_FromString(tmp);
    if (!v || PyDict_SetItemString(d, "bootdrive", v) < 0)
        return -1;
    Py_DECREF(v);

    return 0;
}
#endif

static int
all_ins(d)
        PyObject* d;
{
#ifdef F_OK
        if (ins(d, "F_OK", (long)F_OK)) return -1;
#endif        
#ifdef R_OK
        if (ins(d, "R_OK", (long)R_OK)) return -1;
#endif        
#ifdef W_OK
        if (ins(d, "W_OK", (long)W_OK)) return -1;
#endif        
#ifdef X_OK
        if (ins(d, "X_OK", (long)X_OK)) return -1;
#endif        
#ifdef NGROUPS_MAX
        if (ins(d, "NGROUPS_MAX", (long)NGROUPS_MAX)) return -1;
#endif
#ifdef TMP_MAX
        if (ins(d, "TMP_MAX", (long)TMP_MAX)) return -1;
#endif
#ifdef WNOHANG
        if (ins(d, "WNOHANG", (long)WNOHANG)) return -1;
#endif        
#ifdef O_RDONLY
        if (ins(d, "O_RDONLY", (long)O_RDONLY)) return -1;
#endif
#ifdef O_WRONLY
        if (ins(d, "O_WRONLY", (long)O_WRONLY)) return -1;
#endif
#ifdef O_RDWR
        if (ins(d, "O_RDWR", (long)O_RDWR)) return -1;
#endif
#ifdef O_NDELAY
        if (ins(d, "O_NDELAY", (long)O_NDELAY)) return -1;
#endif
#ifdef O_NONBLOCK
        if (ins(d, "O_NONBLOCK", (long)O_NONBLOCK)) return -1;
#endif
#ifdef O_APPEND
        if (ins(d, "O_APPEND", (long)O_APPEND)) return -1;
#endif
#ifdef O_DSYNC
        if (ins(d, "O_DSYNC", (long)O_DSYNC)) return -1;
#endif
#ifdef O_RSYNC
        if (ins(d, "O_RSYNC", (long)O_RSYNC)) return -1;
#endif
#ifdef O_SYNC
        if (ins(d, "O_SYNC", (long)O_SYNC)) return -1;
#endif
#ifdef O_NOCTTY
        if (ins(d, "O_NOCTTY", (long)O_NOCTTY)) return -1;
#endif
#ifdef O_CREAT
        if (ins(d, "O_CREAT", (long)O_CREAT)) return -1;
#endif
#ifdef O_EXCL
        if (ins(d, "O_EXCL", (long)O_EXCL)) return -1;
#endif
#ifdef O_TRUNC
        if (ins(d, "O_TRUNC", (long)O_TRUNC)) return -1;
#endif
#ifdef O_BINARY
        if (ins(d, "O_BINARY", (long)O_BINARY)) return -1;
#endif
#ifdef O_TEXT
        if (ins(d, "O_TEXT", (long)O_TEXT)) return -1;
#endif

#ifdef HAVE_SPAWNV
        if (ins(d, "P_WAIT", (long)_P_WAIT)) return -1;
        if (ins(d, "P_NOWAIT", (long)_P_NOWAIT)) return -1;
        if (ins(d, "P_OVERLAY", (long)_OLD_P_OVERLAY)) return -1;
        if (ins(d, "P_NOWAITO", (long)_P_NOWAITO)) return -1;
        if (ins(d, "P_DETACH", (long)_P_DETACH)) return -1;
#endif

#if defined(PYOS_OS2)
        if (insertvalues(d)) return -1;
#endif
        return 0;
}


#if ( defined(_MSC_VER) || defined(__WATCOMC__) ) && !defined(__QNX__)
#define INITFUNC initnt
#define MODNAME "nt"
#else
#if defined(PYOS_OS2)
#define INITFUNC initos2
#define MODNAME "os2"
#else
#define INITFUNC initposix
#define MODNAME "posix"
#endif
#endif

DL_EXPORT(void)
INITFUNC()
{
	PyObject *m, *d, *v;
	
	m = Py_InitModule4(MODNAME,
			   posix_methods,
			   posix__doc__,
			   (PyObject *)NULL,
			   PYTHON_API_VERSION);
	d = PyModule_GetDict(m);
	
	/* Initialize environ dictionary */
	v = convertenviron();
	if (v == NULL || PyDict_SetItemString(d, "environ", v) != 0)
		return;
	Py_DECREF(v);

        if (all_ins(d))
                return;

        if (setup_confname_tables(d))
                return;

	PyDict_SetItemString(d, "error", PyExc_OSError);

#ifdef HAVE_PUTENV
	posix_putenv_garbage = PyDict_New();
#endif
}
