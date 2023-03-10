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

/* Complex object implementation */

/* Borrows heavily from floatobject.c */

/* Submitted by Jim Hugunin */
#include "Python.h"
#ifndef WITHOUT_COMPLEX

#include "mymath.h"
#include "other/complexobject_c.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif


/* elementary operations on complex numbers */

static Py_complex c_1 = {1., 0.};

Py_complex c_sum(a,b)
	Py_complex a,b;
{
	Py_complex r;
	r.real = a.real + b.real;
	r.imag = a.imag + b.imag;
	return r;
}

Py_complex c_diff(a,b)
	Py_complex a,b;
{
	Py_complex r;
	r.real = a.real - b.real;
	r.imag = a.imag - b.imag;
	return r;
}

Py_complex c_neg(a)
	Py_complex a;
{
	Py_complex r;
	r.real = -a.real;
	r.imag = -a.imag;
	return r;
}

Py_complex c_prod(a,b)
	Py_complex a,b;
{
	Py_complex r;
	r.real = a.real*b.real - a.imag*b.imag;
	r.imag = a.real*b.imag + a.imag*b.real;
	return r;
}

Py_complex c_quot(a,b)
	Py_complex a,b;
{
	Py_complex r;
	double d = b.real*b.real + b.imag*b.imag;
	if (d == 0.)
		errno = EDOM;
	r.real = (a.real*b.real + a.imag*b.imag)/d;
	r.imag = (a.imag*b.real - a.real*b.imag)/d;
	return r;
}

Py_complex c_pow(a,b)
	Py_complex a,b;
{
	Py_complex r;
	double vabs,len,at,phase;
	if (b.real == 0. && b.imag == 0.) {
		r.real = 1.;
		r.imag = 0.;
	}
	else if (a.real == 0. && a.imag == 0.) {
		if (b.imag != 0. || b.real < 0.)
			errno = ERANGE;
		r.real = 0.;
		r.imag = 0.;
	}
	else {
		vabs = hypot(a.real,a.imag);
		len = pow(vabs,b.real);
		at = atan2(a.imag, a.real);
		phase = at*b.real;
		if (b.imag != 0.0) {
			len /= exp(at*b.imag);
			phase += b.imag*log(vabs);
		}
		r.real = len*cos(phase);
		r.imag = len*sin(phase);
	}
	return r;
}

static Py_complex c_powu(x, n)
	Py_complex x;
	long n;
{
	Py_complex r, p;
	long mask = 1;
	r = c_1;
	p = x;
	while (mask > 0 && n >= mask) {
		if (n & mask)
			r = c_prod(r,p);
		mask <<= 1;
		p = c_prod(p,p);
	}
	return r;
}

static Py_complex c_powi(x, n)
	Py_complex x;
	long n;
{
	Py_complex cn;

	if (n > 100 || n < -100) {
		cn.real = (double) n;
		cn.imag = 0.;
		return c_pow(x,cn);
	}
	else if (n > 0)
		return c_powu(x,n);
	else
		return c_quot(c_1,c_powu(x,-n));

}

PyObject *
PyComplex_FromCComplex(cval)
	Py_complex cval;
{
	register PyComplexObject *op =
		(PyComplexObject *) malloc(sizeof(PyComplexObject));
	if (op == NULL)
		return PyErr_NoMemory();
	op->ob_type = &PyComplex_Type;
	op->cval = cval;
	_Py_NewReference((PyObject *)op);
	return (PyObject *) op;
}

PyObject *
PyComplex_FromDoubles(real, imag)
	double real, imag;
{
	Py_complex c;
	c.real = real;
	c.imag = imag;
	return PyComplex_FromCComplex(c);
}

double
PyComplex_RealAsDouble(op)
	PyObject *op;
{
	if (PyComplex_Check(op)) {
		return ((PyComplexObject *)op)->cval.real;
	} else {
		return PyFloat_AsDouble(op);
	}
}

double
PyComplex_ImagAsDouble(op)
	PyObject *op;
{
	if (PyComplex_Check(op)) {
		return ((PyComplexObject *)op)->cval.imag;
	} else {
		return 0.0;
	}
}

Py_complex
PyComplex_AsCComplex(op)
	PyObject *op;
{
	Py_complex cv;
	if (PyComplex_Check(op)) {
		return ((PyComplexObject *)op)->cval;
	} else {
		cv.real = PyFloat_AsDouble(op);
		cv.imag = 0.;
		return cv;
	}   
}

static void
complex_dealloc(op)
	PyObject *op;
{
	PyMem_DEL(op);
}


static void
complex_buf_repr(buf, v)
	char *buf;
	PyComplexObject *v;
{
	if (v->cval.real == 0.)
		sprintf(buf, "%.12gj", v->cval.imag);
	else
		sprintf(buf, "(%.12g%+.12gj)", v->cval.real, v->cval.imag);
}

static int
complex_print(v, fp, flags)
	PyComplexObject *v;
	FILE *fp;
	int flags; /* Not used but required by interface */
{
	char buf[100];
	complex_buf_repr(buf, v);
	fputs(buf, fp);
	return 0;
}

static PyObject *
complex_repr(v)
	PyComplexObject *v;
{
	char buf[100];
	complex_buf_repr(buf, v);
	return PyString_FromString(buf);
}

static int
complex_compare(v, w)
	PyComplexObject *v, *w;
{
/* Note: "greater" and "smaller" have no meaning for complex numbers,
   but Python requires that they be defined nevertheless. */
	Py_complex i, j;
	i = v->cval;
	j = w->cval;
	if (i.real == j.real && i.imag == j.imag)
	   return 0;
	else if (i.real != j.real)
	   return (i.real < j.real) ? -1 : 1;
	else
	   return (i.imag < j.imag) ? -1 : 1;
}

static long
complex_hash(v)
	PyComplexObject *v;
{
	double intpart, fractpart;
	int expo;
	long hipart, x;
	/* This is designed so that Python numbers with the same
	   value hash to the same value, otherwise comparisons
	   of mapping keys will turn out weird */

#ifdef MPW /* MPW C modf expects pointer to extended as second argument */
{
	extended e;
	fractpart = modf(v->cval.real, &e);
	intpart = e;
}
#else
	fractpart = modf(v->cval.real, &intpart);
#endif

	if (fractpart == 0.0 && v->cval.imag == 0.0) {
		if (intpart > 0x7fffffffL || -intpart > 0x7fffffffL) {
			/* Convert to long int and use its hash... */
			PyObject *w = PyLong_FromDouble(v->cval.real);
			if (w == NULL)
				return -1;
			x = PyObject_Hash(w);
			Py_DECREF(w);
			return x;
		}
		x = (long)intpart;
	}
	else {
		fractpart = frexp(fractpart, &expo);
		fractpart = fractpart * 2147483648.0; /* 2**31 */
		hipart = (long)fractpart; /* Take the top 32 bits */
		fractpart = (fractpart - (double)hipart) * 2147483648.0;
						/* Get the next 32 bits */
		x = hipart + (long)fractpart + (long)intpart + (expo << 15);
						/* Combine everything */

		if (v->cval.imag != 0.0) { /* Hash the imaginary part */
			/* XXX Note that this hashes complex(x, y)
			   to the same value as complex(y, x).
			   Still better than it used to be :-) */
#ifdef MPW
			{
				extended e;
				fractpart = modf(v->cval.imag, &e);
				intpart = e;
			}
#else
			fractpart = modf(v->cval.imag, &intpart);
#endif
			fractpart = frexp(fractpart, &expo);
			fractpart = fractpart * 2147483648.0; /* 2**31 */
			hipart = (long)fractpart; /* Take the top 32 bits */
			fractpart =
				(fractpart - (double)hipart) * 2147483648.0;
						/* Get the next 32 bits */
			x ^= hipart + (long)fractpart +
				(long)intpart + (expo << 15);
						/* Combine everything */
		}
	}
	if (x == -1)
		x = -2;
	return x;
}

static PyObject *
complex_add(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
	Py_complex result;
	PyFPE_START_PROTECT("complex_add", return 0)
	result = c_sum(v->cval,w->cval);
	PyFPE_END_PROTECT(result)
	return PyComplex_FromCComplex(result);
}

static PyObject *
complex_sub(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
	Py_complex result;
	PyFPE_START_PROTECT("complex_sub", return 0)
	result = c_diff(v->cval,w->cval);
	PyFPE_END_PROTECT(result)
	return PyComplex_FromCComplex(result);
}

static PyObject *
complex_mul(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
	Py_complex result;
	PyFPE_START_PROTECT("complex_mul", return 0)
	result = c_prod(v->cval,w->cval);
	PyFPE_END_PROTECT(result)
	return PyComplex_FromCComplex(result);
}

static PyObject *
complex_div(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
	Py_complex quot;
	PyFPE_START_PROTECT("complex_div", return 0)
	errno = 0;
	quot = c_quot(v->cval,w->cval);
	PyFPE_END_PROTECT(quot)
	if (errno == EDOM) {
		PyErr_SetString(PyExc_ZeroDivisionError, "complex division");
		return NULL;
	}
	return PyComplex_FromCComplex(quot);
}

static PyObject *
complex_remainder(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
        Py_complex div, mod;
	errno = 0;
	div = c_quot(v->cval,w->cval); /* The raw divisor value. */
	if (errno == EDOM) {
		PyErr_SetString(PyExc_ZeroDivisionError, "complex remainder");
		return NULL;
	}
	div.real = floor(div.real); /* Use the floor of the real part. */
	div.imag = 0.0;
	mod = c_diff(v->cval, c_prod(w->cval, div));

	return PyComplex_FromCComplex(mod);
}


static PyObject *
complex_divmod(v, w)
	PyComplexObject *v;
	PyComplexObject *w;
{
        Py_complex div, mod;
	PyObject *d, *m, *z;
	errno = 0;
	div = c_quot(v->cval,w->cval); /* The raw divisor value. */
	if (errno == EDOM) {
		PyErr_SetString(PyExc_ZeroDivisionError, "complex divmod()");
		return NULL;
	}
	div.real = floor(div.real); /* Use the floor of the real part. */
	div.imag = 0.0;
	mod = c_diff(v->cval, c_prod(w->cval, div));
	d = PyComplex_FromCComplex(div);
	m = PyComplex_FromCComplex(mod);
	z = Py_BuildValue("(OO)", d, m);
	Py_XDECREF(d);
	Py_XDECREF(m);
	return z;
}

static PyObject *
complex_pow(v, w, z)
	PyComplexObject *v;
	PyObject *w;
	PyComplexObject *z;
{
	Py_complex p;
	Py_complex exponent;
	long int_exponent;

 	if ((PyObject *)z!=Py_None) {
		PyErr_SetString(PyExc_ValueError, "complex modulo");
		return NULL;
	}

	PyFPE_START_PROTECT("complex_pow", return 0)
	errno = 0;
	exponent = ((PyComplexObject*)w)->cval;
	int_exponent = (long)exponent.real;
	if (exponent.imag == 0. && exponent.real == int_exponent)
		p = c_powi(v->cval,int_exponent);
	else
		p = c_pow(v->cval,exponent);

	PyFPE_END_PROTECT(p)
	if (errno == ERANGE) {
		PyErr_SetString(PyExc_ValueError,
				"0.0 to a negative or complex power");
		return NULL;
	}

	return PyComplex_FromCComplex(p);
}

static PyObject *
complex_neg(v)
	PyComplexObject *v;
{
	Py_complex neg;
	neg.real = -v->cval.real;
	neg.imag = -v->cval.imag;
	return PyComplex_FromCComplex(neg);
}

static PyObject *
complex_pos(v)
	PyComplexObject *v;
{
	Py_INCREF(v);
	return (PyObject *)v;
}

static PyObject *
complex_abs(v)
	PyComplexObject *v;
{
	double result;
	PyFPE_START_PROTECT("complex_abs", return 0)
	result = hypot(v->cval.real,v->cval.imag);
	PyFPE_END_PROTECT(result)
	return PyFloat_FromDouble(result);
}

static int
complex_nonzero(v)
	PyComplexObject *v;
{
	return v->cval.real != 0.0 || v->cval.imag != 0.0;
}

static int
complex_coerce(pv, pw)
	PyObject **pv;
	PyObject **pw;
{
	Py_complex cval;
	cval.imag = 0.;
	if (PyInt_Check(*pw)) {
		cval.real = (double)PyInt_AsLong(*pw);
		*pw = PyComplex_FromCComplex(cval);
		Py_INCREF(*pv);
		return 0;
	}
	else if (PyLong_Check(*pw)) {
		cval.real = PyLong_AsDouble(*pw);
		*pw = PyComplex_FromCComplex(cval);
		Py_INCREF(*pv);
		return 0;
	}
	else if (PyFloat_Check(*pw)) {
		cval.real = PyFloat_AsDouble(*pw);
		*pw = PyComplex_FromCComplex(cval);
		Py_INCREF(*pv);
		return 0;
	}
	return 1; /* Can't do it */
}

static PyObject *
complex_int(v)
	PyObject *v;
{
	PyErr_SetString(PyExc_TypeError,
		   "can't convert complex to int; use e.g. int(abs(z))");
	return NULL;
}

static PyObject *
complex_long(v)
	PyObject *v;
{
	PyErr_SetString(PyExc_TypeError,
		   "can't convert complex to long; use e.g. long(abs(z))");
	return NULL;
}

static PyObject *
complex_float(v)
	PyObject *v;
{
	PyErr_SetString(PyExc_TypeError,
		   "can't convert complex to float; use e.g. abs(z)");
	return NULL;
}

static PyObject *
complex_conjugate(self, args)
	PyObject *self;
	PyObject *args;
{
	Py_complex c;
	if (!PyArg_ParseTuple(args, ":conjugate"))
		return NULL;
	c = ((PyComplexObject *)self)->cval;
	c.imag = -c.imag;
	return PyComplex_FromCComplex(c);
}

static PyMethodDef complex_methods[] = {
	{"conjugate",	complex_conjugate,	1},
	{NULL,		NULL}		/* sentinel */
};


static PyObject *
complex_getattr(self, name)
	PyComplexObject *self;
	char *name;
{
	if (strcmp(name, "real") == 0)
		return (PyObject *)PyFloat_FromDouble(self->cval.real);
	else if (strcmp(name, "imag") == 0)
		return (PyObject *)PyFloat_FromDouble(self->cval.imag);
	else if (strcmp(name, "__members__") == 0)
		return Py_BuildValue("[ss]", "imag", "real");
	return Py_FindMethod(complex_methods, (PyObject *)self, name);
}

static PyNumberMethods complex_as_number = {
	(binaryfunc)complex_add, /*nb_add*/
	(binaryfunc)complex_sub, /*nb_subtract*/
	(binaryfunc)complex_mul, /*nb_multiply*/
	(binaryfunc)complex_div, /*nb_divide*/
	(binaryfunc)complex_remainder,	/*nb_remainder*/
	(binaryfunc)complex_divmod,	/*nb_divmod*/
	(ternaryfunc)complex_pow, /*nb_power*/
	(unaryfunc)complex_neg, /*nb_negative*/
	(unaryfunc)complex_pos, /*nb_positive*/
	(unaryfunc)complex_abs, /*nb_absolute*/
	(inquiry)complex_nonzero, /*nb_nonzero*/
	0,		/*nb_invert*/
	0,		/*nb_lshift*/
	0,		/*nb_rshift*/
	0,		/*nb_and*/
	0,		/*nb_xor*/
	0,		/*nb_or*/
	(coercion)complex_coerce, /*nb_coerce*/
	(unaryfunc)complex_int, /*nb_int*/
	(unaryfunc)complex_long, /*nb_long*/
	(unaryfunc)complex_float, /*nb_float*/
	0,		/*nb_oct*/
	0,		/*nb_hex*/
};

PyTypeObject PyComplex_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"complex",
	sizeof(PyComplexObject),
	0,
	(destructor)complex_dealloc,	/*tp_dealloc*/
	(printfunc)complex_print,	/*tp_print*/
	(getattrfunc)complex_getattr,	/*tp_getattr*/
	0,				/*tp_setattr*/
	(cmpfunc)complex_compare,	/*tp_compare*/
	(reprfunc)complex_repr,		/*tp_repr*/
	&complex_as_number,    		/*tp_as_number*/
	0,				/*tp_as_sequence*/
	0,				/*tp_as_mapping*/
	(hashfunc)complex_hash, 	/*tp_hash*/
};

#endif
