
#ifndef _DBMEM_H_
#define _DBMEM_H_ 1

#include "dbmempub.h"
#include "compile.h"

PyObject * dbmem_addPyObject(PyObject *s) SEG_DBMEM_H;
PyObject * dbmem_addPyCodeObject(PyCodeObject *v) SEG_DBMEM_H;
PyObject * dbmem_addPyTupleObject(PyTupleObject *v) SEG_DBMEM_H;

#endif /* _DBMEM_H_ */
