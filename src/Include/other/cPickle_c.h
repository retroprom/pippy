#ifndef CPICKLE_C_H
#define CPICKLE_C_H

static void 
Pdata_dealloc(Pdata *self) SEG_CPICKLE_C;

static PyObject *
Pdata_New() SEG_CPICKLE_C;
static int 
stackUnderflow() SEG_CPICKLE_C;
static int
Pdata_clear(Pdata *self, int clearto) SEG_CPICKLE_C;
static int 
Pdata_grow(Pdata *self) SEG_CPICKLE_C;
static PyObject *
Pdata_popTuple(Pdata *self, int start) SEG_CPICKLE_C;
static PyObject *
Pdata_popList(Pdata *self, int start) SEG_CPICKLE_C;

static
PyObject *
#ifdef HAVE_STDARG_PROTOTYPES
/* VARARGS 2 */
cPickle_ErrFormat(PyObject *ErrType, char *stringformat, char *format, ...) SEG_CPICKLE_C;
#else
/* VARARGS */
cPickle_ErrFormat(va_alist) va_dcl SEG_CPICKLE_C;
#endif


static int 
write_file(Picklerobject *self, char *s, int  n) SEG_CPICKLE_C;
static int 
write_cStringIO(Picklerobject *self, char *s, int  n) SEG_CPICKLE_C;
static int 
write_none(Picklerobject *self, char *s, int  n) SEG_CPICKLE_C;
static int 
write_other(Picklerobject *self, char *s, int  n) SEG_CPICKLE_C;
static int 
read_file(Unpicklerobject *self, char **s, int  n) SEG_CPICKLE_C;
static int 
readline_file(Unpicklerobject *self, char **s) SEG_CPICKLE_C;
static int 
read_cStringIO(Unpicklerobject *self, char **s, int  n) SEG_CPICKLE_C;
static int 
readline_cStringIO(Unpicklerobject *self, char **s) SEG_CPICKLE_C;
static int 
read_other(Unpicklerobject *self, char **s, int  n) SEG_CPICKLE_C;
static int 
readline_other(Unpicklerobject *self, char **s) SEG_CPICKLE_C;
static char *
pystrndup(char *s, int l) SEG_CPICKLE_C;
static int
get(Picklerobject *self, PyObject *id) SEG_CPICKLE_C;
static int
put(Picklerobject *self, PyObject *ob) SEG_CPICKLE_C;
static int
put2(Picklerobject *self, PyObject *ob) SEG_CPICKLE_C;
static PyObject *
whichmodule(PyObject *global, PyObject *global_name) SEG_CPICKLE_C;
static int
save_none(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_int(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_long(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_float(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_float(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_string(Picklerobject *self, PyObject *args, int doput) SEG_CPICKLE_C;
static int
save_tuple(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_empty_tuple(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_list(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_dict(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int  
save_inst(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static int
save_global(Picklerobject *self, PyObject *args, PyObject *name) SEG_CPICKLE_C;
static int
save_pers(Picklerobject *self, PyObject *args, PyObject *f) SEG_CPICKLE_C;
static int 
save_reduce(Picklerobject *self, PyObject *callable,
            PyObject *tup, PyObject *state, PyObject *ob) SEG_CPICKLE_C;
static int
save(Picklerobject *self, PyObject *args, int  pers_save) SEG_CPICKLE_C;
static int
dump(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
Pickle_clear_memo(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
Pickle_getvalue(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
Pickler_dump(Picklerobject *self, PyObject *args) SEG_CPICKLE_C;
static Picklerobject *
newPicklerobject(PyObject *file, int bin) SEG_CPICKLE_C;
static PyObject *
get_Pickler(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static void
Pickler_dealloc(Picklerobject *self) SEG_CPICKLE_C;
static PyObject *
Pickler_getattr(Picklerobject *self, char *name) SEG_CPICKLE_C;
static PyObject *
find_class(PyObject *py_module_name, PyObject *py_global_name, PyObject *fc) SEG_CPICKLE_C;
static int
marker(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_none(Unpicklerobject *self) SEG_CPICKLE_C;
static int
bad_readline() SEG_CPICKLE_C;
static int
load_int(Unpicklerobject *self) SEG_CPICKLE_C;
static long 
calc_binint(char *s, int  x) SEG_CPICKLE_C;
static int
load_binintx(Unpicklerobject *self, char *s, int  x) SEG_CPICKLE_C;
static int
load_binint(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binint1(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binint2(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_long(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_float(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binfloat(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_float(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binfloat(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_string(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binstring(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_short_binstring(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_tuple(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_empty_tuple(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_empty_list(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_empty_dict(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_list(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_dict(Unpicklerobject *self) SEG_CPICKLE_C;
static PyObject *
Instance_New(PyObject *cls, PyObject *args) SEG_CPICKLE_C;
static int
load_obj(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_inst(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_global(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_persid(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binpersid(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_pop(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_pop_mark(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_dup(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_get(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binget(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_long_binget(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_put(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_binput(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_long_binput(Unpicklerobject *self) SEG_CPICKLE_C;
static int 
do_append(Unpicklerobject *self, int  x) SEG_CPICKLE_C;
static int
load_append(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_appends(Unpicklerobject *self) SEG_CPICKLE_C;
static int
do_setitems(Unpicklerobject *self, int  x) SEG_CPICKLE_C;
static int
load_setitem(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_setitems(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_build(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_mark(Unpicklerobject *self) SEG_CPICKLE_C;
static int
load_reduce(Unpicklerobject *self) SEG_CPICKLE_C;
static PyObject *
load(Unpicklerobject *self) SEG_CPICKLE_C;
static int
noload_obj(Unpicklerobject *self) SEG_CPICKLE_C;
static int
noload_inst(Unpicklerobject *self) SEG_CPICKLE_C;
static int
noload_global(Unpicklerobject *self) SEG_CPICKLE_C;
static int
noload_reduce(Unpicklerobject *self) SEG_CPICKLE_C;
static int
noload_build(Unpicklerobject *self) SEG_CPICKLE_C;
static PyObject *
noload(Unpicklerobject *self) SEG_CPICKLE_C;
static PyObject *
Unpickler_load(Unpicklerobject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
Unpickler_noload(Unpicklerobject *self, PyObject *args) SEG_CPICKLE_C;
static Unpicklerobject *
newUnpicklerobject(PyObject *f) SEG_CPICKLE_C;
static PyObject *
get_Unpickler(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static void
Unpickler_dealloc(Unpicklerobject *self) SEG_CPICKLE_C;
static PyObject *
Unpickler_getattr(Unpicklerobject *self, char *name) SEG_CPICKLE_C;
static int
Unpickler_setattr(Unpicklerobject *self, char *name, PyObject *value) SEG_CPICKLE_C;
static PyObject *
cpm_dump(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
cpm_dumps(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
cpm_load(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static PyObject *
cpm_loads(PyObject *self, PyObject *args) SEG_CPICKLE_C;
static int
init_stuff(PyObject *module, PyObject *module_dict) SEG_CPICKLE_C;

static int 
cPickle_PyMapping_HasKey(PyObject *o, PyObject *key) SEG_CPICKLE_C;
static int 
Pickler_setattr(Picklerobject *self, char *name, PyObject *value) SEG_CPICKLE_C;

#endif /* CPICKLE_C_H */

