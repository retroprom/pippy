/*

  palmsys module

  This module provides an interface to various PalmOS system functions.

*/

#include "Python.h"
#include <PalmOS.h>

static PyObject *palmsys_error;

static PyObject *palmsys_getstackinfo(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_reset(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_ticksPerSecond(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_batteryinfo(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static Int32 get_heapid(PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_heapcompact(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_heapfreebytes(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
static PyObject *palmsys_heapid(PyObject *self, PyObject *args) SEG_PALMSYSMODULE_C;
void initpalmsys(void) SEG_PALMSYSMODULE_C;


static PyObject *
palmsys_getstackinfo(PyObject *self, PyObject *args)
{
	Boolean stack_not_overflow;
	MemPtr start, end;
	PyObject *res = NULL;

	if (!PyArg_NoArgs(args))
		return NULL;

	stack_not_overflow = SysGetStackInfo (&start, &end);

	if (!(res = Py_BuildValue("(ill)", stack_not_overflow, start, end)))
		return NULL;

	return res;
}

static PyObject *
palmsys_reset(PyObject *self, PyObject *args)
{
	/* perform a soft reset of the device */
	if (!PyArg_NoArgs(args))
		return NULL;

	SysReset ();

	/* will never get here, so just add these lines to please
	   the compiler */
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmsys_ticksPerSecond(PyObject *self, PyObject *args)
{
	/* perform a soft reset of the device */
	if (!PyArg_NoArgs(args))
		return NULL;

	return PyInt_FromLong((long)SysTicksPerSecond());
}

/* batteryinfo returns a tuple describing the state of the battery. */

static PyObject *
palmsys_batteryinfo(PyObject *self, PyObject *args)
{
	UInt16 volts_100;
	UInt16 warnThresh, criticalThresh, maxTicks;
	SysBatteryKind kind;
	Boolean pluggedIn;
	UInt8 percent;
	char *s;
		
	/* perform a soft reset of the device */
	if (!PyArg_NoArgs(args))
		return NULL;

	volts_100 = SysBatteryInfo (0, &warnThresh, &criticalThresh, 
				    &maxTicks, &kind, &pluggedIn, &percent);
	
	switch(kind) {
	case sysBatteryKindAlkaline:
		s = "Alkaline";
		break;
	case sysBatteryKindNiCad:
		s = "NiCad";
		break;
	case sysBatteryKindLiIon:
		s = "LiIon";
		break;
	case sysBatteryKindRechAlk:
		s = "RechAlk";
		break;
	case sysBatteryKindNiMH:
		s = "NiMH";
		break;
	case sysBatteryKindLiIon1400:
		s = "LiIon1400";
		break;
	default:
		s = "Unknown";
	}

	return Py_BuildValue("(iiiisii)", volts_100, warnThresh, criticalThresh,
			     maxTicks, s, (UInt16) pluggedIn, 
			     (UInt16) percent);
}

static Int32
get_heapid(PyObject *args)
{
	UInt16 card, heap_index;
	Int32 res = -1; /* really want UInt16, but need to return a
			   negative result if an error occurs */

	if (!PyArg_ParseTuple(args, "ii", &card, &heap_index))
		return res;
	if (card != 0){
		PyErr_SetString(palmsys_error, "card must be 0");
		return res;
	}
	if (heap_index < 0 || heap_index > MemNumHeaps(card)-1) {
		PyErr_SetString(palmsys_error, "heap index is out of range");
		return res;
	}

	return MemHeapID (card, heap_index);
}


static PyObject *
palmsys_heapcompact(PyObject *self, PyObject *args)
{
	Int32 id;

	if ((id=get_heapid(args)) < 0)
		return NULL;

	/* always returns 0 */
	MemHeapCompact((UInt16)id);

	Py_INCREF(Py_None);
	return Py_None;
}


	
static PyObject *
palmsys_heapfreebytes(PyObject *self, PyObject *args)
{
	Int32 id;
	UInt32 amt_free, max_contig_free;

	if ((id=get_heapid(args)) < 0)
		return NULL;

	/* always returns 0 */
	MemHeapFreeBytes ((UInt16)id, &amt_free, &max_contig_free);

	return Py_BuildValue("(ll)", amt_free, max_contig_free);
}

static PyObject *
palmsys_heapid(PyObject *self, PyObject *args)
{
	Int32 res;
	if ((res=get_heapid(args)) < 0)
		return NULL;
	return PyInt_FromLong((UInt32)res);
}

static PyObject *
palmsys_displayError(PyObject *self, PyObject *args)
{
	char *msg;
	int cond;

	if (!PyArg_ParseTuple(args, "is", &cond, &msg))
		return NULL;
	
	ErrFatalDisplayIf(cond, msg);

	Py_INCREF(Py_None);
	return Py_None;
}	

static PyMethodDef palmsys_methods[] = {
	{"getstackinfo",  (PyCFunction)palmsys_getstackinfo, 0},
	{"reset",  (PyCFunction)palmsys_reset, 0},
	{"ticksPerSecond",  (PyCFunction)palmsys_ticksPerSecond, 0},
	{"batteryinfo",  (PyCFunction)palmsys_batteryinfo, 0},
	{"heapcompact",  (PyCFunction)palmsys_heapcompact, METH_VARARGS},
	{"heapfreebytes",  (PyCFunction)palmsys_heapfreebytes, METH_VARARGS},
	{"heapid",  (PyCFunction)palmsys_heapid, METH_VARARGS},
	{"displayError",  (PyCFunction)	palmsys_displayError, METH_VARARGS},
	{NULL, NULL} };




void initpalmsys(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmsys", palmsys_methods, NULL);
	d = PyModule_GetDict(m);
	palmsys_error = PyErr_NewException("palmsys.error", NULL, NULL);
	if (palmsys_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmsys_error);

}
