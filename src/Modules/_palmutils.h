#ifndef _PALMUTILS_H
#define _PALMUTILS_H

PyObject * palmutil_buildErrObject(UInt16 ifErrs);
PyObject * palmutil_setErr(PyObject *exc, Err err);
PyObject * palmutil_setErr_FromDMError(PyObject *exc);

#endif /* _PALMUTILS_H */
