#ifndef IMPORT_C_H
#define IMPORT_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from import.c */
static PyObject *load_module Py_PROTO((char *, FILE *, char *, int)) SEG_IMPORT_C;
static struct filedescr *find_module Py_PROTO((char *, PyObject *,
					       char *, int, FILE **)) SEG_IMPORT_C;
static struct _frozen *find_frozen Py_PROTO((char *name)) SEG_IMPORT_C;
static int init_builtin Py_PROTO((char *)) SEG_IMPORT_C; /* Forward */

static PyObject *get_parent Py_PROTO((PyObject *globals,
				      char *buf, int *p_buflen)) SEG_IMPORT_C;
static PyObject *load_next Py_PROTO((PyObject *mod, PyObject *altmod,
				     char **p_name, char *buf, int *p_buflen)) SEG_IMPORT_C;
static int mark_miss Py_PROTO((char *name)) SEG_IMPORT_C;
static int ensure_fromlist Py_PROTO((PyObject *mod, PyObject *fromlist,
				     char *buf, int buflen, int recursive)) SEG_IMPORT_C;
static PyObject * import_submodule Py_PROTO((PyObject *mod,
					     char *name, char *fullname)) SEG_IMPORT_C;
/* end forward decls */

static void lock_import() SEG_IMPORT_C;
static void unlock_import() SEG_IMPORT_C;
static char *make_compiled_pathname Py_PROTO((char *pathname,char *buf,	int buflen)) SEG_IMPORT_C;
static FILE *check_compiled_module Py_PROTO((char *pathname,long mtime,	char *cpathname)) SEG_IMPORT_C;
static PyCodeObject *read_compiled_module Py_PROTO((char *cpathname,FILE *fp)) SEG_IMPORT_C;
static PyObject *load_compiled_module Py_PROTO((char *name,char *cpathname,FILE *fp)) SEG_IMPORT_C;
static PyCodeObject *parse_source_module Py_PROTO((char *pathname,FILE *fp)) SEG_IMPORT_C;
static void write_compiled_module Py_PROTO((PyCodeObject *co,char *cpathname,long mtime)) SEG_IMPORT_C;
static PyObject *load_source_module Py_PROTO((char *name,char *pathname,FILE *fp)) SEG_IMPORT_C;
static PyObject *load_package Py_PROTO((char *name, char *pathname)) SEG_IMPORT_C;
static int is_builtin Py_PROTO((char *name)) SEG_IMPORT_C;
#ifdef CHECK_IMPORT_CASE
static int check_case(char *, int, int, char *) SEG_IMPORT_C;
#endif

#ifdef HAVE_STAT
static int find_init_module Py_PROTO((char *)) SEG_IMPORT_C; /* Forward */
#endif /* HAVE_STAT */

static struct filedescr *find_module Py_PROTO((char *realname,PyObject *path,char *buf,int buflen,FILE **p_fp)) SEG_IMPORT_C;

static PyObject *load_module Py_PROTO((char *name,FILE *fp,char *buf,int type)) SEG_IMPORT_C;
static int init_builtin Py_PROTO((char *name)) SEG_IMPORT_C;
static struct _frozen *find_frozen Py_PROTO((char *name)) SEG_IMPORT_C;
static PyObject *get_frozen_object Py_PROTO((char *name)) SEG_IMPORT_C;
static PyObject *import_module_ex Py_PROTO((char *name,PyObject *globals,PyObject *locals,PyObject *fromlist)) SEG_IMPORT_C;

PYCFUNCTION_PROTO(imp_get_magic) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_get_suffixes) SEG_IMPORT_C;
PYCFUNCTION_PROTO(call_find_module) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_find_module) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_init_builtin) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_init_frozen) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_get_frozen_object) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_is_builtin) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_is_frozen) SEG_IMPORT_C;
static FILE * get_file Py_PROTO((char *pathname, PyObject *fob, char *mode)) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_find_module) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_get_magic) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_get_suffixes) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_module) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_new_module) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_get_frozen_object) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_init_builtin) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_init_frozen) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_is_builtin) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_is_frozen) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_compiled) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_dynamic) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_package) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_resource) SEG_IMPORT_C;
PYCFUNCTION_PROTO(imp_load_source) SEG_IMPORT_C;
static int setint Py_PROTO((PyObject *d, char *name,int value)) SEG_IMPORT_C;

#endif /* IMPORT_C_H */
