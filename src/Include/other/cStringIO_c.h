#ifndef CSTRINGIO_C_H
#define CSTRINGIO_C_H

static PyObject *
IO_StringIO(PyObject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
I_close(Iobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_reset(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_tell(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_seek(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static int
O_cread(PyObject *self, char **output, int  n) SEG_CSTRINGIO_C;
static PyObject *
O_read(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static int
O_creadline(PyObject *self, char **output) SEG_CSTRINGIO_C;
static PyObject *
O_readline(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static int
O_cwrite(PyObject *self, char *c, int  l) SEG_CSTRINGIO_C;
static PyObject *
O_write(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_getval(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_cgetval(PyObject *self) SEG_CSTRINGIO_C;
static PyObject *
O_truncate(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_isatty(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_close(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_flush(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
O_writelines(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static void
O_dealloc(Oobject *self) SEG_CSTRINGIO_C;
static PyObject *
O_getattr(Oobject *self, char *name) SEG_CSTRINGIO_C;
static int
O_setattr(Oobject *self, char *name, PyObject *value) SEG_CSTRINGIO_C;
static PyObject *
newOobject(int  size) SEG_CSTRINGIO_C;
static PyObject *
I_close(Iobject *self, PyObject *args) SEG_CSTRINGIO_C;
static PyObject *
I_seek(Oobject *self, PyObject *args) SEG_CSTRINGIO_C;
static void
I_dealloc(Iobject *self) SEG_CSTRINGIO_C;
static PyObject *
I_getattr(Iobject *self, char *name) SEG_CSTRINGIO_C;
static PyObject *
newIobject(PyObject *s) SEG_CSTRINGIO_C;
static PyObject *
IO_StringIO(PyObject *self, PyObject *args) SEG_CSTRINGIO_C;

#endif /* CSTRINGIO_C_H */

