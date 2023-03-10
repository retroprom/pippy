#ifndef Py_SLICEOBJECT_H
#define Py_SLICEOBJECT_H
#ifdef __cplusplus
extern "C" {
#endif

/* The unique ellipsis object "..." */

extern DL_IMPORT(PyObject) _Py_EllipsisObject; /* Don't use this directly */

#define Py_Ellipsis (&_Py_EllipsisObject)

/* Slice object interface */

/*

A slice object containing start, stop, and step data members (the
names are from range).  After much talk with Guido, it was decided to
let these be any arbitrary python type. 
*/

typedef struct {
	PyObject_HEAD
	PyObject *start, *stop, *step;
} PySliceObject;

extern DL_IMPORT(PyTypeObject) PySlice_Type;

#define PySlice_Check(op) ((op)->ob_type == &PySlice_Type)

DL_IMPORT(PyObject *) PySlice_New Py_PROTO((
	PyObject* start, PyObject* stop, PyObject* step)) SEG_SLICEOBJECT_H;
DL_IMPORT(int) PySlice_GetIndices Py_PROTO((
	PySliceObject *r, int length, int *start, int *stop, int *step)) SEG_SLICEOBJECT_H;

#ifdef __cplusplus
}
#endif
#endif /* !Py_SLICEOBJECT_H */
