
#ifndef _DBMEM_H_
#define _DBMEM_H_ 1

#include "other/segment_defs.h"
#include "compile.h"

extern int dbmem_init();
extern void * dbmem_add(void *p) SEG_DBMEM_H;
extern void * dbmem_addentry(void *p, long size) SEG_DBMEM_H;
extern int dbmem_fini();
int  dbmem_size(void *s) SEG_DBMEM_H;

PyObject * dbmem_addPyObject(PyObject *s) SEG_DBMEM_H;
PyObject * dbmem_addPyCodeObject(PyCodeObject *v) SEG_DBMEM_H;
PyObject * dbmem_addPyTupleObject(PyTupleObject *v) SEG_DBMEM_H;

#endif /* _DBMEM_H_ */
