#include "Python.h"
#include <PalmOS.h>
#include "palmglue.h"
#include "palm_lib.h"


int initialize_FrozenModules();
int cleanup_FrozenModules();
extern struct _frozen _PyImport_FrozenModules[];

void python_init() {

	int verbose = 1;
	int res;

	DMESSAGE("starting python_init");

	DMESSAGE("before set_flags");
	set_flags(0, /* debug */
		  verbose, /* verbose */
		  0, /* interactive */
		  1, /* no site */
		  0, /* use class exceptions */
		  1, /* frozen */
		  0 /* optimize */
		  );


	DMESSAGE("before initialize_FrozenModules");
	res = initialize_FrozenModules();
	ErrFatalDisplayIf( !res, "Problem with loading frozen modules");

	/* put the following statement in frozenmodule_importer */

	DMESSAGE("before set_frozen_tab");
	set_frozen_tab(_PyImport_FrozenModules);

	DMESSAGE("before Py_Initialize()");
	Py_Initialize();
	DMESSAGE("before cleanup_FrozenModules()");
	res = cleanup_FrozenModules();
	ErrFatalDisplayIf( !res,
			   "Problem during frozen module cleanup");

	if (verbose)
		fprintf(stderr, "Python %s\n%s\n",
			Py_GetVersion(), Py_GetCopyright());
	
	PySys_SetArgv(0, NULL);
	
}

void python_finalize(){
	Py_Finalize();
	DMESSAGE("done python_finalize")
}
