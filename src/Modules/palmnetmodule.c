/*
  palmnet module

  This module provides an interface to the PalmOS NetLib networking
  library.  It is used for starting and stopping TCP/IP networking,
  setting the interface (ppp, etc.), and debugging.

*/
#include "kludge.h"
#include "_palmutils.h"
#include "netutil.h"

static PyObject *palmnet_error;
extern UInt16 AppNetRefnum;

/* AppNetOpen is non-zero when the Net library has been opened */
UInt16 AppNetOpen = 0;


static PyObject *palmnet_Err(UInt16 err) SEG_PALMNETMODULE_C;
static PyObject *palmnet_ErrFromErrno() SEG_PALMNETMODULE_C;
static PyObject *palmnet_settimeout(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *palmnet_gettimeout(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *palmnet_isopen(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *palmnet_open(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *palmnet_close(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *init_helper() SEG_PALMNETMODULE_C;
static PyObject *palmnet_refnum(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static PyObject *palmnet_refcount(PyObject *self, PyObject *args) SEG_PALMNETMODULE_C;
static int init_syslib() SEG_PALMNETMODULE_C;
static void palmnet_fini(void) SEG_PALMNETMODULE_C;
static void _close(void) SEG_PALMNETMODULE_C;
PyObject *Palm_InitNetwork() SEG_PALMNETMODULE_C;
int PalmNet_EnsureOpen() SEG_PALMNETMODULE_C;
int PalmNet_IsOpen(void) SEG_PALMNETMODULE_C;
void initpalmnet(void) SEG_PALMNETMODULE_C;



static PyObject *
palmnet_Err(UInt16 err)
{
	PyObject *v;

	v = palmutil_buildErrObject(errno);
	if (v != NULL) {
		PyErr_SetObject(palmnet_error, v);
		Py_DECREF(v);
	}

	return NULL;
}
	

static PyObject *
palmnet_ErrFromErrno()
{
	return palmnet_Err(errno);
}

static PyObject *palmnet_settimeout(PyObject *self, PyObject *args)
{

	/*  Set the timeout in milliseconds.  This value represents the
	    maximum number of system ticks to wait before a net library call
	    expires.
	*/
	
	long timeout;
	PyObject *o;

	if (!PyArg_ParseTuple(args, "O:timeout", &o)) { 
		return NULL;
	}
	if ( o == Py_None )
		timeout = -1;
	else if (PyInt_Check(o))
		timeout = PyInt_AS_LONG(o);
	else {
		PyErr_SetString(PyExc_ValueError, "Argument must be an integer or None");
		return NULL;
	}

	netutil_settimeout(timeout);

	Py_INCREF(Py_None);
	return Py_None;

}

static PyObject *palmnet_gettimeout(PyObject *self, PyObject *args) 
{

	/*  Get the timeout in milliseconds.  This value represents the
	    maximum number of system ticks to wait before a net library call
	    expires.
	*/
	return PyInt_FromLong( (long) netutil_gettimeout());
}

static void _close(void){
	if (AppNetOpen) {
		NetLibClose(AppNetRefnum, false);
		AppNetOpen = 0;
	}
}

static PyObject *palmnet_close(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	_close();
	Py_INCREF(Py_None);
	return Py_None;
}


static void palmnet_fini(void){
	/* Close the Palmos Network Library.

	   This function is registered in the list of functions
	   to be called when the python interpreter is exited.

	 */

	_close();

}


static PyObject *palmnet_refcount(PyObject *self, PyObject *args) {
	UInt16 count;
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	if (!AppNetRefnum && !init_syslib())
		return NULL;
	NetLibOpenCount (AppNetRefnum, &count);
	return PyInt_FromLong((long) count);
}

static PyObject *palmnet_refnum(PyObject *self, PyObject *args) {
	
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	
	return PyInt_FromLong((long) AppNetRefnum);
}


static int init_syslib() {
	int ret = 0;
	Err err;

	err = SysLibFind("Net.lib", &AppNetRefnum);
	if (!AppNetRefnum){
		PyErr_SetString(palmnet_error,"Net.lib cannot be found - this is bad!");
		goto fail;
	}
	if (err == sysErrLibNotFound) {
		PyErr_SetString(palmnet_error, "Palm Net Library is not present.");
		goto fail;
	}
	else if (err) {
		PyErr_SetString(palmnet_error, "Problem loading Palm Net Library");
		goto fail;
	}

	ret = 1;
  fail:
	return ret;

}

	
static PyObject *init_helper(){
	/* 
	   Initialize the connection by opening the PalmOS Network Library.
	   On success, AppNetRefnum is set to a non-zero number and
	   Py_None is returned;  otherwise, raise an exception.
	*/
	Err err;
	UInt16 ifErrs;
	
	if (!AppNetRefnum && !init_syslib())
		return NULL;
	
	if (AppNetOpen){
		Py_INCREF(Py_None);
		return Py_None;
	}

	/* Open the net library */
	err = NetLibOpen(AppNetRefnum, &ifErrs); 
	
	switch (err) {
	case 0: break;
	case netErrAlreadyOpen:
		/* Net already open - ref count on net lib will be 
		   incremented automatically by PalmOS */
		break;
	default:
		return palmnet_Err(err);
	}
	
	/* check for network interface-related errors */
	if (ifErrs)
		return palmnet_Err(ifErrs);

	AppNetOpen=1;
	Py_INCREF(Py_None);
	return Py_None;
	
	}

static PyObject *palmnet_open(PyObject *self, PyObject *args) {
	return init_helper();
}

static PyObject *palmnet_isopen(PyObject *self, PyObject *args) {
	return Py_BuildValue( "i", (AppNetOpen != 0));
}

int PalmNet_IsOpen(void) {
	return (AppNetOpen != 0);
}
int PalmNet_EnsureOpen(){
	if (!AppNetOpen) {
	  PyErr_SetString(palmnet_error, "Net Library has not been opened");
		return -1;
	}
	return 0;
}


PyObject *Palm_InitNetwork() {
	/* exported interface */
	return init_helper();
}


static PyMethodDef palmnet_methods[] = {
	{"settimeout",		(PyCFunction)palmnet_settimeout, METH_VARARGS},
	{"gettimeout",		(PyCFunction)palmnet_gettimeout, METH_VARARGS},
	{"open",                (PyCFunction)palmnet_open, METH_VARARGS},
	{"isopen",              (PyCFunction)palmnet_isopen, METH_VARARGS},
	{"refnum",              (PyCFunction)palmnet_refnum, METH_VARARGS},
	{"refcount",            (PyCFunction)palmnet_refcount, METH_VARARGS},
	{"close",               (PyCFunction)palmnet_close, METH_VARARGS},
	{NULL, NULL} };


void initpalmnet(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmnet", palmnet_methods, NULL);
	d = PyModule_GetDict(m);
	palmnet_error = PyErr_NewException("palmnet.error", NULL, NULL);
	if (palmnet_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmnet_error);

	/* Try to obtain the AppNetRefnum, and ignore the errors if it
	   fails.  Another attempt will be made in the "open" global 
	   routine, */
	init_syslib();
	if (PyErr_Occurred)
		PyErr_Clear();

	Py_AtExit(palmnet_fini);

}
