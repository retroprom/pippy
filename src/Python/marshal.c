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

/* Write Python objects to files and read them back.
   This is intended for writing and reading compiled Python code only;
   a true persistent storage facility would be much harder, since
   it would have to take circular links and sharing into account. */

#include "Python.h"
#include "longintrepr.h"
#include "compile.h"
#include "marshal.h"
#include "dbmem.h"

#define TYPE_NULL	'0'
#define TYPE_NONE	'N'
#define TYPE_ELLIPSIS   '.'
#define TYPE_INT	'i'
#define TYPE_INT64	'I'
#define TYPE_FLOAT	'f'
#define TYPE_COMPLEX	'x'
#define TYPE_LONG	'l'
#define TYPE_STRING	's'
#define TYPE_TUPLE	'('
#define TYPE_LIST	'['
#define TYPE_DICT	'{'
#define TYPE_CODE	'c'
#define TYPE_UNKNOWN	'?'

typedef struct {
	FILE *fp;
	int error;
	/* If fp == NULL, the following are valid: */
	PyObject *str;
	char *ptr;
	char *end;
} WFILE;

typedef WFILE RFILE; /* Same struct with different invariants */

#define w_byte(c, p) if (((p)->fp)) putc((c), (p)->fp); \
		      else if ((p)->ptr != (p)->end) *(p)->ptr++ = (c); \
			   else w_more(c, p)
#include "other/marshal_c.h"

static void
w_more(
       char c,
       WFILE *p)
{
	int size, newsize;
	if (p->str == NULL)
		return; /* An error already occurred */
	size = PyString_Size(p->str);
	newsize = size + 1024;
	if (_PyString_Resize(&p->str, newsize) != 0) {
		p->ptr = p->end = NULL;
	}
	else {
		p->ptr = PyString_AS_STRING((PyStringObject *)p->str) + size;
		p->end =
			PyString_AS_STRING((PyStringObject *)p->str) + newsize;
		*p->ptr++ = c;
	}
}

static void
w_string(s, n, p)
	char *s;
	int n;
	WFILE *p;
{
	if (p->fp != NULL) {
		fwrite(s, 1, n, p->fp);
	}
	else {
		while (--n >= 0) {
			w_byte(*s, p);
			s++;
		}
	}
}

static void
w_short(x, p)
	int x;
	WFILE *p;
{
	w_byte( x      & 0xff, p);
	w_byte((x>> 8) & 0xff, p);
}

static void
w_long(x, p)
	long x;
	WFILE *p;
{
	w_byte((int)( x      & 0xff), p);
	w_byte((int)((x>> 8) & 0xff), p);
	w_byte((int)((x>>16) & 0xff), p);
	w_byte((int)((x>>24) & 0xff), p);
}

#if SIZEOF_LONG > 4
static void
w_long64(x, p)
	long x;
	WFILE *p;
{
	w_long(x, p);
	w_long(x>>32, p);
}
#endif

static void
w_object(v, p)
	PyObject *v;
	WFILE *p;
{
	int i, n;
	PyBufferProcs *pb;
	
	if (v == NULL) {
		w_byte(TYPE_NULL, p);
	}
	else if (v == Py_None) {
		w_byte(TYPE_NONE, p);
	}
	else if (v == Py_Ellipsis) {
	        w_byte(TYPE_ELLIPSIS, p);
	}
	else if (PyInt_Check(v)) {
		long x = PyInt_AS_LONG((PyIntObject *)v);
#if SIZEOF_LONG > 4
		long y = x>>31;
		if (y && y != -1) {
			w_byte(TYPE_INT64, p);
			w_long64(x, p);
		}
		else
#endif
			{
			w_byte(TYPE_INT, p);
			w_long(x, p);
		}
	}
	else if (PyLong_Check(v)) {
		PyLongObject *ob = (PyLongObject *)v;
		w_byte(TYPE_LONG, p);
		n = ob->ob_size;
		w_long((long)n, p);
		if (n < 0)
			n = -n;
		for (i = 0; i < n; i++)
			w_short(ob->ob_digit[i], p);
	}
#ifndef WITHOUT_FLOAT
	else if (PyFloat_Check(v)) {
		extern void PyFloat_AsString
			Py_PROTO((char *, PyFloatObject *));
		char buf[256]; /* Plenty to format any double */
		PyFloat_AsString(buf, (PyFloatObject *)v);
		n = strlen(buf);
		w_byte(TYPE_FLOAT, p);
		w_byte(n, p);
		w_string(buf, n, p);
	}
#endif /* WITHOUT_FLOAT */
#ifndef WITHOUT_COMPLEX
	else if (PyComplex_Check(v)) {
		extern void PyFloat_AsString
			Py_PROTO((char *, PyFloatObject *));
		char buf[256]; /* Plenty to format any double */
		PyFloatObject *temp;
		w_byte(TYPE_COMPLEX, p);
		temp = (PyFloatObject*)PyFloat_FromDouble(
			PyComplex_RealAsDouble(v));
		PyFloat_AsString(buf, temp);
		Py_DECREF(temp);
		n = strlen(buf);
		w_byte(n, p);
		w_string(buf, n, p);
		temp = (PyFloatObject*)PyFloat_FromDouble(
			PyComplex_ImagAsDouble(v));
		PyFloat_AsString(buf, temp);
		Py_DECREF(temp);
		n = strlen(buf);
		w_byte(n, p);
		w_string(buf, n, p);
	}
#endif
	else if (PyString_Check(v)) {
		w_byte(TYPE_STRING, p);
		n = PyString_Size(v);
		w_long((long)n, p);
		w_string(PyString_AsString(v), n, p);
	}
	else if (PyTuple_Check(v)) {
		w_byte(TYPE_TUPLE, p);
		n = PyTuple_Size(v);
		w_long((long)n, p);
		for (i = 0; i < n; i++) {
			w_object(PyTuple_GET_ITEM(v, i), p);
		}
	}
	else if (PyList_Check(v)) {
		w_byte(TYPE_LIST, p);
		n = PyList_Size(v);
		w_long((long)n, p);
		for (i = 0; i < n; i++) {
			w_object(PyList_GetItem(v, i), p);
		}
	}
	else if (PyDict_Check(v)) {
		int pos;
		PyObject *key, *value;
		w_byte(TYPE_DICT, p);
		/* This one is NULL object terminated! */
		pos = 0;
		while (PyDict_Next(v, &pos, &key, &value)) {
			w_object(key, p);
			w_object(value, p);
		}
		w_object((PyObject *)NULL, p);
	}
	else if (PyCode_Check(v)) {
		PyCodeObject *co = (PyCodeObject *)v;
		w_byte(TYPE_CODE, p);
		w_short(co->co_argcount, p);
		w_short(co->co_nlocals, p);
		w_short(co->co_stacksize, p);
		w_short(co->co_flags, p);
		w_object(co->co_code, p);
		w_object(co->co_consts, p);
		w_object(co->co_names, p);
		w_object(co->co_varnames, p);
		w_object(co->co_filename, p);
		w_object(co->co_name, p);
		w_short(co->co_firstlineno, p);
		w_object(co->co_lnotab, p);
	}
	else if ((pb = v->ob_type->tp_as_buffer) != NULL &&
		 pb->bf_getsegcount != NULL &&
		 pb->bf_getreadbuffer != NULL &&
		 (*pb->bf_getsegcount)(v, NULL) == 1)
	{
		/* Write unknown buffer-style objects as a string */
		char *s;
		w_byte(TYPE_STRING, p);
		n = (*pb->bf_getreadbuffer)(v, 0, (void **)&s);
		w_long((long)n, p);
		w_string(s, n, p);
	}
	else {
		w_byte(TYPE_UNKNOWN, p);
		p->error = 1;
	}
}

void
PyMarshal_WriteLongToFile(x, fp)
	long x;
	FILE *fp;
{
	WFILE wf;
	wf.fp = fp;
	wf.error = 0;
	w_long(x, &wf);
}

void
PyMarshal_WriteObjectToFile(x, fp)
	PyObject *x;
	FILE *fp;
{
	WFILE wf;
	wf.fp = fp;
	wf.error = 0;
	w_object(x, &wf);
}


#define rs_byte(p) (((p)->ptr != (p)->end) ? (unsigned char)*(p)->ptr++ : EOF)

#define r_byte(p) ((p)->fp ? getc((p)->fp) : rs_byte(p))

static int
r_string(s, n, p)
	char *s;
	int n;
	RFILE *p;
{
	if (p->fp != NULL)
		return fread(s, 1, n, p->fp);
	if (p->end - p->ptr < n)
		n = p->end - p->ptr;
	memcpy(s, p->ptr, n);
	p->ptr += n;
	return n;
}

static int
r_string_in_place(s, n, p)
	char **s;
	int n;
	RFILE *p;
{
/* 	if (p->fp != NULL) */
/* 		return fread(s, 1, n, p->fp); */
	if (p->end - p->ptr < n)
		n = p->end - p->ptr;
	*s = p->ptr;
	p->ptr += n;
	return n;
}

static int
r_short(p)
	RFILE *p;
{
	register short x;
	x = r_byte(p);
	x |= r_byte(p) << 8;
	/* XXX If your short is > 16 bits, add sign-extension here!!! */
	return x;
}

static long
r_long(p)
	RFILE *p;
{
	register long x;
	register FILE *fp = p->fp;
	if (fp) {
		x = getc(fp);
		x |= (long)getc(fp) << 8;
		x |= (long)getc(fp) << 16;
		x |= (long)getc(fp) << 24;
	}
	else {
		x = rs_byte(p);
		x |= (long)rs_byte(p) << 8;
		x |= (long)rs_byte(p) << 16;
		x |= (long)rs_byte(p) << 24;
	}
#if SIZEOF_LONG > 4
	/* Sign extension for 64-bit machines */
	x <<= (8*sizeof(long) - 32);
	x >>= (8*sizeof(long) - 32);
#endif
	return x;
}

static long
r_long64(p)
	RFILE *p;
{
	register long x;
	x = r_long(p);
#if SIZEOF_LONG > 4
	x = (x & 0xFFFFFFFF) | (r_long(p) << 32);
#else
	if (r_long(p) != 0) {
		PyObject *f = PySys_GetObject("stderr");
		if (f != NULL)
			(void) PyFile_WriteString(
			    "Warning: un-marshal 64-bit int in 32-bit mode\n",
			    f);
	}
#endif
	return x;
}

static PyObject *
r_code_as_buffer(RFILE *p)
{
	/* this is a hack to read bytecodes directly from datamanager memory,
	   and should be replaced by a more robust solution.
	 */

	long n;
	char *s;
	PyObject *v;
	int type = r_byte(p);
	
	switch (type) {

	case TYPE_STRING:
		n = r_long(p);
		if (n < 0) {
			PyErr_SetString(PyExc_ValueError, "bad marshal data");
			return NULL;
		}

		if (r_string_in_place(&s, (int)n, p) != n) {
			PyErr_SetString(PyExc_EOFError,
					"EOF read where object expected");
			return NULL;
		}
		
		v = PyBuffer_FromMemory(s, n, (unsigned)0);
		if (v==NULL)
			return NULL;
/* 		printf("buffer size = %d\n", ((PyBufferObject *)v)->b_size); */
		
		return v;
	default:
		PyErr_SetString(PyExc_ValueError, 
				"Hey - we were expecting a string for co_code");
		return NULL;
	}

}


static PyObject *
r_object(p)
	RFILE *p;
{
	PyObject *v, *v2;
	long i, n;
	int type = r_byte(p);
	
	switch (type) {
	
	case EOF:
		PyErr_SetString(PyExc_EOFError,
				"EOF read where object expected");
		return NULL;
	
	case TYPE_NULL:
		return NULL;
	
	case TYPE_NONE:
		Py_INCREF(Py_None);
		return Py_None;
	
	case TYPE_ELLIPSIS:
		Py_INCREF(Py_Ellipsis);
		return Py_Ellipsis;

	case TYPE_INT:
		return PyInt_FromLong(r_long(p));
	
	case TYPE_INT64:
		return PyInt_FromLong(r_long64(p));
	
	case TYPE_LONG:
		{
			int size;
			PyLongObject *ob;
			n = r_long(p);
			size = n<0 ? -n : n;
			ob = _PyLong_New(size);
			if (ob == NULL)
				return NULL;
			ob->ob_size = n;
			for (i = 0; i < size; i++)
				ob->ob_digit[i] = r_short(p);
			return (PyObject *)ob;
		}
	
	case TYPE_FLOAT:
		{
#ifndef WITHOUT_FLOAT
			extern double atof Py_PROTO((const char *));
			char buf[256];
			double dx;
			n = r_byte(p);
			if (r_string(buf, (int)n, p) != n) {
				PyErr_SetString(PyExc_EOFError,
					"EOF read where object expected");
				return NULL;
			}
			buf[n] = '\0';
			PyFPE_START_PROTECT("atof", return 0)
			dx = atof(buf);
			PyFPE_END_PROTECT(dx)
			return PyFloat_FromDouble(dx);
#else /* !WITHOUT_FLOAT */
			PyErr_SetString(PyExc_MissingFeatureError,
			"Float objects are not provided in this python build");
			return NULL;
#endif /* !WITHOUT_FLOAT */
		}
	
	case TYPE_COMPLEX:
		{
#ifndef WITHOUT_COMPLEX
			extern double atof Py_PROTO((const char *));
			char buf[256];
			Py_complex c;
			n = r_byte(p);
			if (r_string(buf, (int)n, p) != n) {
				PyErr_SetString(PyExc_EOFError,
					"EOF read where object expected");
				return NULL;
			}
			buf[n] = '\0';
			PyFPE_START_PROTECT("atof", return 0)
			c.real = atof(buf);
			PyFPE_END_PROTECT(c)
			n = r_byte(p);
			if (r_string(buf, (int)n, p) != n) {
				PyErr_SetString(PyExc_EOFError,
					"EOF read where object expected");
				return NULL;
			}
			buf[n] = '\0';
			PyFPE_START_PROTECT("atof", return 0)
			c.imag = atof(buf);
			PyFPE_END_PROTECT(c)
			return PyComplex_FromCComplex(c);
#else /* !WITHOUT_COMPLEX */
			PyErr_SetString(PyExc_MissingFeatureError,
					"Complex objects are not provided in this python build");
			return NULL;
#endif
		}
	
	case TYPE_STRING:
		n = r_long(p);
		if (n < 0) {
			PyErr_SetString(PyExc_ValueError, "bad marshal data");
			return NULL;
		}
		v = PyString_FromStringAndSize((char *)NULL, n);
		if (v != NULL) {
			if (r_string(PyString_AsString(v), (int)n, p) != n) {
				Py_DECREF(v);
				v = NULL;
				PyErr_SetString(PyExc_EOFError,
					"EOF read where object expected");
			}
		}
		return v;
	
	case TYPE_TUPLE:
		n = r_long(p);
		if (n < 0) {
			PyErr_SetString(PyExc_ValueError, "bad marshal data");
			return NULL;
		}
		v = PyTuple_New((int)n);
		if (v == NULL)
			return v;
		for (i = 0; i < n; i++) {
			v2 = r_object(p);
			if ( v2 == NULL ) {
				Py_DECREF(v);
				v = NULL;
				break;
			}
			PyTuple_SET_ITEM(v, (int)i, v2);
		}
		return v;
	
	case TYPE_LIST:
		n = r_long(p);
		if (n < 0) {
			PyErr_SetString(PyExc_ValueError, "bad marshal data");
			return NULL;
		}
		v = PyList_New((int)n);
		if (v == NULL)
			return v;
		for (i = 0; i < n; i++) {
			v2 = r_object(p);
			if ( v2 == NULL ) {
				Py_DECREF(v);
				v = NULL;
				break;
			}
			PyList_SetItem(v, (int)i, v2);
		}
		return v;
	
	case TYPE_DICT:
		v = PyDict_New();
		if (v == NULL)
			return NULL;
		for (;;) {
			PyObject *key, *val;
			key = r_object(p);
			if (key == NULL)
				break; /* XXX Assume TYPE_NULL, not an error */
			val = r_object(p);
			if (val != NULL)
				PyDict_SetItem(v, key, val);
			Py_DECREF(key);
			Py_XDECREF(val);
		}
		return v;
	
	case TYPE_CODE:
		{
			int argcount = r_short(p);
			int nlocals = r_short(p);
			int stacksize = r_short(p);
			int flags = r_short(p);
			PyObject *code = NULL;
			PyObject *consts = NULL;
			PyObject *names = NULL;
			PyObject *varnames = NULL;
			PyObject *filename = NULL;
			PyObject *name = NULL;
			int firstlineno = 0;
			PyObject *lnotab = NULL;
			
/* 			code = r_object(p); */
			code = r_code_as_buffer(p);
			if (code) consts = r_object(p);
			if (consts) names = r_object(p);
			if (names) varnames = r_object(p);
			if (varnames) filename = r_object(p);
			if (filename) name = r_object(p);
			if (name) {
				firstlineno = r_short(p);
				lnotab = r_object(p);
			}
			DMESSAGE("marshal: before code");
			
			if (!PyErr_Occurred()) {
				v = (PyObject *) PyCode_New(
					argcount, nlocals, stacksize, flags, 
					code, consts, names, varnames,
					filename, name, firstlineno, lnotab);

				DMESSAGE("marshal co created");
#ifdef PALMDM_CODE_OBJECTS				
				/* try to intern the result */
				v = dbmem_addPyCodeObject(v);
#endif /* PALMDM_CODE_OBJECTS */
			}
			else
				v = NULL;
			Py_XDECREF(code);
			Py_XDECREF(consts);
			Py_XDECREF(names);
			Py_XDECREF(varnames);
			Py_XDECREF(filename);
			Py_XDECREF(name);
			Py_XDECREF(lnotab);

		}
		return v;
	
	default:
		/* Bogus data got written, which isn't ideal.
		   This will let you keep working and recover. */
		PyErr_SetString(PyExc_ValueError, "bad marshal data");
		return NULL;
	
	}
}

long
PyMarshal_ReadLongFromFile(fp)
	FILE *fp;
{
	RFILE rf;
	rf.fp = fp;
	return r_long(&rf);
}

PyObject *
PyMarshal_ReadObjectFromFile(fp)
	FILE *fp;
{
	RFILE rf;
	if (PyErr_Occurred()) {
		fprintf(stderr, "XXX rd_object called with exception set\n");
		return NULL;
	}
	rf.fp = fp;
	return r_object(&rf);
}

PyObject *
PyMarshal_ReadObjectFromString(str, len)
	char *str;
	int len;
{
	RFILE rf;
	if (PyErr_Occurred()) {
		fprintf(stderr, "XXX rds_object called with exception set\n");
		return NULL;
	}
	rf.fp = NULL;
	rf.str = NULL;
	rf.ptr = str;
	rf.end = str + len;
	return r_object(&rf);
}

PyObject *
PyMarshal_WriteObjectToString(x) /* wrs_object() */
	PyObject *x;
{
	WFILE wf;
	wf.fp = NULL;
	wf.str = PyString_FromStringAndSize((char *)NULL, 50);
	if (wf.str == NULL)
		return NULL;
	wf.ptr = PyString_AS_STRING((PyStringObject *)wf.str);
	wf.end = wf.ptr + PyString_Size(wf.str);
	wf.error = 0;
	w_object(x, &wf);
	if (wf.str != NULL)
		_PyString_Resize(&wf.str,
		    (int) (wf.ptr -
			   PyString_AS_STRING((PyStringObject *)wf.str)));
	if (wf.error) {
		Py_XDECREF(wf.str);
		PyErr_SetString(PyExc_ValueError, "unmarshallable object");
		return NULL;
	}
	return wf.str;
}

/* And an interface for Python programs... */

static PyObject *
marshal_dump(self, args)
	PyObject *self;
	PyObject *args;
{
	WFILE wf;
	PyObject *x;
	PyObject *f;
	if (!PyArg_Parse(args, "(OO)", &x, &f))
		return NULL;
	if (!PyFile_Check(f)) {
		PyErr_SetString(PyExc_TypeError,
				"marshal.dump() 2nd arg must be file");
		return NULL;
	}
	wf.fp = PyFile_AsFile(f);
	wf.str = NULL;
	wf.ptr = wf.end = NULL;
	wf.error = 0;
	w_object(x, &wf);
	if (wf.error) {
		PyErr_SetString(PyExc_ValueError, "unmarshallable object");
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
marshal_load(self, args)
	PyObject *self;
	PyObject *args;
{
	RFILE rf;
	PyObject *f;
	PyObject *v;
	if (!PyArg_Parse(args, "O", &f))
		return NULL;
	if (!PyFile_Check(f)) {
		PyErr_SetString(PyExc_TypeError,
				"marshal.load() arg must be file");
		return NULL;
	}
	rf.fp = PyFile_AsFile(f);
	rf.str = NULL;
	rf.ptr = rf.end = NULL;
	PyErr_Clear();
	v = r_object(&rf);
	if (PyErr_Occurred()) {
		Py_XDECREF(v);
		v = NULL;
	}
	return v;
}

static PyObject *
marshal_dumps(self, args)
	PyObject *self;
	PyObject *args;
{
	PyObject *x;
	if (!PyArg_Parse(args, "O", &x))
		return NULL;
	return PyMarshal_WriteObjectToString(x);
}

static PyObject *
marshal_loads(self, args)
	PyObject *self;
	PyObject *args;
{
	RFILE rf;
	PyObject *v;
	char *s;
	int n;
	if (!PyArg_Parse(args, "s#", &s, &n))
		return NULL;
	rf.fp = NULL;
	rf.str = args;
	rf.ptr = s;
	rf.end = s + n;
	PyErr_Clear();
	v = r_object(&rf);
	if (PyErr_Occurred()) {
		Py_XDECREF(v);
		v = NULL;
	}
	return v;
}

static PyMethodDef marshal_methods[] = {
	{"dump",	marshal_dump},
	{"load",	marshal_load},
	{"dumps",	marshal_dumps},
	{"loads",	marshal_loads},
	{NULL,		NULL}		/* sentinel */
};

void
PyMarshal_Init()
{
	(void) Py_InitModule("marshal", marshal_methods);
}
