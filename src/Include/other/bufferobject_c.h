#ifndef BUFFEROBJECT_C_H
#define BUFFEROBJECT_C_H

#include "proto_defs.h"

static PyObject *_PyBuffer_FromObject Py_PROTO((PyObject *, int, int,getreadbufferproc, int)) SEG_BUFFEROBJECT_C;

DESTRUCTOR_PROTO(buffer_dealloc, PyBufferObject) SEG_BUFFEROBJECT_C;
CMPFUNC_PROTO(buffer_compare, PyBufferObject) SEG_BUFFEROBJECT_C;
REPRFUNC_PROTO(buffer_repr, PyBufferObject) SEG_BUFFEROBJECT_C;
HASHFUNC_PROTO(buffer_hash, PyBufferObject) SEG_BUFFEROBJECT_C;
UNARYFUNC_PROTO(buffer_str, PyBufferObject) SEG_BUFFEROBJECT_C;
INQUIRY_PROTO(buffer_length, PyBufferObject) SEG_BUFFEROBJECT_C;
static PyObject *buffer_concat Py_PROTO((PyBufferObject *self, PyObject *other)) SEG_BUFFEROBJECT_C;
INTARGFUNC_PROTO(buffer_repeat, PyBufferObject) SEG_BUFFEROBJECT_C;
INTARGFUNC_PROTO(buffer_item, PyBufferObject) SEG_BUFFEROBJECT_C;
INTINTARGFUNC_PROTO(buffer_slice, PyBufferObject) SEG_BUFFEROBJECT_C;
INTOBJARGPROC_PROTO(buffer_ass_item, PyBufferObject) SEG_BUFFEROBJECT_C;
INTINTOBJARGPROC_PROTO(buffer_ass_slice, PyBufferObject) SEG_BUFFEROBJECT_C;
GETREADBUFFERPROC_PROTO(buffer_getreadbuf, PyBufferObject) SEG_BUFFEROBJECT_C;
GETWRITEBUFFERPROC_PROTO(buffer_getwritebuf, PyBufferObject) SEG_BUFFEROBJECT_C;
GETSEGCOUNTPROC_PROTO(buffer_getsegcount, PyBufferObject) SEG_BUFFEROBJECT_C;
static int buffer_getcharbuf Py_PROTO((PyBufferObject *self, int idx,const char ** pp)) SEG_BUFFEROBJECT_C;


#endif /* BUFFEROBJECT_C_H */
