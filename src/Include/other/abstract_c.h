#ifndef ABSTRACT_C_H
#define ABSTRACT_C_H

#include "proto_defs.h"


static PyObject *type_error Py_PROTO((char *msg)) SEG_ABSTRACT_C;
static PyObject *null_error() SEG_ABSTRACT_C;
BINARYFUNC_PROTO(do_pow, PyObject) SEG_ABSTRACT_C;
UNARYFUNC_PROTO(long_from_string, PyObject) SEG_ABSTRACT_C;


#endif /* ABSTRACT_C_H */
