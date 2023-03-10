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

/* Support for dynamic loading of extension modules */

#include <ctype.h>	/*  for isdigit()	  */
#include <errno.h>	/*  for global errno      */
#include <string.h>	/*  for strerror()        */
#include <stdlib.h>	/*  for malloc(), free()  */
#include <sys/ldr.h>

#include "Python.h"
#include "importdl.h"


#ifdef AIX_GENUINE_CPLUSPLUS
#include "/usr/lpp/xlC/include/load.h"
#define aix_load loadAndInit
#else
#define aix_load load
#endif


extern char *Py_GetProgramName();

typedef struct Module {
	struct Module *next;
	void          *entry;
} Module, *ModulePtr;

const struct filedescr _PyImport_DynLoadFiletab[] = {
	{".so", "rb", C_EXTENSION},
	{"module.so", "rb", C_EXTENSION},
	{0, 0}
};

static int
aix_getoldmodules(modlistptr)
	void **modlistptr;
{
	register ModulePtr       modptr, prevmodptr;
	register struct ld_info  *ldiptr;
	register char            *ldibuf;
	register int             errflag, bufsize = 1024;
	register unsigned int    offset;
	char *progname = Py_GetProgramName();
	
	/*
	-- Get the list of loaded modules into ld_info structures.
	*/
	if ((ldibuf = malloc(bufsize)) == NULL) {
		PyErr_SetString(PyExc_ImportError, strerror(errno));
		return -1;
	}
	while ((errflag = loadquery(L_GETINFO, ldibuf, bufsize)) == -1
	       && errno == ENOMEM) {
		free(ldibuf);
		bufsize += 1024;
		if ((ldibuf = malloc(bufsize)) == NULL) {
			PyErr_SetString(PyExc_ImportError, strerror(errno));
			return -1;
		}
	}
	if (errflag == -1) {
		PyErr_SetString(PyExc_ImportError, strerror(errno));
		return -1;
	}
	/*
	-- Make the modules list from the ld_info structures.
	*/
	ldiptr = (struct ld_info *)ldibuf;
	prevmodptr = NULL;
	do {
		if (strstr(progname, ldiptr->ldinfo_filename) == NULL &&
		    strstr(ldiptr->ldinfo_filename, "python") == NULL) {
			/*
			-- Extract only the modules belonging to the main
			-- executable + those containing "python" as a
			-- substring (like the "python[version]" binary or
			-- "libpython[version].a" in case it's a shared lib).
			*/
			offset = (unsigned int)ldiptr->ldinfo_next;
			ldiptr = (struct ld_info *)((unsigned int)
						    ldiptr + offset);
			continue;
		}
		if ((modptr = (ModulePtr)malloc(sizeof(Module))) == NULL) {
			PyErr_SetString(PyExc_ImportError, strerror(errno));
			while (*modlistptr) {
				modptr = (ModulePtr)*modlistptr;
				*modlistptr = (void *)modptr->next;
				free(modptr);
			}
			return -1;
		}
		modptr->entry = ldiptr->ldinfo_dataorg;
		modptr->next  = NULL;
		if (prevmodptr == NULL)
			*modlistptr = (void *)modptr;
		else
			prevmodptr->next = modptr;
		prevmodptr = modptr;
		offset = (unsigned int)ldiptr->ldinfo_next;
		ldiptr = (struct ld_info *)((unsigned int)ldiptr + offset);
	} while (offset);
	free(ldibuf);
	return 0;
}

static int
aix_bindnewmodule(newmoduleptr, modlistptr)
	void *newmoduleptr;
	void *modlistptr;        
{
	register ModulePtr modptr;

	/*
	-- Bind the new module with the list of loaded modules.
	*/
	for (modptr = (ModulePtr)modlistptr; modptr; modptr = modptr->next)
		if (loadbind(0, modptr->entry, newmoduleptr) != 0)
			return -1;
	return 0;
}

static void
aix_loaderror(pathname)
	char *pathname;
{

	char *message[1024], errbuf[1024];
	register int i,j;

	struct errtab { 
		int errNo;
		char *errstr;
	} load_errtab[] = {
		{L_ERROR_TOOMANY,	"too many errors, rest skipped."},
		{L_ERROR_NOLIB,		"can't load library:"},
		{L_ERROR_UNDEF,		"can't find symbol in library:"},
		{L_ERROR_RLDBAD,
		 "RLD index out of range or bad relocation type:"},
		{L_ERROR_FORMAT,	"not a valid, executable xcoff file:"},
		{L_ERROR_MEMBER,
		 "file not an archive or does not contain requested member:"},
		{L_ERROR_TYPE,		"symbol table mismatch:"},
		{L_ERROR_ALIGN,		"text alignment in file is wrong."},
		{L_ERROR_SYSTEM,	"System error:"},
		{L_ERROR_ERRNO,		NULL}
	};

#define LOAD_ERRTAB_LEN	(sizeof(load_errtab)/sizeof(load_errtab[0]))
#define ERRBUF_APPEND(s) strncat(errbuf, s, sizeof(errbuf)-strlen(errbuf)-1)

	sprintf(errbuf, "from module %.200s ", pathname);

	if (!loadquery(L_GETMESSAGES, &message[0], sizeof(message))) {
		ERRBUF_APPEND(strerror(errno));
		ERRBUF_APPEND("\n");
	}
	for(i = 0; message[i] && *message[i]; i++) {
		int nerr = atoi(message[i]);
		for (j=0; j<LOAD_ERRTAB_LEN ; j++) {
		    if (nerr == load_errtab[j].errNo && load_errtab[j].errstr)
			ERRBUF_APPEND(load_errtab[j].errstr);
		}
		while (isdigit(*message[i])) message[i]++ ; 
		ERRBUF_APPEND(message[i]);
		ERRBUF_APPEND("\n");
	}
	errbuf[strlen(errbuf)-1] = '\0';	/* trim off last newline */
	PyErr_SetString(PyExc_ImportError, errbuf); 
	return; 
}


dl_funcptr _PyImport_GetDynLoadFunc(const char *fqname, const char *shortname,
				    const char *pathname, FILE *fp)
{
	dl_funcptr p;

	/*
	-- Invoke load() with L_NOAUTODEFER leaving the imported symbols
	-- of the shared module unresolved. Thus we have to resolve them
	-- explicitely with loadbind. The new module is loaded, then we
	-- resolve its symbols using the list of already loaded modules
	-- (only those that belong to the python executable). Get these
	-- with loadquery(L_GETINFO).
	*/

	static void *staticmodlistptr = NULL;

	if (!staticmodlistptr)
		if (aix_getoldmodules(&staticmodlistptr) == -1)
			return NULL;
	p = (dl_funcptr) aix_load((char *)pathname, L_NOAUTODEFER, 0);
	if (p == NULL) {
		aix_loaderror(pathname);
		return NULL;
	}
	if (aix_bindnewmodule((void *)p, staticmodlistptr) == -1) {
		aix_loaderror(pathname);
		return NULL;
	}

	return p;
}
