#ifndef COMPILE_C_H
#define COMPILE_C_H

#include "proto_defs.h"
/* previously forward declarations stripped from compile.c */
/* end forward decls */

#define COMPILINGFUNC_PROTO(name) static void name Py_PROTO((struct compiling *c, int type))
#define COMPILINGFUNCMSG_PROTO(name) static void name Py_PROTO((struct compiling *c, int type, char*msg))
#define COMPNODEFUNC_PROTO(name) static void name Py_PROTO((struct compiling *c, node *n))
#define COMPNODEASSFUNC_PROTO(name) static void name Py_PROTO((struct compiling *c, node *n, int assigning))

static void com_error Py_PROTO((struct compiling *c, PyObject *exc,char *msg)) SEG_COMPILE_C;
static void block_push Py_PROTO((struct compiling *c, int type)) SEG_COMPILE_C;
static void block_pop Py_PROTO((struct compiling *c, int type)) SEG_COMPILE_C;
static PyObject *parsenumber Py_PROTO((struct compiling *co, char *s)) SEG_COMPILE_C;

COMPNODEFUNC_PROTO(com_list_constructor) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_dictmaker) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_atom) SEG_COMPILE_C;
static void com_slice Py_PROTO((struct compiling *c, node *n, int op)) SEG_COMPILE_C;
static void com_argument Py_PROTO((struct compiling *c, node *n, PyObject **pkeywords)) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_call_function) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_select_member) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_sliceobj) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_subscript) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_subscriptlist) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_apply_trailer) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_power) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_factor) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_term) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_arith_expr) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_shift_expr) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_and_expr) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_xor_expr) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_expr) SEG_COMPILE_C;
static enum cmp_op cmp_type Py_PROTO((node *n)) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_comparison) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_not_test) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_and_test) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_test) SEG_COMPILE_C;
static void com_list Py_PROTO(( struct compiling *c, node *n, int toplevel)) SEG_COMPILE_C;
static void com_assign_name Py_PROTO((struct compiling *, node *, int)) SEG_COMPILE_C;
static void com_assign Py_PROTO((struct compiling *, node *, int)) SEG_COMPILE_C;

COMPNODEASSFUNC_PROTO(com_assign_attr) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_assign_trailer) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_assign_tuple) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_assign_list) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_assign_name) SEG_COMPILE_C;
COMPNODEASSFUNC_PROTO(com_assign) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_expr_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_assert_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_print_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_return_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_raise_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_import_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_global_stmt) SEG_COMPILE_C;
static int com_newlocal_o Py_PROTO((struct compiling *c, PyObject *nameval)) SEG_COMPILE_C;
static int com_addlocal_o Py_PROTO((struct compiling *c, PyObject *nameval)) SEG_COMPILE_C;
static int com_newlocal_o Py_PROTO((struct compiling *c, PyObject *nameval)) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_exec_stmt) SEG_COMPILE_C;

static int is_constant_false Py_PROTO((struct compiling *c, node *n));
COMPNODEFUNC_PROTO(com_if_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_while_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_for_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_try_except) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_try_finally) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_try_stmt) SEG_COMPILE_C;
static node * get_rawdocstring Py_PROTO((node *n)) SEG_COMPILE_C;
static PyObject *get_docstring Py_PROTO((node *n)) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_suite) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_continue_stmt) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_funcdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_bases) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_classdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_node) SEG_COMPILE_C;
static void com_fplist Py_PROTO((struct compiling *, node *)) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_fpdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_fplist) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_arglist) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(com_file_input) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(compile_funcdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(compile_lambdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(compile_classdef) SEG_COMPILE_C;
COMPNODEFUNC_PROTO(compile_node) SEG_COMPILE_C;
static void optimize Py_PROTO((	struct compiling *c)) SEG_COMPILE_C;
static PyCodeObject *icompile Py_PROTO((node *n, struct compiling *base)) SEG_COMPILE_C;
static PyCodeObject *jcompile Py_PROTO((node *n, char *filename,struct compiling *base)) SEG_COMPILE_C;

#endif /* COMPILE_C_H */
