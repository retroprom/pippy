#ifndef Py_BUFFEROBJECT_H
#define Py_BUFFEROBJECT_H
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

/* Buffer object interface */

/* Note: the object's structure was private, but not anymore */

typedef struct {
	PyObject_HEAD
	PyObject *b_base;
	void *b_ptr;
	int b_size;
	unsigned int b_flags;
#ifdef CACHE_HASH
	long b_hash;
#endif
} PyBufferObject;


extern DL_IMPORT(PyTypeObject) PyBuffer_Type;

#define PyBuffer_Check(op) ((op)->ob_type == &PyBuffer_Type)

#define Py_END_OF_BUFFER	(-1)

extern DL_IMPORT(PyObject *) PyBuffer_FromObject Py_PROTO((PyObject *base, int offset, int size)) SEG_BUFFEROBJECT_H;
extern DL_IMPORT(PyObject *) PyBuffer_FromReadWriteObject Py_PROTO((PyObject *base, int offset, int size)) SEG_BUFFEROBJECT_H;

extern DL_IMPORT(PyObject *) PyBuffer_FromMemory Py_PROTO((void *ptr, int size, unsigned int flags)) SEG_BUFFEROBJECT_H;
extern DL_IMPORT(PyObject *) PyBuffer_FromReadWriteMemory Py_PROTO((void *ptr, int size, unsigned int flags)) SEG_BUFFEROBJECT_H;


extern DL_IMPORT(PyObject *) PyBuffer_New Py_PROTO((int size)) SEG_BUFFEROBJECT_H;

#define PyBuffer_READONLY 0x1
#define PyBuffer_BASE 0x2

#define PyBuffer_IsReadOnly(op) ((op)->b_flags & PyBuffer_READONLY)
#define PyBuffer_SetReadOnly(op) ((op)->b_flags |= PyBuffer_READONLY)
#define PyBuffer_SetBase(op) ((op)->b_flags |= PyBuffer_BASE)
#define PyBuffer_IsBase(op) ((op)->b_flags & PyBuffer_BASE)

#define PyBuffer_IsValid(op) (((op)->b_ptr != NULL) \
		&& (((op)->b_base != NULL && PyBuffer_Check((op)->b_base)) ?\
                   (((PyBufferObject *)((op)->b_base))->b_ptr != NULL) : 1))
#define PyBuffer_Invalidate(op) (op)->b_ptr = NULL;\
                                ((op)->b_base != NULL && PyBuffer_Check((op)->b_base) ?\
                                 ((PyBufferObject *)(op)->b_base)->b_ptr = NULL: NULL )

#ifdef __cplusplus
}
#endif
#endif /* !Py_BUFFEROBJECT_H */

