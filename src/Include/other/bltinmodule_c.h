#ifndef BLTINMODULE_C_H
#define BLTINMODULE_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from bltinmodule.c */

PYCFUNCTION_PROTO(builtin___import__, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_abs, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_apply, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_buffer, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_callable, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_chr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_cmp, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_coerce, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_compile, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_complex, PyObject) SEG_BLTINMODULE_C;
#ifndef WITHOUT_COMPLEX
static PyObject *complex_from_string Py_PROTO((PyObject *)) SEG_BLTINMODULE_C;
#endif
PYCFUNCTION_PROTO(builtin_delattr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_dir, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_divmod, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_eval, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_execfile, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_filter, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_float, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_getattr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_globals, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_hasattr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_hash, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_hex, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_id, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_input, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_intern, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_int, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_isinstance, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_issubclass, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_len, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_list, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_locals, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_long, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_map, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_max, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_min, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_oct, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_open, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_ord, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_pow, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_range, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_raw_input, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_reduce, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_reload, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_repr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_round, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_setattr, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_slice, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_str, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_tuple, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_type, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_vars, PyObject) SEG_BLTINMODULE_C;
PYCFUNCTION_PROTO(builtin_xrange, PyObject) SEG_BLTINMODULE_C;
static PyObject *min_max Py_PROTO((PyObject *,	int)) SEG_BLTINMODULE_C;
static long get_len_of_range Py_PROTO((long, long,long)) SEG_BLTINMODULE_C;
static int init_class_exc Py_PROTO((PyObject *)) SEG_BLTINMODULE_C;
static void fini_instances() SEG_BLTINMODULE_C;
static PyObject *newstdexception Py_PROTO((PyObject *, char *)) SEG_BLTINMODULE_C;
static void initerrors Py_PROTO((PyObject *)) SEG_BLTINMODULE_C;
static void finierrors() SEG_BLTINMODULE_C;
static PyObject *filtertuple Py_PROTO((PyObject *,PyObject *)) SEG_BLTINMODULE_C;
static PyObject *filterstring Py_PROTO((PyObject *,PyObject *)) SEG_BLTINMODULE_C;

#endif /* BLTINMODULE_C_H */
