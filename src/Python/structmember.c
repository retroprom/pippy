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

/* Map C struct members to Python object attributes */

#include "Python.h"

#include "structmember.h"
#include "other/structmember_c.h"

static PyObject *
listmembers(mlist)
	struct memberlist *mlist;
{
	int i, n;
	PyObject *v;
	for (n = 0; mlist[n].name != NULL; n++)
		;
	v = PyList_New(n);
	if (v != NULL) {
		for (i = 0; i < n; i++)
			PyList_SetItem(v, i,
				       PyString_FromString(mlist[i].name));
		if (PyErr_Occurred()) {
			Py_DECREF(v);
			v = NULL;
		}
		else {
			PyList_Sort(v);
		}
	}
	return v;
}

PyObject *
PyMember_Get(addr, mlist, name)
	char *addr;
	struct memberlist *mlist;
	char *name;
{
	struct memberlist *l;
	
	if (strcmp(name, "__members__") == 0)
		return listmembers(mlist);
	for (l = mlist; l->name != NULL; l++) {
		if (strcmp(l->name, name) == 0) {
			PyObject *v;
			addr += l->offset;
			switch (l->type) {
			case T_BYTE:
				v = PyInt_FromLong((long)
						 (((*(char*)addr & 0xff)
						   ^ 0x80) - 0x80));
				break;
			case T_UBYTE:
				v = PyInt_FromLong((long) *(char*)addr & 0xff);
				break;
			case T_SHORT:
				v = PyInt_FromLong((long) *(short*)addr);
				break;
			case T_USHORT:
				v = PyInt_FromLong((long)
						 *(unsigned short*)addr);
				break;
			case T_INT:
				v = PyInt_FromLong((long) *(int*)addr);
				break;
			case T_UINT:
				v = PyInt_FromLong((long)
						   *(unsigned int*)addr);
				break;
			case T_LONG:
				v = PyInt_FromLong(*(long*)addr);
				break;
			case T_ULONG:
				v = PyLong_FromDouble((double)
						   *(unsigned long*)addr);
				break;
			case T_FLOAT:
#ifndef WITHOUT_FLOAT
				v = PyFloat_FromDouble((double)*(float*)addr);
#else /* !WITHOUT_FLOAT */
				PyErr_SetString(PyExc_MissingFeatureError,
			        "Float objects are not provided in this python build");
				v = NULL;
#endif /* !WITHOUT_FLOAT */
				break;
			case T_DOUBLE:
#ifndef WITHOUT_FLOAT
				v = PyFloat_FromDouble(*(double*)addr);
#else /* !WITHOUT_FLOAT */
				PyErr_SetString(PyExc_MissingFeatureError,
			        "Float objects are not provided in this python build");
				v = NULL;
#endif /* !WITHOUT_FLOAT */
				break;
			case T_STRING:
				if (*(char**)addr == NULL) {
					Py_INCREF(Py_None);
					v = Py_None;
				}
				else
					v = PyString_FromString(*(char**)addr);
				break;
			case T_STRING_INPLACE:
				v = PyString_FromString((char*)addr);
				break;
#ifdef macintosh
			case T_PSTRING:
				if (*(char**)addr == NULL) {
					Py_INCREF(Py_None);
					v = Py_None;
				}
				else
					v = PyString_FromStringAndSize(
						(*(char**)addr)+1,
						**(unsigned char**)addr);
				break;
			case T_PSTRING_INPLACE:
				v = PyString_FromStringAndSize(
					((char*)addr)+1,
					*(unsigned char*)addr);
				break;
#endif /* macintosh */
			case T_CHAR:
				v = PyString_FromStringAndSize((char*)addr, 1);
				break;
			case T_OBJECT:
				v = *(PyObject **)addr;
				if (v == NULL)
					v = Py_None;
				Py_INCREF(v);
				break;
			default:
				PyErr_SetString(PyExc_SystemError,
						"bad memberlist type");
				v = NULL;
			}
			return v;
		}
	}
	
	PyErr_SetString(PyExc_AttributeError, name);
	return NULL;
}

int
PyMember_Set(addr, mlist, name, v)
	char *addr;
	struct memberlist *mlist;
	char *name;
	PyObject *v;
{
	struct memberlist *l;
	PyObject *oldv;
	
	for (l = mlist; l->name != NULL; l++) {
		if (strcmp(l->name, name) == 0) {
#ifdef macintosh
			if (l->readonly || l->type == T_STRING ||
			    l->type == T_PSTRING)
			{
#else
			if (l->readonly || l->type == T_STRING ) {
#endif /* macintosh */
				PyErr_SetString(PyExc_TypeError,
						"readonly attribute");
				return -1;
			}
			if (v == NULL && l->type != T_OBJECT) {
				PyErr_SetString(PyExc_TypeError,
				  "can't delete numeric/char attribute");
				return -1;
			}
			addr += l->offset;
			switch (l->type) {
			case T_BYTE:
			case T_UBYTE:
				if (!PyInt_Check(v)) {
					PyErr_BadArgument();
					return -1;
				}
				*(char*)addr = (char) PyInt_AsLong(v);
				break;
			case T_SHORT:
			case T_USHORT:
				if (!PyInt_Check(v)) {
					PyErr_BadArgument();
					return -1;
				}
				*(short*)addr = (short) PyInt_AsLong(v);
				break;
			case T_UINT:
			case T_INT:
				if (!PyInt_Check(v)) {
					PyErr_BadArgument();
					return -1;
				}
				*(int*)addr = (int) PyInt_AsLong(v);
				break;
			case T_LONG:
				if (!PyInt_Check(v)) {
					PyErr_BadArgument();
					return -1;
				}
				*(long*)addr = PyInt_AsLong(v);
				break;
			case T_ULONG:
				if (PyInt_Check(v))
					*(long*)addr = PyInt_AsLong(v);
				else if (PyLong_Check(v))
					*(long*)addr = PyLong_AsLong(v);
				else {
					PyErr_BadArgument();
					return -1;
				}
				break;
			case T_FLOAT:
#ifndef WITHOUT_FLOAT
				if (PyInt_Check(v))
					*(float*)addr =
						(float) PyInt_AsLong(v);
				else if (PyFloat_Check(v))
					*(float*)addr =
						(float) PyFloat_AsDouble(v);
				else {
					PyErr_BadArgument();
					return -1;
				}
#else /* !WITHOUT_FLOAT */
				PyErr_SetString(PyExc_MissingFeatureError,
			        "Float objects are not provided in this python build");
				return -1;
#endif /* !WITHOUT_FLOAT */
				break;
			case T_DOUBLE:
#ifndef WITHOUT_FLOAT
				if (PyInt_Check(v))
					*(double*)addr =
						(double) PyInt_AsLong(v);
				else if (PyFloat_Check(v))
					*(double*)addr = PyFloat_AsDouble(v);
				else {
					PyErr_BadArgument();
					return -1;
				}
#else /* !WITHOUT_FLOAT */
				PyErr_SetString(PyExc_MissingFeatureError,
			        "Float objects are not provided in this python build");
				return -1;
#endif /* !WITHOUT_FLOAT */
				break;
			case T_OBJECT:
				Py_XINCREF(v);
				oldv = *(PyObject **)addr;
				*(PyObject **)addr = v;
				Py_XDECREF(oldv);
				break;
			case T_CHAR:
				if (PyString_Check(v) &&
				    PyString_Size(v) == 1) {
					*(char*)addr =
						PyString_AsString(v)[0];
				}
				else {
					PyErr_BadArgument();
					return -1;
				}
			default:
				PyErr_SetString(PyExc_SystemError,
						"bad memberlist type");
				return -1;
			}
			return 0;
		}
	}
	
	PyErr_SetString(PyExc_AttributeError, name);
	return -1;
}
