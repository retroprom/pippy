#ifndef GETARGS_C_H
#define GETARGS_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from getargs.c */
static int vgetargs1 Py_PROTO((PyObject *, char *, va_list *, int)) SEG_GETARGS_C;
static void seterror Py_PROTO((int, char *, int *, char *, char *)) SEG_GETARGS_C;
static char *convertitem Py_PROTO((PyObject *, char **, va_list *,
				   int *, char *)) SEG_GETARGS_C;
static char *converttuple Py_PROTO((PyObject *, char **, va_list *,
				 int *, char *, int)) SEG_GETARGS_C;
static char *convertsimple Py_PROTO((PyObject *, char **, va_list *, char *)) SEG_GETARGS_C;
static char *convertsimple1 Py_PROTO((PyObject *, char **, va_list *)) SEG_GETARGS_C;

static int vgetargskeywords Py_PROTO((PyObject *, PyObject *,
				   char *, char **, va_list *)) SEG_GETARGS_C;
static char *skipitem Py_PROTO((char **, va_list *)) SEG_GETARGS_C;
/* end forward decls */
#endif /* GETARGS_C_H */
