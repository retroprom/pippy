#ifndef Py_PYTHONRUN_H
#define Py_PYTHONRUN_H
#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
Copyright 1991-1995 by Stichting Mathematisch Centrum, Amsterdam,
The Netherlands.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the names of Stichting Mathematisch
Centrum or CWI or Corporation for National Research Initiatives or
CNRI not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

While CWI is the initial source for this software, a modified version
is made available by the Corporation for National Research Initiatives
(CNRI) at the Internet address ftp://ftp.python.org.

STICHTING MATHEMATISCH CENTRUM AND CNRI DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL STICHTING MATHEMATISCH
CENTRUM OR CNRI BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

******************************************************************/

/* Interfaces to parse and execute pieces of python code */

DL_IMPORT(void) Py_SetProgramName Py_PROTO((char *)) SEG_PYTHONRUN_H;
DL_IMPORT(char *) Py_GetProgramName Py_PROTO((void)) SEG_PYTHONRUN_H;

DL_IMPORT(void) Py_SetPythonHome Py_PROTO((char *)) SEG_PYTHONRUN_H;
DL_IMPORT(char *) Py_GetPythonHome Py_PROTO((void)) SEG_PYTHONRUN_H;

DL_IMPORT(void) Py_Initialize Py_PROTO((void)) SEG_MAIN;
DL_IMPORT(void) Py_Finalize Py_PROTO((void)) SEG_MAIN;
DL_IMPORT(int) Py_IsInitialized Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(PyThreadState *) Py_NewInterpreter Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(void) Py_EndInterpreter Py_PROTO((PyThreadState *)) SEG_PYTHONRUN_H;

DL_IMPORT(int) PyRun_AnyFile Py_PROTO((FILE *, char *)) SEG_PYTHONRUN_H;

DL_IMPORT(int) PyRun_SimpleString Py_PROTO((char *));
DL_IMPORT(int) PyRun_SimpleFile Py_PROTO((FILE *, char *)) SEG_PYTHONRUN_H;
DL_IMPORT(int) PyRun_InteractiveOne Py_PROTO((FILE *, char *)) SEG_PYTHONRUN_H;
DL_IMPORT(int) PyRun_InteractiveLoop Py_PROTO((FILE *, char *)) SEG_PYTHONRUN_H;

DL_IMPORT(struct _node *) PyParser_SimpleParseString Py_PROTO((char *, int)) SEG_PYTHONRUN_H;
DL_IMPORT(struct _node *) PyParser_SimpleParseFile Py_PROTO((FILE *, char *, int)) SEG_PYTHONRUN_H;

DL_IMPORT(PyObject *) PyRun_String Py_PROTO((char *, int, PyObject *, PyObject *)) SEG_PYTHONRUN_H;
DL_IMPORT(PyObject *) PyRun_File Py_PROTO((FILE *, char *, int, PyObject *, PyObject *)) SEG_PYTHONRUN_H;

DL_IMPORT(PyObject *) Py_CompileString Py_PROTO((char *, char *, int)) SEG_PYTHONRUN_H;

DL_IMPORT(void) PyErr_Print Py_PROTO((void)) SEG_MAIN;
DL_IMPORT(void) PyErr_PrintEx Py_PROTO((int)) SEG_PYTHONRUN_H;

DL_IMPORT(int) Py_AtExit Py_PROTO((void (*func) Py_PROTO((void)))) SEG_PYTHONRUN_H;

DL_IMPORT(void) Py_Exit Py_PROTO((int)) SEG_PYTHONRUN_H;

DL_IMPORT(int) Py_FdIsInteractive Py_PROTO((FILE *, char *)) SEG_PYTHONRUN_H;

/* In getpath.c */
DL_IMPORT(char *) Py_GetProgramFullPath Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(char *) Py_GetPrefix Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(char *) Py_GetExecPrefix Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(char *) Py_GetPath Py_PROTO((void)) SEG_PYTHONRUN_H;

/* In their own files */
DL_IMPORT(const char *) Py_GetVersion Py_PROTO((void)) SEG_MAIN;
DL_IMPORT(const char *) Py_GetPlatform Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(const char *) Py_GetCopyright Py_PROTO((void)) SEG_MAIN;
DL_IMPORT(const char *) Py_GetCompiler Py_PROTO((void)) SEG_PYTHONRUN_H;
DL_IMPORT(const char *) Py_GetBuildInfo Py_PROTO((void)) SEG_PYTHONRUN_H;

/* Internal -- various one-time initializations */

DL_IMPORT(PyObject *) _PyBuiltin_Init_1 Py_PROTO((void)) SEG_BLTINMODULE_C;
DL_IMPORT(void) _PyBuiltin_Init_2 Py_PROTO((PyObject *)) SEG_BLTINMODULE_C;
DL_IMPORT(PyObject *) _PySys_Init Py_PROTO((void)) SEG_SYSMODULE_C;
DL_IMPORT(void) _PyImport_Init Py_PROTO((void)) SEG_IMPORT_C;

/* Various internal finalizers */
DL_IMPORT(void) _PyImport_Fini Py_PROTO((void)) SEG_IMPORT_C;
DL_IMPORT(void) _PyBuiltin_Fini_1 Py_PROTO((void)) SEG_BLTINMODULE_C;
DL_IMPORT(void) _PyBuiltin_Fini_2 Py_PROTO((void)) SEG_BLTINMODULE_C;
DL_IMPORT(void) PyMethod_Fini Py_PROTO((void)) SEG_CLASSOBJECT_C;
DL_IMPORT(void) PyFrame_Fini Py_PROTO((void)) SEG_FRAMEOBJECT_C;
DL_IMPORT(void) PyCFunction_Fini Py_PROTO((void)) SEG_METHODOBJECT_C;
DL_IMPORT(void) PyTuple_Fini Py_PROTO((void)) SEG_TUPLEOBJECT_C;
DL_IMPORT(void) PyString_Fini Py_PROTO((void)) SEG_STRINGOBJECT_H;
DL_IMPORT(void) PyInt_Fini Py_PROTO((void)) SEG_INTOBJECT_C;
DL_IMPORT(void) PyFloat_Fini Py_PROTO((void)) SEG_PYTHONRUN_H;

/* Stuff with no proper home (yet) */
DL_IMPORT(char *) PyOS_Readline Py_PROTO((char *)) SEG_MYREADLINE_C;
/* jdc - the following are function pointers and should _not_ be
   associated with a segment. */
extern DL_IMPORT(int) (*PyOS_InputHook) Py_PROTO((void));
extern DL_IMPORT(char) *(*PyOS_ReadlineFunctionPointer) Py_PROTO((char *));

#ifdef __cplusplus
}
#endif
#endif /* !Py_PYTHONRUN_H */
