#ifndef METHODOBJECT_C_H
#define METHODOBJECT_C_H

#include "proto_defs.h"


DESTRUCTOR_PROTO(meth_dealloc, PyCFunctionObject) SEG_METHODOBJECT_C;
GETATTRFUNC_PROTO(meth_getattr, PyCFunctionObject) SEG_METHODOBJECT_C;
CMPFUNC_PROTO(meth_compare, PyCFunctionObject) SEG_METHODOBJECT_C;
REPRFUNC_PROTO(meth_repr, PyCFunctionObject) SEG_METHODOBJECT_C;
HASHFUNC_PROTO(meth_hash, PyCFunctionObject) SEG_METHODOBJECT_C;

static PyObject *listmethodchain Py_PROTO((PyMethodChain *)) SEG_METHODOBJECT_C;

#endif /* METHODOBJECT_C_H */
