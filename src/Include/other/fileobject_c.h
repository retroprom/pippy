#ifndef FILEOBJECT_C_H
#define FILEOBJECT_C_H

#include "proto_defs.h"

static PyObject *err_closed() SEG_FILEOBJECT_C;
static size_t new_buffersize Py_PROTO((PyFileObject *, size_t)) SEG_FILEOBJECT_C;
static PyObject *getline Py_PROTO((PyFileObject *, int)) SEG_FILEOBJECT_C;

DESTRUCTOR_PROTO(file_dealloc, PyFileObject) SEG_FILEOBJECT_C;
GETATTRFUNC_PROTO(file_getattr, PyFileObject) SEG_FILEOBJECT_C;
SETATTRFUNC_PROTO(file_setattr, PyFileObject) SEG_FILEOBJECT_C;
REPRFUNC_PROTO(file_repr, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_readline, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_read, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_write, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_fileno, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_seek, PyFileObject) SEG_FILEOBJECT_C;
#ifdef HAVE_FTRUNCATE
BINARYFUNC1_PROTO(file_truncate, PyFileObject) SEG_FILEOBJECT_C;
#endif
BINARYFUNC1_PROTO(file_tell, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_readinto, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_readlines, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_writelines, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_flush, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_close, PyFileObject) SEG_FILEOBJECT_C;
BINARYFUNC1_PROTO(file_isatty, PyFileObject) SEG_FILEOBJECT_C;

#endif /* FILEOBJECT_C_H */

