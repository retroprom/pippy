#ifndef CLASSOBJECT_C_H
#define CLASSOBJECT_C_H

#include "proto_defs.h"

DESTRUCTOR_PROTO(class_dealloc, PyClassObject) SEG_CLASSOBJECT_C;
GETATTROFUNC_PROTO(class_getattr, PyClassObject) SEG_CLASSOBJECT_C;
SETATTROFUNC_PROTO(class_setattr, PyClassObject) SEG_CLASSOBJECT_C;
REPRFUNC_PROTO(class_repr, PyClassObject) SEG_CLASSOBJECT_C;
REPRFUNC_PROTO(class_str, PyClassObject) SEG_CLASSOBJECT_C;

static void set_slot Py_PROTO((PyObject **, PyObject *)) SEG_CLASSOBJECT_C;
static void set_attr_slots Py_PROTO((PyClassObject *)) SEG_CLASSOBJECT_C;
static char *set_dict Py_PROTO((PyClassObject *, PyObject *)) SEG_CLASSOBJECT_C;
static char *set_bases Py_PROTO((PyClassObject *, PyObject *)) SEG_CLASSOBJECT_C;
static char *set_name Py_PROTO((PyClassObject *, PyObject *)) SEG_CLASSOBJECT_C;

UNARYFUNC_PROTO(instance_neg, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_pos, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_abs, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_invert, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_int, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_long, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_float, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_oct, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_hex, PyInstanceObject) SEG_CLASSOBJECT_C;
BINARYFUNC1_PROTO(instance_subscript, PyInstanceObject) SEG_CLASSOBJECT_C;
TERNARYFUNC_PROTO(instance_pow, PyObject) SEG_CLASSOBJECT_C;
INQUIRY_PROTO(instance_length, PyInstanceObject) SEG_CLASSOBJECT_C;
INQUIRY_PROTO(instance_nonzero, PyInstanceObject) SEG_CLASSOBJECT_C;
COERCION_PROTO(instance_coerce) SEG_CLASSOBJECT_C;
INTARGFUNC_PROTO(instance_item, PyInstanceObject) SEG_CLASSOBJECT_C;
INTINTARGFUNC_PROTO(instance_slice, PyInstanceObject) SEG_CLASSOBJECT_C;
INTOBJARGPROC_PROTO(instance_ass_item, PyInstanceObject) SEG_CLASSOBJECT_C;
INTINTOBJARGPROC_PROTO(instance_ass_slice, PyInstanceObject) SEG_CLASSOBJECT_C;
OBJOBJARGPROC_PROTO(instance_ass_subscript, PyInstanceObject) SEG_CLASSOBJECT_C;
DESTRUCTOR_PROTO(instance_dealloc, PyInstanceObject) SEG_CLASSOBJECT_C;
GETATTROFUNC_PROTO(instance_getattr, PyInstanceObject) SEG_CLASSOBJECT_C;
SETATTROFUNC_PROTO(instance_setattr, PyInstanceObject) SEG_CLASSOBJECT_C;
SETATTROFUNC_PROTO(instance_setattr1, PyInstanceObject) SEG_CLASSOBJECT_C;
CMPFUNC_PROTO(instance_compare, PyObject) SEG_CLASSOBJECT_C;
BINARYFUNC_PROTO(instance_compare1, PyObject) SEG_CLASSOBJECT_C;
REPRFUNC_PROTO(instance_repr, PyInstanceObject) SEG_CLASSOBJECT_C;
HASHFUNC_PROTO(instance_hash, PyInstanceObject) SEG_CLASSOBJECT_C;
static int instance_contains Py_PROTO((PyInstanceObject *inst, PyObject *member)) SEG_CLASSOBJECT_C;
static PyObject *generic_unary_op Py_PROTO((PyInstanceObject *, PyObject *)) SEG_CLASSOBJECT_C;

UNARYFUNC_PROTO(instance_neg, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_pos, PyInstanceObject) SEG_CLASSOBJECT_C;
UNARYFUNC_PROTO(instance_abs, PyInstanceObject) SEG_CLASSOBJECT_C;

DESTRUCTOR_PROTO(instancemethod_dealloc, PyMethodObject) SEG_CLASSOBJECT_C;
GETATTROFUNC_PROTO(instancemethod_getattr, PyMethodObject) SEG_CLASSOBJECT_C;
CMPFUNC_PROTO(instancemethod_compare, PyMethodObject) SEG_CLASSOBJECT_C;
REPRFUNC_PROTO(instancemethod_repr, PyMethodObject) SEG_CLASSOBJECT_C;
HASHFUNC_PROTO(instancemethod_hash, PyMethodObject) SEG_CLASSOBJECT_C;


#endif /* CLASSOBJECT_C_H */
