
#include "Python.h"
#include <PalmOS.h>
#include "_palmutils.h"

/* must fix the following include - put somewhere else */
#include "commonRsc.h"


/* palmutil_buildErrObject:
   
   Build a Python string object containing the palmnet error and 
   return the result.  Callers are handed ownership of the resulting
   object and should decrement the reference when finished with it.

*/

PyObject *
palmutil_buildErrObject(UInt16 ifErrs) 
{
	UInt16 maxLen=30;
	char netErrString[maxLen];
	char errnoString[maxLen];
	/* Grab the error id from the errno.  Extract last byte of 0x1200
	   error offset. */
	Err i = (ifErrs & 0x00FF) - 1; /* SysStringByIndex is 0 based; errors are 1-based */

/* 	SysStringByIndex (netErrStringCodes_tSTL, i, netErrString, maxLen); */
	SysErrString(ifErrs, netErrString, maxLen);
	SysStringByIndex (errnoStringCodes_tSTL, i, errnoString, maxLen);
	return Py_BuildValue("(iss)", (int)ifErrs, errnoString, netErrString);
	
}


PyObject *
palmutil_setErr(PyObject *exc, Err err)
{
	char msg[80];
	SysErrString(err, msg, 80);
	PyErr_SetString(exc, msg);
	return NULL;
}

PyObject *
palmutil_setErr_FromDMError(PyObject *exc)
{
	return palmutil_setErr(exc, DmGetLastErr());
}
