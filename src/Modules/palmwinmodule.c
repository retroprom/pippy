/*

  palmwin module

  This module provides an interface to PalmOS window and graphics functions.

*/

#include "Python.h"
#include <PalmOS.h>
#include <Window.h>

static PyObject *palmwin_error;

static PyObject *palmwin_drawpixel(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;
static PyObject *palmwin_drawrect(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;
static PyObject *palmwin_drawinvertchars(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;
static PyObject *palmwin_drawline(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;
static PyObject *palmwin_erasewin(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;
static PyObject *palmwin_drawchars(PyObject *self, PyObject *args) SEG_PALMWINMODULE_C;

/*
TODO: 
  o Fill/Patterns
  o WinSetUnderlineMode
  o Fonts
  o WinGetDisplayExtent / WinGetWindowExtent
  o Example/test programs
  o Offscreen drawing?
  o All the window methods?
*/

void initpalmwin(void) SEG_PALMWINMODULE_C;

static PyObject *
palmwin_drawchars(PyObject *self, PyObject *args)
{
	char *chars;
	int x, y;
	
	if (!PyArg_ParseTuple(args, "sii", &chars, &x, &y))
		return NULL;

  WinDrawChars(chars, strlen(chars), x, y);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmwin_drawpixel(PyObject *self, PyObject *args)
{
	int x, y;
	
	if (!PyArg_ParseTuple(args, "ii", &x, &y))
		return NULL;

  WinDrawPixel(x, y);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmwin_drawinvertchars(PyObject *self, PyObject *args)
{
	char *chars;
	int x, y;
	
	if (!PyArg_ParseTuple(args, "sii", &chars, &x, &y))
		return NULL;

  WinDrawInvertedChars(chars, strlen(chars), x, y);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmwin_drawline(PyObject *self, PyObject *args)
{
	int x0, y0, x1, y1;
	
	if (!PyArg_ParseTuple(args, "iiii", &x0, &y0, &x1, &y1))
		return NULL;

  WinDrawLine(x0, y0, x1, y1);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmwin_drawrect(PyObject *self, PyObject *args)
{
  PointType pos, size;
  RectangleType rect;
  int x0, y0, x1, y1;
	
	if (!PyArg_ParseTuple(args, "iiii", &x0, &y0, &x1, &y1))
    return NULL;

  rect.topLeft.x = x0;
  rect.topLeft.y = y0;
  rect.extent.x = x1;
  rect.extent.y = y1;
  
  WinDrawRectangle(&rect, 0);

	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject *
palmwin_erasewin(PyObject *self, PyObject *args)
{
  WinEraseWindow();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef palmwin_methods[] = {
	{"drawpixel",  (PyCFunction)palmwin_drawpixel, METH_VARARGS},
	{"drawchars",  (PyCFunction)palmwin_drawchars, METH_VARARGS},
	{"drawinvertchars",  (PyCFunction)palmwin_drawinvertchars, METH_VARARGS},
	{"drawline",  (PyCFunction)palmwin_drawline, METH_VARARGS},
	{"drawrect",  (PyCFunction)palmwin_drawrect, METH_VARARGS},
	{"erasewin",  (PyCFunction)palmwin_erasewin, METH_VARARGS},
	{NULL, NULL} };


void initpalmwin(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmwin", palmwin_methods, NULL);
	d = PyModule_GetDict(m);
	palmwin_error = PyErr_NewException("palmwin.error", NULL, NULL);
	if (palmwin_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmwin_error);

}
