
#include "Python.h"
#include "PalmOS.h"
#include "structmember.h"


#define BUF(v) PyString_AS_STRING((PyStringObject *)v)
#define hostfile_Check(op) ((op)->ob_type == &hostfile_Type)

static PyObject *palmemu_error;

typedef struct {
	PyObject_HEAD
	HostFILE *f_fp;
	PyObject *f_name;
	PyObject *f_mode;
	int (*f_close) Py_PROTO((HostFILE *));
	int f_softspace; /* Flag used by 'print' command */
} hostfileObject;


PyTypeObject hostfile_Type;


static PyObject *err_closed() SEG_PALMEMUMODULE_C;
HostFILE *hostfile_AsFile(PyObject *f) SEG_PALMEMUMODULE_C;
PyObject *hostfile_FromFile(HostFILE *fp, char *name,char *mode,
			    int (*close) Py_FPROTO((HostFILE *))) SEG_PALMEMUMODULE_C;
static int _close(HostFILE *f) SEG_PALMEMUMODULE_C;
PyObject *hostfile_FromString(char *name, char *mode) SEG_PALMEMUMODULE_C;
static void hostfile_dealloc(hostfileObject *f) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_repr(hostfileObject *f) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_close(hostfileObject *f, PyObject *args) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_flush(hostfileObject *f, PyObject *args) SEG_PALMEMUMODULE_C;
static size_t new_buffersize(hostfileObject *f, size_t currentsize) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_read(hostfileObject *f, PyObject *args) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_write(hostfileObject *f, PyObject *args) SEG_PALMEMUMODULE_C;
static PyObject *hostfile_getattr(hostfileObject *f, char *name) SEG_PALMEMUMODULE_C;
static int hostfile_setattr(hostfileObject *f, char *name, PyObject *v) SEG_PALMEMUMODULE_C;
static PyObject *palmemu_open_hostfile(PyObject *self,PyObject *args) SEG_PALMEMUMODULE_C;




static PyObject *
err_closed()
{
	PyErr_SetString(PyExc_ValueError, "I/O operation on closed file");
	return NULL;
}

HostFILE *
hostfile_AsFile(f)
	PyObject *f;
{
	if (f == NULL || !hostfile_Check(f))
		return NULL;
	else
		return ((hostfileObject *)f)->f_fp;
}


PyObject *
hostfile_FromFile(fp, name, mode, close)
	HostFILE *fp;
	char *name;
	char *mode;
	int (*close) Py_FPROTO((HostFILE *));
{
	hostfileObject *f = PyObject_NEW(hostfileObject, &hostfile_Type);
	if (f == NULL)
		return NULL;
	f->f_fp = NULL;
	f->f_name = PyString_FromString(name);
	f->f_mode = PyString_FromString(mode);
	f->f_close = close;
	f->f_softspace = 0;
	if (f->f_name == NULL || f->f_mode == NULL) {
		Py_DECREF(f);
		return NULL;
	}
	f->f_fp = fp;
	return (PyObject *) f;
}

static int
_close(HostFILE *f)
{
	return (int) HostFClose(f);
}

PyObject *
hostfile_FromString(name, mode)
	char *name, *mode;
{
	hostfileObject *f;
	f = (hostfileObject *) hostfile_FromFile((HostFILE *)NULL, name, mode, _close);
	if (f == NULL)
		return NULL;
	Py_BEGIN_ALLOW_THREADS
        f->f_fp = HostFOpen(name, mode);
	Py_END_ALLOW_THREADS

	if (f->f_fp == NULL) {
#ifdef NO_FOPEN_ERRNO
		/* Metroworks only, not testable, so unchanged */
		if ( errno == 0 ) {
			PyErr_SetString(PyExc_IOError, "Cannot open file");
			Py_DECREF(f);
			return NULL;
		}
#endif
		errno = (int) HostFError(f->f_fp);
		PyErr_SetFromErrnoWithFilename(PyExc_IOError, name);
		errno = 0;
		Py_DECREF(f);
		return NULL;
	}
	return (PyObject *)f;
}


/*  hostfileObject Methods */


static void
hostfile_dealloc(f)
	hostfileObject *f;
{
	if (f->f_fp != NULL && f->f_close != NULL) {
		Py_BEGIN_ALLOW_THREADS
		(*f->f_close)(f->f_fp);
		Py_END_ALLOW_THREADS
	}
	if (f->f_name != NULL) {
		Py_DECREF(f->f_name);
	}
	if (f->f_mode != NULL) {
		Py_DECREF(f->f_mode);
	}
	free((char *)f);
}

static PyObject *
hostfile_repr(f)
	hostfileObject *f;
{
	char buf[300];
	sprintf(buf, "<%s file '%.256s', mode '%.10s' at %lx>",
		f->f_fp == NULL ? "closed" : "open",
		PyString_AsString(f->f_name),
		PyString_AsString(f->f_mode),
		(long)f);
	return PyString_FromString(buf);
}

static PyObject *
hostfile_close(f, args)
	hostfileObject *f;
	PyObject *args;
{
	int sts = 0;
	if (!PyArg_NoArgs(args))
		return NULL;
	if (f->f_fp != NULL) {
		if (f->f_close != NULL) {
			Py_BEGIN_ALLOW_THREADS
			errno = 0;
			sts = (*f->f_close)(f->f_fp);
			Py_END_ALLOW_THREADS
		}
		f->f_fp = NULL;
	}
	if (sts != 0) {
		if (HostFEOF(f->f_fp)) {
			errno = HostFError(f->f_fp);
			return PyErr_SetFromErrno(PyExc_IOError);
		} 
		else {
			return PyInt_FromLong((long)sts);
		}
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
hostfile_flush(f, args)
	hostfileObject *f;
	PyObject *args;
{
	int res;
	
	if (f->f_fp == NULL)
		return err_closed();
	if (!PyArg_NoArgs(args))
		return NULL;
	Py_BEGIN_ALLOW_THREADS
	errno = 0;
	res = HostFFlush(f->f_fp);
	Py_END_ALLOW_THREADS
	if (res != 0) {
		PyErr_SetFromErrno(PyExc_IOError);
		errno = 0;
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}


#if BUFSIZ < 8192
#define SMALLCHUNK 8192
#else
#define SMALLCHUNK BUFSIZ
#endif

#define SIZEOF_INT 2

#if SIZEOF_INT < 4
#define BIGCHUNK  (512 * 32)
#else
#define BIGCHUNK  (512 * 1024)
#endif

static size_t
new_buffersize(f, currentsize)
	hostfileObject *f;
	size_t currentsize;
{
	if (currentsize > SMALLCHUNK) {
		/* Keep doubling until we reach BIGCHUNK;
		   then keep adding BIGCHUNK. */
		if (currentsize <= BIGCHUNK)
			return currentsize + currentsize;
		else
			return currentsize + BIGCHUNK;
	}
	return currentsize + SMALLCHUNK;
}

static PyObject *
hostfile_read(f, args)
	hostfileObject *f;
	PyObject *args;
{
	long bytesrequested = -1;
	size_t bytesread, buffersize, chunksize;
	PyObject *v;
	
	if (f->f_fp == NULL)
		return err_closed();
	if (!PyArg_ParseTuple(args, "|l:read", &bytesrequested))
		return NULL;
	if (bytesrequested < 0)
		buffersize = new_buffersize(f, (size_t)0);
	else
		buffersize = bytesrequested;
	v = PyString_FromStringAndSize((char *)NULL, buffersize);
	if (v == NULL)
		return NULL;
	bytesread = 0;
	for (;;) {
		Py_BEGIN_ALLOW_THREADS
		errno = 0;
		chunksize = HostFRead(BUF(v) + bytesread, 1,
				  buffersize - bytesread, f->f_fp);
		Py_END_ALLOW_THREADS
		if (chunksize == 0) {
			if (!HostFError(f->f_fp))
				break;
			PyErr_SetFromErrno(PyExc_IOError);
			Py_DECREF(v);
			return NULL;
		}
		bytesread += chunksize;
		if (bytesread < buffersize)
			break;
		if (bytesrequested < 0) {
			buffersize = new_buffersize(f, buffersize);
			if (_PyString_Resize(&v, buffersize) < 0)
				return NULL;
		}
	}
	if (bytesread != buffersize)
		_PyString_Resize(&v, bytesread);
	return v;
}




static PyObject *
hostfile_write(f, args)
	hostfileObject *f;
	PyObject *args;
{
	char *s;
	int n, n2;
	if (f->f_fp == NULL)
		return err_closed();
	if (!PyArg_Parse(args, "s#", &s, &n))
		return NULL;
	f->f_softspace = 0;
	Py_BEGIN_ALLOW_THREADS
	errno = 0;
	n2 = HostFWrite(s, 1, n, f->f_fp);
	Py_END_ALLOW_THREADS
	if (n2 != n) {
		errno = HostFError(f->f_fp);
		PyErr_SetFromErrno(PyExc_IOError);
		errno = 0;
		return NULL;
	}
	Py_INCREF(Py_None);
	return Py_None;
}


static PyMethodDef hostfile_methods[] = {
	{"read",	(PyCFunction)hostfile_read, 1},
	{"write",	(PyCFunction)hostfile_write, 0},
	{"flush",	(PyCFunction)hostfile_flush, 0},
	{"close",	(PyCFunction)hostfile_close, 0},
	{NULL,		NULL}		/* sentinel */
};

#define OFF(x) offsetof(hostfileObject, x)

static struct memberlist hostfile_memberlist[] = {
	{"softspace",	T_INT,		OFF(f_softspace)},
	{"mode",	T_OBJECT,	OFF(f_mode),	RO},
	{"name",	T_OBJECT,	OFF(f_name),	RO},
	/* getattr(f, "closed") is implemented without this table */
	{"closed",	T_INT,		0,		RO},
	{NULL}	/* Sentinel */
};

static PyObject *
hostfile_getattr(f, name)
	hostfileObject *f;
	char *name;
{
	PyObject *res;

	res = Py_FindMethod(hostfile_methods, (PyObject *)f, name);
	if (res != NULL)
		return res;
	PyErr_Clear();
	if (strcmp(name, "closed") == 0)
		return PyInt_FromLong((long)(f->f_fp == 0));
	return PyMember_Get((char *)f, hostfile_memberlist, name);
}

static int
hostfile_setattr(f, name, v)
	hostfileObject *f;
	char *name;
	PyObject *v;
{
	if (v == NULL) {
		PyErr_SetString(PyExc_AttributeError,
				"can't delete file attributes");
		return -1;
	}
	return PyMember_Set((char *)f, hostfile_memberlist, name, v);
}

PyTypeObject hostfile_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
	0,
	"hostfile",
	sizeof(hostfileObject),
	0,
	(destructor)hostfile_dealloc, /*tp_dealloc*/
	0,		/*tp_print*/
	(getattrfunc)hostfile_getattr, /*tp_getattr*/
	(setattrfunc)hostfile_setattr, /*tp_setattr*/
	0,		/*tp_compare*/
	(reprfunc)hostfile_repr, /*tp_repr*/
};



static PyObject *
palmemu_open_hostfile(self, args)
	PyObject *self;
	PyObject *args;
{
	char *name;
	char *mode = "r";
	int bufsize = -1;
	PyObject *f;

	if (!PyArg_ParseTuple(args, "s|si:open", &name, &mode, &bufsize))
		return NULL;
	f = hostfile_FromString(name, mode);
	/* no buffer setting */
	return f;
}


static PyMethodDef palmemu_methods[] = {
	{"openhostfile",	(PyCFunction)palmemu_open_hostfile, 1},
	{NULL,		NULL}		/* sentinel */
};


void initpalmemu(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmemu", palmemu_methods, NULL);
	d = PyModule_GetDict(m);
	palmemu_error = PyErr_NewException("palmemu.error", NULL, NULL);
	if (palmemu_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmemu_error);

	if (PyErr_Occurred)
		PyErr_Clear();

}
