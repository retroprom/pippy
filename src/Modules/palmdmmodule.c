/*  First cut at exporting the PalmOS Data Manager interface */



#include "Python.h"
#include <PalmOS.h>
#define IGNORE_UNISTD_STUBS  /* causes portions of sys_socket.h to be ignored */
#define IGNORE_STDLIB_STUBS  /* causes portions of sys_socket.h to be ignored */
#define IGNORE_SYS_TYPES  /* portions of sys_types.h ignored */
#define __stdarg_h  /* causes unix_stdarg.h be effectively ignored */
#define __stdio_h  /* causes unix_stdio.h to be ignored */
#include "sys_types.h"
#include "_palmutils.h"


static PyObject *palmdm_error;


typedef struct {
	PyObject_HEAD

/* Palm Database Object */
	DmOpenRef db;
	LocalID localid;
	UInt16 card;
	UInt16 mode;

} palmdbobject;

static PyTypeObject PalmDBType;

typedef struct {
	PyObject_HEAD

/* Palm Record Object */
	UInt16 mode;
	UInt16 index;
	PyBufferObject *buf;
	palmdbobject *ob_dm;

} palmrecobject;

static PyTypeObject PalmRecType;

static PyObject *newrectype(UInt16 index, PyBufferObject *buf, palmdbobject* ob_dm) SEG_DMMODULE_C;
static void palmrec_dealloc(palmrecobject *dp) SEG_DMMODULE_C;
static PyObject *newdbtype(DmOpenRef db, LocalID localid, UInt16 card, UInt16 mode) SEG_DMMODULE_C;
static PyObject *palmdm_open(PyObject *self, PyObject *args) SEG_DMMODULE_C;
static void palmdb_dealloc(palmdbobject *dp) SEG_DMMODULE_C;
static PyObject *palmdb_close(palmdbobject *dp, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmdb_listcategories(palmdbobject *dp, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmdb_listrecords(palmdbobject *dp, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmdb_info(palmdbobject *dp, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmdb_queryrecord(palmdbobject *dp, PyObject *args) SEG_DMMODULE_C;
void initpalmdm(void) SEG_DMMODULE_C;
static PyObject *palmrec_info(palmrecobject *rec, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmrec_release(palmrecobject *rec, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmrec_getbuffer(palmrecobject *rec, PyObject *args) SEG_DMMODULE_C;
static PyObject *palmrec_getattr(PyObject *dp, char *name) SEG_DMMODULE_C;
static PyObject *palmdb_getattr(PyObject *dp, char *name) SEG_DMMODULE_C;


static PyObject *
newrectype(UInt16 index, PyBufferObject *buf, palmdbobject* ob_dm)
{
	palmrecobject *dp;

	if (!( dp = PyObject_NEW(palmrecobject, &PalmRecType)))
		return NULL;

	/* need to add additional attributes */
	Py_INCREF(ob_dm);
	dp->ob_dm = ob_dm;
	dp->index = index;
	dp->buf = buf;
	dp->mode = ob_dm->mode;
	return (PyObject *) dp;
}


static void
palmrec_dealloc(palmrecobject *dp)
{
	Py_DECREF(dp->ob_dm);
	dp->mode = 0;
	Py_XDECREF(dp->buf);
	PyMem_DEL(dp);
}




static PyObject *
newdbtype(DmOpenRef db, LocalID localid, UInt16 card, UInt16 mode)
{
	palmdbobject *dp;

	if (!( dp = PyObject_NEW(palmdbobject, &PalmDBType)))
		return NULL;

	/* need to add additional attributes */
	dp->db = db;
	dp->localid = localid;
	dp->card = card;
	dp->mode = mode;
	return (PyObject *) dp;

}


static PyObject *
palmdm_open(PyObject *self, PyObject *args)
{
	/* open a database return a database object */
/*
  mode = 'r', 'w', 'rw'
  modifiers: 'e' = exclusive, 'o' = leave open after app quits,
             's' = show private records
	     'N' = no overlay (see DmOpenDBNoOverlay)
*/
	PyObject *name;
	char *dbname;
	char *creator_s, *type_s, *mode_s;
	UInt32 type, creator;
	DmOpenRef db;
	UInt16 mode = 0;
	char *chr;
	UInt8 no_overlay = 0;
	UInt8 by_type_creator = 0;
	int i;

	UInt16 card = 0;
	LocalID dbID;

	if (!PyArg_ParseTuple(args, "O|s:open", &name, &mode_s))
		return NULL;

	chr = mode_s;
	for (i=0; i<strlen(mode_s); i++, chr++){ 
		switch (*chr){
		case 'r':
			mode |= dmModeReadOnly;
			break;
		case 'w':
			mode |= dmModeWrite;
			break;
		case 'o':
			mode |= dmModeLeaveOpen;
			break;
		case 'e':
			mode |= dmModeExclusive;
			break;
		case 's':
			mode |= dmModeShowSecret;
			break;
		case 'N':
			no_overlay = 1;
			break;
		default:
			PyErr_SetString(palmdm_error, "Illegal mode");
			return NULL;
		}
	}
	
	if (PyString_Check(name))
		dbname = PyString_AsString(name);
	else if(PyTuple_Check(name)){
		if (!(PyArg_ParseTuple(name, "ss", &type_s, &creator_s))){
			PyErr_SetString(palmdm_error, 
					"Must be type, creator as strings");
			return NULL;
		}
		if (strlen(type_s) != 4 || strlen(creator_s) != 4) {
			PyErr_SetString(palmdm_error, 
					"Type and creator must be exactly 4 characters");
			return NULL;
		}       
		
		bcopy(creator_s, &creator, 4);
		bcopy(type_s, &type, 4);

		printf("type: %ld   creator: %ld\n", type, creator);
		by_type_creator = 1;
	}
	else {
		PyErr_SetString(palmdm_error, 
				"Database name must be by name or (type,creator)");
		return NULL;
	}
	

	/* now that we have the arguments, lets select the open by
	   the appropriate method */

	dbID = 0;  /* default value for opening by type/creator */
	if (by_type_creator) {
		if (!(db = DmOpenDatabaseByTypeCreator (type, creator, mode)))
			return palmutil_setErr_FromDMError(palmdm_error);
	}
	else if (!no_overlay) {
		if (!(dbID = DmFindDatabase(card, dbname)))
			return palmutil_setErr_FromDMError(palmdm_error);
		if (!(db = DmOpenDatabase (card, dbID, mode)))
			return palmutil_setErr_FromDMError(palmdm_error);
	}
	else { /* no_overlay case */
		if (!(dbID = DmFindDatabase(card, dbname)))
			return palmutil_setErr_FromDMError(palmdm_error);
		if (!(db = DmOpenDatabase (card, dbID, mode)))
			return palmutil_setErr_FromDMError(palmdm_error);
	}
		
	return newdbtype(db, dbID, card, mode);
}

static void
palmdb_dealloc(palmdbobject *dp)
{

	if (dp->db) /* still open: close and ignore errors */
		DmCloseDatabase(dp->db);
	dp->db = 0;
	dp->localid = 0;
			
	PyMem_DEL(dp);

}


static PyObject*
palmdb_close(palmdbobject *dp, PyObject *args)
{
	Err err;
	/* need to determine whether there are records still open
	   and perform some cleanup prior to closing */

	if (!PyArg_NoArgs(args))
		return NULL;

	if (!dp->db){
		PyErr_SetString(palmdm_error, "Database is closed");
		return NULL;
	}
		
	if ((err = DmCloseDatabase(dp->db)) != errNone )
		return palmutil_setErr(palmdm_error, err);
	dp->db = 0;
		
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject*
palmdb_listcategories(palmdbobject *dp, PyObject *args)
{
	UInt16 i;
	char name[dmCategoryLength];
	PyObject *list, *str;

	if (!PyArg_Parse(args,""))
		return NULL;
	if (!dp->db){
		PyErr_SetString(palmdm_error, "Database is closed");
		return NULL;
	}

	if ((list = PyList_New(dmRecNumCategories)) == NULL)
		return NULL;
	for (i=0; i<dmRecNumCategories; i++){
		CategoryGetName (dp->db, i, name);
		if ((str = PyString_FromString(name)) == NULL)
			return NULL;
		if (PyList_SetItem(list,i, str) < -1)
			return NULL;
	}
	
	return list;
}

static PyObject*
palmdb_listrecords(palmdbobject *dp, PyObject *args)
{
	char *category = NULL;
	UInt16 cat_index = dmAllCategories;
	UInt16 i, index, numrecs;
	MemHandle rechand;
	PyObject *list, *obj;

	if (!dp->db){
		PyErr_SetString(palmdm_error, "Database is closed");
		return NULL;
	}
	if (!PyArg_ParseTuple(args,"|s", &category))
		return NULL;

	if (category)
		cat_index = CategoryFind(dp->db, category);

	numrecs = DmNumRecordsInCategory(dp->db, cat_index);
	if ((list = PyList_New(numrecs)) == NULL)
		return NULL;
	index = 0;
	for (i=0; i<numrecs; i++){
		rechand = DmQueryNextInCategory (dp->db, &index, cat_index);
		if ((obj=PyInt_FromLong((Int32)index)) == NULL)
			return NULL;
		if (PyList_SetItem(list, i, obj) == -1)
			return NULL;
		index++;
	}
	
	return list;
}

static PyObject*
palmdb_info(palmdbobject *dp, PyObject *args)
{
	UInt16 attributes, version;
	Char name[32];
	UInt32 crDate, modDate, bckUpDate, modNum, type, creator;
	LocalID appInfoID, sortInfoID;
	Err err;
	char type_s[5], creator_s[5];

	if (!PyArg_Parse(args,""))
		return NULL;
	if (!dp->db){
		PyErr_SetString(palmdm_error, "Database is closed");
		return NULL;
	}
	if (!dp->localid){
		PyErr_SetString(palmdm_error, 
				"No local ID available - DB opened by type/creator?");
		return NULL;
	}
	if ( (err = DmDatabaseInfo (dp->card, dp->localid, name,
				    &attributes, &version, &crDate, &modDate,
				    &bckUpDate, &modNum, 
				    &appInfoID, &sortInfoID, 
				    &type, &creator)) != errNone)
		return palmutil_setErr(palmdm_error, err);

	Py_INCREF(Py_None);
	Py_INCREF(Py_None);
	bcopy(&creator, creator_s, 4); creator_s[4]='\0';
	bcopy(&type, type_s, 4); type_s[4]='\0';
	return Py_BuildValue("sllllllOOss", name, (Int32)attributes, (Int32)version, 
			     crDate, modDate, bckUpDate, modNum, Py_None, Py_None,
			     type_s, creator_s);

}
		
static PyObject *
palmdb_queryrecord(palmdbobject *dp, PyObject *args)
{
	Int32 indx;
	UInt16 index, index_wanted, cat_index = dmAllCategories;
	MemPtr recptr;
	MemHandle rechand;
	PyBufferObject *recbuf;
	palmrecobject *op;

	if ( !PyArg_ParseTuple(args, "l", &indx))
		return NULL;
	index_wanted = index = (UInt16)indx;
	
	rechand = DmQueryNextInCategory(dp->db, &index, cat_index);
	if (index_wanted != index) {
		PyErr_SetString(palmdm_error, "No record at that index");
		return NULL;
	}

	recptr = MemHandleLock(rechand);
	
	recbuf = (PyBufferObject *)PyBuffer_FromMemory(recptr, 
						       MemHandleSize(rechand),
						       (unsigned int)PyBuffer_BASE);

	if (recbuf == NULL)
		return NULL;
	
	if ((op = (palmrecobject *)newrectype(index, recbuf, dp)) == NULL)
		return NULL;

	op->mode |= dmModeReadOnly;
	return (PyObject *)op;
	
}


static PyMethodDef palmdm_methods[] = {
	{"open",  (PyCFunction)palmdm_open, METH_VARARGS},
	{NULL, NULL} };



void initpalmdm(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmdm", palmdm_methods, NULL);
	d = PyModule_GetDict(m);
	palmdm_error = PyErr_NewException("palmdm.error", NULL, NULL);
	if (palmdm_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmdm_error);

}


static PyObject *
palmrec_info(palmrecobject *rec, PyObject *args)
{
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmrec_release(palmrecobject *rec, PyObject *args)
{
	int idirty=0;
	Boolean dirty;
	Err err;

	/* check to see whether the record has already been released */
	if (!PyBuffer_IsValid(rec->buf)) {
		PyErr_SetString(palmdm_error, "Record has already been released");
		return NULL;
	}

	if ( !PyArg_ParseTuple(args, "|i", &idirty))
		return NULL;
	dirty = (idirty ? true: false);
	

	if (rec->mode & dmModeWrite)
		if ((err=DmReleaseRecord (rec->ob_dm->db, rec->index, dirty)) != errNone)
			return palmutil_setErr(palmdm_error, err);
	
	if ((err = MemPtrUnlock(rec->buf->b_ptr)) != 0)
		return palmutil_setErr(palmdm_error, err);

	PyBuffer_Invalidate(rec->buf);

	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject *
palmrec_getbuffer(palmrecobject *rec, PyObject *args)
{
	if (rec->buf == NULL) {
		Py_INCREF(Py_None);
		return Py_None;
	}
	Py_INCREF(rec->buf);
	return (PyObject *)rec->buf;
}


static PyMethodDef palmrec_methods[] = {
	{"info",	        (PyCFunction)palmrec_info, 0},
	{"release",		(PyCFunction)palmrec_release, METH_VARARGS},
	{"getbuffer",		(PyCFunction)palmrec_getbuffer, 0},
	{NULL,	       	NULL}		/* sentinel */
};


static PyMethodDef palmdb_methods[] = {
	{"listrecords",		(PyCFunction)palmdb_listrecords, METH_VARARGS},
	{"listcategories",	(PyCFunction)palmdb_listcategories, 0},
	{"info",	        (PyCFunction)palmdb_info, 0},
	{"queryrecord",         (PyCFunction)palmdb_queryrecord, METH_VARARGS},
	{"close",		(PyCFunction)palmdb_close, 0},
	{NULL,	       	NULL}		/* sentinel */
};

static PyObject *
palmrec_getattr(PyObject *dp, char *name)
{
	return Py_FindMethod(palmrec_methods, dp, name);
}

static PyObject *
palmdb_getattr(PyObject *dp, char *name)
{
	return Py_FindMethod(palmdb_methods, dp, name);
}

static PyTypeObject PalmDBType = {
	PyObject_HEAD_INIT(NULL)
	0,
	"palmdb",
	sizeof(palmdbobject),
	0,
	(destructor)palmdb_dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	(getattrfunc)palmdb_getattr, /*tp_getattr*/
	0,			/*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0			/*tp_as_sequence*/
};


static PyTypeObject PalmRecType = {
	PyObject_HEAD_INIT(NULL)
	0,
	"palmrec",
	sizeof(palmrecobject),
	0,
	(destructor)palmrec_dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	(getattrfunc)palmrec_getattr, /*tp_getattr*/
	0,			/*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0			/*tp_as_sequence*/
};

