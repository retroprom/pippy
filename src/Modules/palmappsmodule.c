/* 
   This module contains the interfaces to the four main Palm applications.
*/


#include "Python.h"
#include <PalmOS.h>
#define IGNORE_UNISTD_STUBS  /* causes portions of sys_socket.h to be ignored */
#define IGNORE_STDLIB_STUBS  /* causes portions of sys_socket.h to be ignored */
#define IGNORE_SYS_TYPES  /* portions of sys_types.h ignored */
#define __stdarg_h  /* causes unix_stdarg.h be effectively ignored */
#define __stdio_h  /* causes unix_stdio.h to be ignored */
#include "sys_types.h"
#include "_palmutils.h"
#include "AddressDB.h"
#include "datebook.h"
#include "MemoDB.h"
#include "ToDoDB.h"

static PyObject *palmapps_error;

static char *_get_memptr(PyObject *args, PyObject **Oret) SEG_APPSMODULE_C;
static PyObject *palmapps_memo(PyObject *self, PyObject *args) SEG_APPSMODULE_C;
static PyObject *palmapps_datebook(PyObject *self, PyObject *args) SEG_APPSMODULE_C;
static PyObject *palmapps_todo(PyObject *self, PyObject *args) SEG_APPSMODULE_C;
static PyObject *palmapps_addrbook(PyObject *self, PyObject *args) SEG_APPSMODULE_C;
void initpalmapps(void) SEG_APPSMODULE_C;


static char *
_get_memptr(PyObject *args, PyObject **Oret)
{
	char *rec;
	PyObject *o;

	if (!PyArg_ParseTuple(args, "O", &o))
		return NULL;
	if (PyBuffer_Check(o)){
		/* handle buffer - be sure to grab the base */
		rec = (char*) ((PyBufferObject *)o)->b_ptr;
	}
	else if (PyString_Check(o)){
		/* handle as string */
		rec = (char *) ((PyStringObject *)o)->ob_sval;
	}
	else {
		PyErr_SetString(PyExc_ValueError,
				"Argument must be a Buffer or String");
		return NULL;
	}
	/* set the return object */
	*Oret = o;

	return rec;
}

static PyObject *
palmapps_memo(PyObject *self, PyObject *args)
{
	PyObject *o;
	char *rec;
	
	if (!(rec = _get_memptr(args, &o)))
		return NULL;
	
	/* return result as a buffer */
	return PyBuffer_FromObject(o, 0, strlen(rec));
}

static PyObject *
palmapps_datebook(PyObject *self, PyObject *args)
{
	PyObject *o;
	char *rec;
	PyObject *b1, *b2=NULL;
	int off, off2, l;
	PackedApptDBRecordType *ap;

	if (!(rec = _get_memptr(args, &o)))
		return NULL;
	
	ap = (PackedApptDBRecordType *) rec;
	off = sizeof(PackedApptDBRecordType);
	if ( ap->flags.alarm )   /* skip alarm */
		off += sizeof(AlarmInfoType);
	if ( ap->flags.repeat )   /* skip repeat */
		off += sizeof(RepeatInfoType);
	if ( ap->flags.exceptions )   /* skip exceptions */
		off += sizeof(ExceptionsListType);

	l = strlen(&rec[off]);
	b1 = PyBuffer_FromObject(o, off, l);
	
	off2 = off + l + 1;
	if (!b1)
		return NULL;
	if ( ap->flags.note ){   /* note is present */
		b2 = PyBuffer_FromObject(o, off2, strlen(&rec[off2]));
		if (!b2)
			return NULL;
	}
      
	if (b2)
		return Py_BuildValue("(OO)", b1, b2);
	else {
		Py_INCREF(Py_None);
		return Py_BuildValue("(OO)", b1, Py_None);
	}
}

static PyObject *
palmapps_todo(PyObject *self, PyObject *args)
{
	PyObject *o;
	char *rec;
	char *desc, *note;
	PyObject *b1, *b2, *priority;
	int off1, off2;

	if (!(rec = _get_memptr(args, &o)))
		return NULL;
	
	/* skip the first 3 bytes */
	off1 = 3;
	desc = &rec[off1];

	off2 = off1 + strlen(desc)+1;
	note = &rec[off2];
	
	b1 = PyBuffer_FromObject(o, off1, strlen(desc));
	if (!b1)
		return NULL;
	b2 = PyBuffer_FromObject(o, off2, strlen(note));
	if (!b2)
		return NULL;
	priority = PyInt_FromLong((long)(rec[2] & 0xff));
	if (!priority)
		return NULL;
	return Py_BuildValue("(OOO)", priority, b1, b2);
}

static PyObject *
palmapps_addrbook(PyObject *self, PyObject *args)
{
	char *rec;
	PyObject *o;
	struct header {
		unsigned long p;
		unsigned long flags;
	} h;
	struct header *hp = &h;
	int item_off[19];
	int off;
	int i;
	PyObject *t, *buf;

	if (!(rec = _get_memptr(args, &o)))
		return NULL;
	hp = (struct header *) rec;
	off = 9;
	for (i = 0; i < 19; i++) {
		if (hp->flags & (1L << i)) {
			item_off[i] = off;
			off += strlen(&rec[item_off[i]]) + 1;
		} else
			item_off[i] = -1;
	}

	if (!(t = PyTuple_New(19)))
	    return NULL;

	for (i=0; i < 19; i++){
		off = item_off[i];
		if (off != -1) {
			buf = PyBuffer_FromObject(o, off, strlen(&rec[off]));
			if (!buf)
				return NULL;
			PyTuple_SET_ITEM(t, i, buf);
		} 
		else {
			Py_INCREF(Py_None);
			PyTuple_SET_ITEM(t, i, Py_None);
		}
	}
	return t;
}


static PyMethodDef palmapps_methods[] = {
	{"memo",  (PyCFunction)palmapps_memo, METH_VARARGS},
	{"datebook",  (PyCFunction)palmapps_datebook, METH_VARARGS},
	{"todo",  (PyCFunction)palmapps_todo, METH_VARARGS},
	{"addrbook",  (PyCFunction)palmapps_addrbook, METH_VARARGS},
	{NULL, NULL} };

void initpalmapps(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmapps", palmapps_methods, NULL);
	d = PyModule_GetDict(m);
	palmapps_error = PyErr_NewException("palmapps.error", NULL, NULL);
	if (palmapps_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmapps_error);

}
