#ifndef STRINGOBJECT_C_H
#define STRINGOBJECT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from stringobject.c */


static int string_contains Py_PROTO((PyObject *, PyObject *)) SEG_STRINGOBJECT_C;
static int string_buffer_getcharbuf Py_PROTO((PyStringObject *,int,const char **)) SEG_STRINGOBJECT_C;
static PyObject *split_whitespace Py_PROTO((char *,int,int)) SEG_STRINGOBJECT_C;
static PyObject *string_split Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_join Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static long string_find_internal Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_find Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_index Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static long string_rfind_internal Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_rfind Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_rindex Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *do_strip Py_PROTO((PyStringObject *, PyObject *, int)) SEG_STRINGOBJECT_C;
static PyObject *string_strip Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_lstrip Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_rstrip Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_lower Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_upper Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_capitalize Py_PROTO((PyStringObject *, PyObject *args)) SEG_STRINGOBJECT_C;
static PyObject *string_count Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_swapcase Py_PROTO((PyStringObject *,PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_translate Py_PROTO((PyStringObject *,PyObject *)) SEG_STRINGOBJECT_C;
static int mymemfind Py_PROTO((char *,int,char *,int)) SEG_STRINGOBJECT_C;
static int mymemcnt Py_PROTO((char *,int,char *,int)) SEG_STRINGOBJECT_C;
static char *mymemreplace Py_PROTO((char *,int,char *,int,char *,int,int,int *)) SEG_STRINGOBJECT_C;
static PyObject *string_replace Py_PROTO((PyStringObject *,PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_startswith Py_PROTO((PyStringObject *,PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *string_endswith Py_PROTO((PyStringObject *, PyObject *)) SEG_STRINGOBJECT_C;
static PyObject *getnextarg Py_PROTO((PyObject *,int,int *)) SEG_STRINGOBJECT_C;
#ifndef WITHOUT_FLOAT
static int formatfloat Py_PROTO((char *,int,int,int,PyObject *)) SEG_STRINGOBJECT_C;
#endif /* WITHOUT_FLOAT */
static int formatint Py_PROTO((char *,int,int,int,PyObject *)) SEG_STRINGOBJECT_C;
static int formatchar Py_PROTO((char *, PyObject *)) SEG_STRINGOBJECT_C;


static PyObject *string_concat Py_PROTO((register PyStringObject *a, register PyObject *bb)) SEG_STRINGOBJECT_C;
INQUIRY_PROTO(string_length, PyStringObject) SEG_STRINGOBJECT_C;
INTARGFUNC_PROTO(string_repeat, PyStringObject) SEG_STRINGOBJECT_C;
INTARGFUNC_PROTO(string_item, PyStringObject) SEG_STRINGOBJECT_C;
INTINTARGFUNC_PROTO(string_slice, PyStringObject) SEG_STRINGOBJECT_C;
static int string_buffer_getreadbuf Py_PROTO((PyStringObject *self, int index, const void **ptr)) SEG_STRINGOBJECT_C;
static int string_buffer_getwritebuf Py_PROTO((PyStringObject *self, int index, const void **ptr)) SEG_STRINGOBJECT_C;

GETSEGCOUNTPROC_PROTO(string_buffer_getsegcount, PyStringObject) SEG_STRINGOBJECT_C;
DESTRUCTOR_PROTO(string_dealloc, PyObject) SEG_STRINGOBJECT_C;
PRINTFUNC_PROTO(string_print, PyStringObject) SEG_STRINGOBJECT_C;
CMPFUNC_PROTO(string_compare, PyStringObject) SEG_STRINGOBJECT_C;
REPRFUNC_PROTO(string_repr, PyStringObject) SEG_STRINGOBJECT_C;
HASHFUNC_PROTO(string_hash, PyStringObject) SEG_STRINGOBJECT_C;

#endif /* STRINGOBJECT_C_H */
