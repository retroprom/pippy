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

/* strop module */

static char strop_module__doc__[] =
"Common string manipulations, optimized for speed.\n\
\n\
Always use \"import string\" rather than referencing\n\
this module directly.";

#include "Python.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#else
#define INT_MAX 2147483647
#endif

#include <ctype.h>
/* XXX This file assumes that the <ctype.h> is*() functions
   XXX are defined for all 8-bit characters! */

/* The lstrip(), rstrip() and strip() functions are implemented
   in do_strip(), which uses an additional parameter to indicate what
   type of strip should occur. */

#define LEFTSTRIP 0
#define RIGHTSTRIP 1
#define BOTHSTRIP 2


static PyObject *
split_whitespace(s, len, maxsplit)
	char *s;
	int len;
	int maxsplit;
{
	int i = 0, j, err;
	int countsplit = 0;
	PyObject* item;
	PyObject *list = PyList_New(0);

	if (list == NULL)
		return NULL;

	while (i < len) {
		while (i < len && isspace(Py_CHARMASK(s[i]))) {
			i = i+1;
		}
		j = i;
		while (i < len && !isspace(Py_CHARMASK(s[i]))) {
			i = i+1;
		}
		if (j < i) {
			item = PyString_FromStringAndSize(s+j, (int)(i-j));
			if (item == NULL)
				goto finally;

			err = PyList_Append(list, item);
			Py_DECREF(item);
			if (err < 0)
				goto finally;

			countsplit++;
			while (i < len && isspace(Py_CHARMASK(s[i]))) {
				i = i+1;
			}
			if (maxsplit && (countsplit >= maxsplit) && i < len) {
				item = PyString_FromStringAndSize(
                                        s+i, (int)(len - i));
				if (item == NULL)
					goto finally;

				err = PyList_Append(list, item);
				Py_DECREF(item);
				if (err < 0)
					goto finally;

				i = len;
			}
		}
	}
	return list;
  finally:
	Py_DECREF(list);
	return NULL;
}


static char splitfields__doc__[] =
"split(s [,sep [,maxsplit]]) -> list of strings\n\
splitfields(s [,sep [,maxsplit]]) -> list of strings\n\
\n\
Return a list of the words in the string s, using sep as the\n\
delimiter string.  If maxsplit is nonzero, splits into at most\n\
maxsplit words.  If sep is not specified, any whitespace string\n\
is a separator.  Maxsplit defaults to 0.\n\
\n\
(split and splitfields are synonymous)";

static PyObject *
strop_splitfields(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	int len, n, i, j, err;
	int splitcount, maxsplit;
	char *s, *sub;
	PyObject *list, *item;

	sub = NULL;
	n = 0;
	splitcount = 0;
	maxsplit = 0;
	if (!PyArg_ParseTuple(args, "t#|z#i:split", &s, &len, &sub, &n, &maxsplit))
		return NULL;
	if (sub == NULL)
		return split_whitespace(s, len, maxsplit);
	if (n == 0) {
		PyErr_SetString(PyExc_ValueError, "empty separator");
		return NULL;
	}

	list = PyList_New(0);
	if (list == NULL)
		return NULL;

	i = j = 0;
	while (i+n <= len) {
		if (s[i] == sub[0] && (n == 1 || memcmp(s+i, sub, n) == 0)) {
			item = PyString_FromStringAndSize(s+j, (int)(i-j));
			if (item == NULL)
				goto fail;
			err = PyList_Append(list, item);
			Py_DECREF(item);
			if (err < 0)
				goto fail;
			i = j = i + n;
			splitcount++;
			if (maxsplit && (splitcount >= maxsplit))
				break;
		}
		else
			i++;
	}
	item = PyString_FromStringAndSize(s+j, (int)(len-j));
	if (item == NULL)
		goto fail;
	err = PyList_Append(list, item);
	Py_DECREF(item);
	if (err < 0)
		goto fail;

	return list;

 fail:
	Py_DECREF(list);
	return NULL;
}


static char joinfields__doc__[] =
"join(list [,sep]) -> string\n\
joinfields(list [,sep]) -> string\n\
\n\
Return a string composed of the words in list, with\n\
intervening occurences of sep.  Sep defaults to a single\n\
space.\n\
\n\
(join and joinfields are synonymous)";

static PyObject *
strop_joinfields(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	PyObject *seq;
	char *sep = NULL;
	int seqlen, seplen = 0;
	int i, reslen = 0, slen = 0, sz = 100;
	PyObject *res = NULL;
	char* p = NULL;
	intargfunc getitemfunc;

	if (!PyArg_ParseTuple(args, "O|t#:join", &seq, &sep, &seplen))
		return NULL;
	if (sep == NULL) {
		sep = " ";
		seplen = 1;
	}

	seqlen = PySequence_Length(seq);
	if (seqlen < 0 && PyErr_Occurred())
		return NULL;

	if (seqlen == 1) {
		/* Optimization if there's only one item */
		PyObject *item = PySequence_GetItem(seq, 0);
		if (item && !PyString_Check(item)) {
			PyErr_SetString(PyExc_TypeError,
				 "first argument must be sequence of strings");
			Py_DECREF(item);
			return NULL;
		}
		return item;
	}

	if (!(res = PyString_FromStringAndSize((char*)NULL, sz)))
		return NULL;
	p = PyString_AsString(res);

	/* optimize for lists, since it's the most common case.  all others
	 * (tuples and arbitrary sequences) just use the sequence abstract
	 * interface.
	 */
	if (PyList_Check(seq)) {
		for (i = 0; i < seqlen; i++) {
			PyObject *item = PyList_GET_ITEM(seq, i);
			if (!PyString_Check(item)) {
				PyErr_SetString(PyExc_TypeError,
				"first argument must be sequence of strings");
				Py_DECREF(res);
				return NULL;
			}
			slen = PyString_GET_SIZE(item);
			while (reslen + slen + seplen >= sz) {
				if (_PyString_Resize(&res, sz * 2)) {
					Py_DECREF(res);
					return NULL;
				}
				sz *= 2;
				p = PyString_AsString(res) + reslen;
			}
			if (i > 0) {
				memcpy(p, sep, seplen);
				p += seplen;
				reslen += seplen;
			}
			memcpy(p, PyString_AS_STRING(item), slen);
			p += slen;
			reslen += slen;
		}
		if (_PyString_Resize(&res, reslen)) {
			Py_DECREF(res);
			res = NULL;
		}
		return res;
	}

	if (seq->ob_type->tp_as_sequence == NULL ||
		 (getitemfunc = seq->ob_type->tp_as_sequence->sq_item) == NULL)
	{
		PyErr_SetString(PyExc_TypeError,
				"first argument must be a sequence");
		return NULL;
	}
	/* This is now type safe */
	for (i = 0; i < seqlen; i++) {
		PyObject *item = getitemfunc(seq, i);
		if (!item || !PyString_Check(item)) {
			PyErr_SetString(PyExc_TypeError,
				 "first argument must be sequence of strings");
			Py_DECREF(res);
			Py_XDECREF(item);
			return NULL;
		}
		slen = PyString_GET_SIZE(item);
		while (reslen + slen + seplen >= sz) {
			if (_PyString_Resize(&res, sz * 2)) {
				Py_DECREF(res);
				Py_DECREF(item);
				return NULL;
			}
			sz *= 2;
			p = PyString_AsString(res) + reslen;
		}
		if (i > 0) {
			memcpy(p, sep, seplen);
			p += seplen;
			reslen += seplen;
		}
		memcpy(p, PyString_AS_STRING(item), slen);
		p += slen;
		reslen += slen;
		Py_DECREF(item);
	}
	if (_PyString_Resize(&res, reslen)) {
		Py_DECREF(res);
		res = NULL;
	}
	return res;
}


static char find__doc__[] =
"find(s, sub [,start [,end]]) -> in\n\
\n\
Return the lowest index in s where substring sub is found,\n\
such that sub is contained within s[start,end].  Optional\n\
arguments start and end are interpreted as in slice notation.\n\
\n\
Return -1 on failure.";

static PyObject *
strop_find(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *sub;
	int len, n, i = 0, last = INT_MAX;

	if (!PyArg_ParseTuple(args, "t#t#|ii:find", &s, &len, &sub, &n, &i, &last))
		return NULL;

	if (last > len)
		last = len;
	if (last < 0)
		last += len;
	if (last < 0)
		last = 0;
	if (i < 0)
		i += len;
	if (i < 0)
		i = 0;

	if (n == 0 && i <= last)
		return PyInt_FromLong((long)i);

	last -= n;
	for (; i <= last; ++i)
		if (s[i] == sub[0] &&
		    (n == 1 || memcmp(&s[i+1], &sub[1], n-1) == 0))
			return PyInt_FromLong((long)i);

	return PyInt_FromLong(-1L);
}


static char rfind__doc__[] =
"rfind(s, sub [,start [,end]]) -> int\n\
\n\
Return the highest index in s where substring sub is found,\n\
such that sub is contained within s[start,end].  Optional\n\
arguments start and end are interpreted as in slice notation.\n\
\n\
Return -1 on failure.";

static PyObject *
strop_rfind(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *sub;
	int len, n, j;
	int i = 0, last = INT_MAX;

	if (!PyArg_ParseTuple(args, "t#t#|ii:rfind", &s, &len, &sub, &n, &i, &last))
		return NULL;

	if (last > len)
		last = len;
	if (last < 0)
		last += len;
	if (last < 0)
		last = 0;
	if (i < 0)
		i += len;
	if (i < 0)
		i = 0;

	if (n == 0 && i <= last)
		return PyInt_FromLong((long)last);

	for (j = last-n; j >= i; --j)
		if (s[j] == sub[0] &&
		    (n == 1 || memcmp(&s[j+1], &sub[1], n-1) == 0))
			return PyInt_FromLong((long)j);

	return PyInt_FromLong(-1L);
}


static PyObject *
do_strip(args, striptype)
	PyObject *args;
	int striptype;
{
	char *s;
	int len, i, j;


	if (!PyArg_Parse(args, "t#", &s, &len))
		return NULL;

	i = 0;
	if (striptype != RIGHTSTRIP) {
		while (i < len && isspace(Py_CHARMASK(s[i]))) {
			i++;
		}
	}

	j = len;
	if (striptype != LEFTSTRIP) {
		do {
			j--;
		} while (j >= i && isspace(Py_CHARMASK(s[j])));
		j++;
	}

	if (i == 0 && j == len) {
		Py_INCREF(args);
		return args;
	}
	else
		return PyString_FromStringAndSize(s+i, j-i);
}


static char strip__doc__[] =
"strip(s) -> string\n\
\n\
Return a copy of the string s with leading and trailing\n\
whitespace removed.";

static PyObject *
strop_strip(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	return do_strip(args, BOTHSTRIP);
}


static char lstrip__doc__[] =
"lstrip(s) -> string\n\
\n\
Return a copy of the string s with leading whitespace removed.";

static PyObject *
strop_lstrip(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	return do_strip(args, LEFTSTRIP);
}


static char rstrip__doc__[] =
"rstrip(s) -> string\n\
\n\
Return a copy of the string s with trailing whitespace removed.";

static PyObject *
strop_rstrip(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	return do_strip(args, RIGHTSTRIP);
}


static char lower__doc__[] =
"lower(s) -> string\n\
\n\
Return a copy of the string s converted to lowercase.";

static PyObject *
strop_lower(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *s_new;
	int i, n;
	PyObject *new;
	int changed;

	if (!PyArg_Parse(args, "t#", &s, &n))
		return NULL;
	new = PyString_FromStringAndSize(NULL, n);
	if (new == NULL)
		return NULL;
	s_new = PyString_AsString(new);
	changed = 0;
	for (i = 0; i < n; i++) {
		int c = Py_CHARMASK(*s++);
		if (isupper(c)) {
			changed = 1;
			*s_new = tolower(c);
		} else
			*s_new = c;
		s_new++;
	}
	if (!changed) {
		Py_DECREF(new);
		Py_INCREF(args);
		return args;
	}
	return new;
}


static char upper__doc__[] =
"upper(s) -> string\n\
\n\
Return a copy of the string s converted to uppercase.";

static PyObject *
strop_upper(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *s_new;
	int i, n;
	PyObject *new;
	int changed;

	if (!PyArg_Parse(args, "t#", &s, &n))
		return NULL;
	new = PyString_FromStringAndSize(NULL, n);
	if (new == NULL)
		return NULL;
	s_new = PyString_AsString(new);
	changed = 0;
	for (i = 0; i < n; i++) {
		int c = Py_CHARMASK(*s++);
		if (islower(c)) {
			changed = 1;
			*s_new = toupper(c);
		} else
			*s_new = c;
		s_new++;
	}
	if (!changed) {
		Py_DECREF(new);
		Py_INCREF(args);
		return args;
	}
	return new;
}


static char capitalize__doc__[] =
"capitalize(s) -> string\n\
\n\
Return a copy of the string s with only its first character\n\
capitalized.";

static PyObject *
strop_capitalize(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *s_new;
	int i, n;
	PyObject *new;
	int changed;

	if (!PyArg_Parse(args, "t#", &s, &n))
		return NULL;
	new = PyString_FromStringAndSize(NULL, n);
	if (new == NULL)
		return NULL;
	s_new = PyString_AsString(new);
	changed = 0;
	if (0 < n) {
		int c = Py_CHARMASK(*s++);
		if (islower(c)) {
			changed = 1;
			*s_new = toupper(c);
		} else
			*s_new = c;
		s_new++;
	}
	for (i = 1; i < n; i++) {
		int c = Py_CHARMASK(*s++);
		if (isupper(c)) {
			changed = 1;
			*s_new = tolower(c);
		} else
			*s_new = c;
		s_new++;
	}
	if (!changed) {
		Py_DECREF(new);
		Py_INCREF(args);
		return args;
	}
	return new;
}


static char expandtabs__doc__[] =
"expandtabs(string, [tabsize]) -> string\n\
\n\
Expand tabs in a string, i.e. replace them by one or more spaces,\n\
depending on the current column and the given tab size (default 8).\n\
The column number is reset to zero after each newline occurring in the\n\
string.  This doesn't understand other non-printing characters.";

static PyObject *
strop_expandtabs(self, args)
	PyObject *self;
	PyObject *args;
{
	/* Original by Fredrik Lundh */
	char* e;
	char* p;
	char* q;
	int i, j;
	PyObject* out;
	char* string;
	int stringlen;
	int tabsize = 8;

	/* Get arguments */
	if (!PyArg_ParseTuple(args, "s#|i:expandtabs", &string, &stringlen, &tabsize))
		return NULL;
	if (tabsize < 1) {
		PyErr_SetString(PyExc_ValueError,
				"tabsize must be at least 1");
		return NULL;
	}

	/* First pass: determine size of output string */
	i = j = 0; /* j: current column; i: total of previous lines */
	e = string + stringlen;
	for (p = string; p < e; p++) {
		if (*p == '\t')
			j += tabsize - (j%tabsize);
		else {
			j++;
			if (*p == '\n') {
				i += j;
				j = 0;
			}
		}
	}

	/* Second pass: create output string and fill it */
	out = PyString_FromStringAndSize(NULL, i+j);
	if (out == NULL)
		return NULL;

	i = 0;
	q = PyString_AS_STRING(out);

	for (p = string; p < e; p++) {
		if (*p == '\t') {
			j = tabsize - (i%tabsize);
			i += j;
			while (j-- > 0)
				*q++ = ' ';
		} else {
			*q++ = *p;
			i++;
			if (*p == '\n')
				i = 0;
		}
	}

	return out;
}


static char count__doc__[] =
"count(s, sub[, start[, end]]) -> int\n\
\n\
Return the number of occurrences of substring sub in string\n\
s[start:end].  Optional arguments start and end are\n\
interpreted as in slice notation.";

static PyObject *
strop_count(self, args)
	PyObject *self;	/* Not used */
	PyObject *args;
{
	char *s, *sub;
	int len, n;
	int i = 0, last = INT_MAX;
	int m, r;

	if (!PyArg_ParseTuple(args, "t#t#|ii:count", &s, &len, &sub, &n, &i, &last))
		return NULL;
	if (last > len)
		last = len;
	if (last < 0)
		last += len;
	if (last < 0)
		last = 0;
	if (i < 0)
		i += len;
	if (i < 0)
		i = 0;
	m = last + 1 - n;
	if (n == 0)
		return PyInt_FromLong((long) (m-i));

	r = 0;
	while (i < m) {
		if (!memcmp(s+i, sub, n)) {
			r++;
			i += n;
		} else {
			i++;
		}
	}
	return PyInt_FromLong((long) r);
}


static char swapcase__doc__[] =
"swapcase(s) -> string\n\
\n\
Return a copy of the string s with upper case characters\n\
converted to lowercase and vice versa.";

static PyObject *
strop_swapcase(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *s_new;
	int i, n;
	PyObject *new;
	int changed;

	if (!PyArg_Parse(args, "t#", &s, &n))
		return NULL;
	new = PyString_FromStringAndSize(NULL, n);
	if (new == NULL)
		return NULL;
	s_new = PyString_AsString(new);
	changed = 0;
	for (i = 0; i < n; i++) {
		int c = Py_CHARMASK(*s++);
		if (islower(c)) {
			changed = 1;
			*s_new = toupper(c);
		}
		else if (isupper(c)) {
			changed = 1;
			*s_new = tolower(c);
		}
		else
			*s_new = c;
		s_new++;
	}
	if (!changed) {
		Py_DECREF(new);
		Py_INCREF(args);
		return args;
	}
	return new;
}


static char atoi__doc__[] =
"atoi(s [,base]) -> int\n\
\n\
Return the integer represented by the string s in the given\n\
base, which defaults to 10.  The string s must consist of one\n\
or more digits, possibly preceded by a sign.  If base is 0, it\n\
is chosen from the leading characters of s, 0 for octal, 0x or\n\
0X for hexadecimal.  If base is 16, a preceding 0x or 0X is\n\
accepted.";

static PyObject *
strop_atoi(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *end;
	int base = 10;
	long x;
	char buffer[256]; /* For errors */

	if (!PyArg_ParseTuple(args, "s|i:atoi", &s, &base))
		return NULL;

	if ((base != 0 && base < 2) || base > 36) {
		PyErr_SetString(PyExc_ValueError, "invalid base for atoi()");
		return NULL;
	}

	while (*s && isspace(Py_CHARMASK(*s)))
		s++;
	errno = 0;
	if (base == 0 && s[0] == '0')
		x = (long) PyOS_strtoul(s, &end, base);
	else
		x = PyOS_strtol(s, &end, base);
	if (end == s || !isalnum(end[-1]))
		goto bad;
	while (*end && isspace(Py_CHARMASK(*end)))
		end++;
	if (*end != '\0') {
  bad:
		sprintf(buffer, "invalid literal for atoi(): %.200s", s);
		PyErr_SetString(PyExc_ValueError, buffer);
		return NULL;
	}
	else if (errno != 0) {
		sprintf(buffer, "atoi() literal too large: %.200s", s);
		PyErr_SetString(PyExc_ValueError, buffer);
		return NULL;
	}
	return PyInt_FromLong(x);
}


static char atol__doc__[] =
"atol(s [,base]) -> long\n\
\n\
Return the long integer represented by the string s in the\n\
given base, which defaults to 10.  The string s must consist\n\
of one or more digits, possibly preceded by a sign.  If base\n\
is 0, it is chosen from the leading characters of s, 0 for\n\
octal, 0x or 0X for hexadecimal.  If base is 16, a preceding\n\
0x or 0X is accepted.  A trailing L or l is not accepted,\n\
unless base is 0.";

static PyObject *
strop_atol(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	char *s, *end;
	int base = 10;
	PyObject *x;
	char buffer[256]; /* For errors */

	if (!PyArg_ParseTuple(args, "s|i:atol", &s, &base))
		return NULL;

	if ((base != 0 && base < 2) || base > 36) {
		PyErr_SetString(PyExc_ValueError, "invalid base for atol()");
		return NULL;
	}

	while (*s && isspace(Py_CHARMASK(*s)))
		s++;
	if (s[0] == '\0') {
		PyErr_SetString(PyExc_ValueError, "empty string for atol()");
		return NULL;
	}
	x = PyLong_FromString(s, &end, base);
	if (x == NULL)
		return NULL;
	if (base == 0 && (*end == 'l' || *end == 'L'))
		end++;
	while (*end && isspace(Py_CHARMASK(*end)))
		end++;
	if (*end != '\0') {
		sprintf(buffer, "invalid literal for atol(): %.200s", s);
		PyErr_SetString(PyExc_ValueError, buffer);
		Py_DECREF(x);
		return NULL;
	}
	return x;
}


static char atof__doc__[] =
"atof(s) -> float\n\
\n\
Return the floating point number represented by the string s.";

static PyObject *
strop_atof(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	extern double strtod Py_PROTO((const char *, char **));
	char *s, *end;
	double x;
	char buffer[256]; /* For errors */

	if (!PyArg_ParseTuple(args, "s:atof", &s))
		return NULL;
	while (*s && isspace(Py_CHARMASK(*s)))
		s++;
	if (s[0] == '\0') {
		PyErr_SetString(PyExc_ValueError, "empty string for atof()");
		return NULL;
	}
	errno = 0;
	PyFPE_START_PROTECT("strop_atof", return 0)
	x = strtod(s, &end);
	PyFPE_END_PROTECT(x)
	while (*end && isspace(Py_CHARMASK(*end)))
		end++;
	if (*end != '\0') {
		sprintf(buffer, "invalid literal for atof(): %.200s", s);
		PyErr_SetString(PyExc_ValueError, buffer);
		return NULL;
	}
	else if (errno != 0) {
		sprintf(buffer, "atof() literal too large: %.200s", s);
		PyErr_SetString(PyExc_ValueError, buffer);
		return NULL;
	}
	return PyFloat_FromDouble(x);
}


static char maketrans__doc__[] =
"maketrans(frm, to) -> string\n\
\n\
Return a translation table (a string of 256 bytes long)\n\
suitable for use in string.translate.  The strings frm and to\n\
must be of the same length.";

static PyObject *
strop_maketrans(self, args)
	PyObject *self; /* Not used */
	PyObject *args;
{
	unsigned char *c, *from=NULL, *to=NULL;
	int i, fromlen=0, tolen=0;
	PyObject *result;

	if (!PyArg_ParseTuple(args, "t#t#:maketrans", &from, &fromlen, &to, &tolen))
		return NULL;

	if (fromlen != tolen) {
		PyErr_SetString(PyExc_ValueError,
				"maketrans arguments must have same length");
		return NULL;
	}

	result = PyString_FromStringAndSize((char *)NULL, 256);
	if (result == NULL)
		return NULL;
	c = (unsigned char *) PyString_AS_STRING((PyStringObject *)result);
	for (i = 0; i < 256; i++)
		c[i]=(unsigned char)i;
	for (i = 0; i < fromlen; i++)
		c[from[i]]=to[i];

	return result;
}


static char translate__doc__[] =
"translate(s,table [,deletechars]) -> string\n\
\n\
Return a copy of the string s, where all characters occurring\n\
in the optional argument deletechars are removed, and the\n\
remaining characters have been mapped through the given\n\
translation table, which must be a string of length 256.";

static PyObject *
strop_translate(self, args)
	PyObject *self;
	PyObject *args;
{
	register char *input, *table, *output;
	register int i, c, changed = 0;
	PyObject *input_obj;
	char *table1, *output_start, *del_table=NULL;
	int inlen, tablen, dellen = 0;
	PyObject *result;
	int trans_table[256];

	if (!PyArg_ParseTuple(args, "St#|t#:translate", &input_obj,
			      &table1, &tablen, &del_table, &dellen))
		return NULL;
	if (tablen != 256) {
		PyErr_SetString(PyExc_ValueError,
			      "translation table must be 256 characters long");
		return NULL;
	}

	table = table1;
	inlen = PyString_Size(input_obj);
	result = PyString_FromStringAndSize((char *)NULL, inlen);
	if (result == NULL)
		return NULL;
	output_start = output = PyString_AsString(result);
	input = PyString_AsString(input_obj);

	if (dellen == 0) {
		/* If no deletions are required, use faster code */
		for (i = inlen; --i >= 0; ) {
			c = Py_CHARMASK(*input++);
			if (Py_CHARMASK((*output++ = table[c])) != c)
				changed = 1;
		}
		if (changed)
			return result;
		Py_DECREF(result);
		Py_INCREF(input_obj);
		return input_obj;
	}

	for (i = 0; i < 256; i++)
		trans_table[i] = Py_CHARMASK(table[i]);

	for (i = 0; i < dellen; i++)
		trans_table[(int) Py_CHARMASK(del_table[i])] = -1;

	for (i = inlen; --i >= 0; ) {
		c = Py_CHARMASK(*input++);
		if (trans_table[c] != -1)
			if (Py_CHARMASK(*output++ = (char)trans_table[c]) == c)
				continue;
		changed = 1;
	}
	if (!changed) {
		Py_DECREF(result);
		Py_INCREF(input_obj);
		return input_obj;
	}
	/* Fix the size of the resulting string */
	if (inlen > 0 &&_PyString_Resize(&result, output-output_start))
		return NULL;
	return result;
}


/* What follows is used for implementing replace().  Perry Stoll. */

/*
  mymemfind

  strstr replacement for arbitrary blocks of memory.

  Locates the first occurance in the memory pointed to by MEM of the
  contents of memory pointed to by PAT.  Returns the index into MEM if
  found, or -1 if not found.  If len of PAT is greater than length of
  MEM, the function returns -1.
*/
static int mymemfind(mem, len, pat, pat_len)
	char *mem;
	int len;
	char *pat;
	int pat_len;
{
	register int ii;

	/* pattern can not occur in the last pat_len-1 chars */
	len -= pat_len;

	for (ii = 0; ii <= len; ii++) {
		if (mem[ii] == pat[0] &&
		    (pat_len == 1 ||
		     memcmp(&mem[ii+1], &pat[1], pat_len-1) == 0)) {
			return ii;
		}
	}
	return -1;
}

/*
  mymemcnt

   Return the number of distinct times PAT is found in MEM.
   meaning mem=1111 and pat==11 returns 2.
           mem=11111 and pat==11 also return 2.
 */
static int mymemcnt(mem, len, pat, pat_len)
	char *mem;
	int len;
	char *pat;
	int pat_len;
{
	register int offset = 0;
	int nfound = 0;

	while (len >= 0) {
		offset = mymemfind(mem, len, pat, pat_len);
		if (offset == -1)
			break;
		mem += offset + pat_len;
		len -= offset + pat_len;
		nfound++;
	}
	return nfound;
}

/*
   mymemreplace

   Return a string in which all occurences of PAT in memory STR are
   replaced with SUB.

   If length of PAT is less than length of STR or there are no occurences
   of PAT in STR, then the original string is returned. Otherwise, a new
   string is allocated here and returned.

   on return, out_len is:
       the length of output string, or
       -1 if the input string is returned, or
       unchanged if an error occurs (no memory).

   return value is:
       the new string allocated locally, or
       NULL if an error occurred.
*/
static char *mymemreplace(str, len, pat, pat_len, sub, sub_len, count, out_len)
	char *str;
	int len;     /* input string  */
	char *pat;
	int pat_len; /* pattern string to find */
	char *sub;
	int sub_len; /* substitution string */
	int count;   /* number of replacements, 0 == all */
	int *out_len;

{
	char *out_s;
	char *new_s;
	int nfound, offset, new_len;

	if (len == 0 || pat_len > len)
		goto return_same;

	/* find length of output string */
	nfound = mymemcnt(str, len, pat, pat_len);
	if (count > 0)
		nfound = nfound > count ? count : nfound;
	if (nfound == 0)
		goto return_same;
	new_len = len + nfound*(sub_len - pat_len);

	new_s = (char *)malloc(new_len);
	if (new_s == NULL) return NULL;

	*out_len = new_len;
	out_s = new_s;

	while (len > 0) {
		/* find index of next instance of pattern */
		offset = mymemfind(str, len, pat, pat_len);
		/* if not found,  break out of loop */
		if (offset == -1) break;

		/* copy non matching part of input string */
		memcpy(new_s, str, offset); /* copy part of str before pat */
		str += offset + pat_len; /* move str past pattern */
		len -= offset + pat_len; /* reduce length of str remaining */

		/* copy substitute into the output string */
		new_s += offset; /* move new_s to dest for sub string */
		memcpy(new_s, sub, sub_len); /* copy substring into new_s */
		new_s += sub_len; /* offset new_s past sub string */

		/* break when we've done count replacements */
		if (--count == 0) break;
	}
	/* copy any remaining values into output string */
	if (len > 0)
		memcpy(new_s, str, len);
	return out_s;

  return_same:
	*out_len = -1;
	return str;
}


static char replace__doc__[] =
"replace (str, old, new[, maxsplit]) -> string\n\
\n\
Return a copy of string str with all occurrences of substring\n\
old replaced by new. If the optional argument maxsplit is\n\
given, only the first maxsplit occurrences are replaced.";

static PyObject *
strop_replace(self, args)
	PyObject *self;	/* Not used */
	PyObject *args;
{
	char *str, *pat,*sub,*new_s;
	int len,pat_len,sub_len,out_len;
	int count = 0;
	PyObject *new;

	if (!PyArg_ParseTuple(args, "t#t#t#|i:replace",
			      &str, &len, &pat, &pat_len, &sub, &sub_len,
			      &count))
		return NULL;
	if (pat_len <= 0) {
		PyErr_SetString(PyExc_ValueError, "empty pattern string");
		return NULL;
	}
	new_s = mymemreplace(str,len,pat,pat_len,sub,sub_len,count,&out_len);
	if (new_s == NULL) {
		PyErr_NoMemory();
		return NULL;
	}
	if (out_len == -1) {
		/* we're returning another reference to the input string */
		new = PyTuple_GetItem(args, 0);
		Py_XINCREF(new);
	}
	else {
		new = PyString_FromStringAndSize(new_s, out_len);
		free(new_s);
	}
	return new;
}


/* List of functions defined in the module */

static PyMethodDef
strop_methods[] = {
	{"atof",	strop_atof, 1, atof__doc__},
	{"atoi",	strop_atoi, 1, atoi__doc__},
	{"atol",	strop_atol, 1, atol__doc__},
	{"capitalize",	strop_capitalize, 0, capitalize__doc__},
	{"count",	strop_count, 1, count__doc__},
	{"expandtabs",	strop_expandtabs, 1, expandtabs__doc__},
	{"find",	strop_find, 1, find__doc__},
	{"join",	strop_joinfields, 1, joinfields__doc__},
	{"joinfields",	strop_joinfields, 1, joinfields__doc__},
	{"lstrip",	strop_lstrip, 0, lstrip__doc__},
	{"lower",	strop_lower, 0, lower__doc__},
	{"maketrans",	strop_maketrans, 1, maketrans__doc__},
	{"replace",	strop_replace, 1, replace__doc__},
	{"rfind",	strop_rfind, 1, rfind__doc__},
	{"rstrip",	strop_rstrip, 0, rstrip__doc__},
	{"split",	strop_splitfields, 1, splitfields__doc__},
	{"splitfields",	strop_splitfields, 1, splitfields__doc__},
	{"strip",	strop_strip, 0, strip__doc__},
	{"swapcase",	strop_swapcase, 0, swapcase__doc__},
	{"translate",	strop_translate, 1, translate__doc__},
	{"upper",	strop_upper, 0, upper__doc__},
	{NULL,		NULL}	/* sentinel */
};


DL_EXPORT(void)
initstrop()
{
	PyObject *m, *d, *s;
	char buf[256];
	int c, n;
	m = Py_InitModule4("strop", strop_methods, strop_module__doc__,
			   (PyObject*)NULL, PYTHON_API_VERSION);
	d = PyModule_GetDict(m);

	/* Create 'whitespace' object */
	n = 0;
	for (c = 0; c < 256; c++) {
		if (isspace(c))
			buf[n++] = c;
	}
	s = PyString_FromStringAndSize(buf, n);
	if (s) {
		PyDict_SetItemString(d, "whitespace", s);
		Py_DECREF(s);
	}
	/* Create 'lowercase' object */
	n = 0;
	for (c = 0; c < 256; c++) {
		if (islower(c))
			buf[n++] = c;
	}
	s = PyString_FromStringAndSize(buf, n);
	if (s) {
		PyDict_SetItemString(d, "lowercase", s);
		Py_DECREF(s);
	}

	/* Create 'uppercase' object */
	n = 0;
	for (c = 0; c < 256; c++) {
		if (isupper(c))
			buf[n++] = c;
	}
	s = PyString_FromStringAndSize(buf, n);
	if (s) {
		PyDict_SetItemString(d, "uppercase", s);
		Py_DECREF(s);
	}

	if (PyErr_Occurred())
		Py_FatalError("can't initialize module strop");
}
