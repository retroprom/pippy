
/*
  This module contains a collection of routines for using the PalmOS
  datamanager for storage of read-only objects.  Its purpose is to eliminate
  multiply referenced objects from the dynamic heap and store them in a
  database instead.  It is assumed that the Python object is first created in
  its entirety on the dynamic heap, then transferred to the datamanager heap
  after.  Then the original object is reclaimed, being replaced by a reference
  to the that in the datamanager.  Candidates for this type of storage
  are immutable ones, such as interned strings and code objects.  

  Implementation:
  
  ob_refcnt = MAX_INT

  This id is checked for in Py_INCREF and Py_DECREF and, if true, the
  reference count is not changed.

  Typical usage:

  1.  call dbmem_init before starting python
  2a. call dbmem_addentry() to write the entry
  2b. may be some searching capabilities, particularly for debugging.
  3.  call dbmem_fini after Py_Finalize() has completed

  We start with just one scratch database for storage of all objects.  Others
  may be opened to accomodate other object types.


  Error return codes for dbmem_* functions (Python convention)
      For functions returning int:
	  -1 = failure
          0  = success
      For functions returning pointer:
          NULL  = failure
	  other = success

*/


#include "Python.h"
#include <PalmOS.h>
#include <stdio.h>
#include "compile.h"
#include "dbmem.h"

#define	bcopy(b1,b2,len)MemMove(b2,b1,len)


static DmOpenRef stringdb_ref = NULL;
static LocalID stringdb_localid = 0;
static int cardNo = 0;
static char name[] = "PyLb-InternedStrings"; /* must be unique */
static char s_creator[] = "PyLb";
static char s_type[] = "dATA";
static long creator;
static long type;


int dbmem_init()
{

	Err err;
	LocalID dbid;
	DmOpenRef dbref;

	if (stringdb_ref != NULL) /* already opened */
		return -1;      /* fail - we only want it opened once */

	bcopy(s_creator, &creator, 4);
	bcopy(s_type, &type, 4);

	err = DmCreateDatabase (cardNo, name, creator, type, false);

	switch (err) {
	case errNone:
		break;
	case dmErrAlreadyExists:
	case memErrNotEnoughSpace:
	case dmErrInvalidDatabaseName:
	case memErrCardNotPresent:
	case dmErrMemError:
	case memErrChunkLocked:
	case memErrInvalidParam:
	case memErrInvalidStoreHeader:
	case memErrRAMOnlyCard:
	default:
		return -1;
	}


	dbid = DmFindDatabase (cardNo, name);
	if (dbid == 0) {
		err = DmGetLastErr();
		return -1;

	}
	stringdb_localid = dbid;

	dbref = DmOpenDatabase (cardNo, dbid, dmModeReadWrite);
	if (dbref == 0) {
		err = DmGetLastErr();
		return -1;
	}

	stringdb_ref = dbref;
	
	return 0;
}


void *
dbmem_addentry(void *p, long size)
{
	/* Add an entry to the database */

	MemHandle recH;
	void *recP;
	Err err;
	UInt16 index;

	if (stringdb_ref == NULL)
		return NULL;

	index = dmMaxRecordIndex; /* append the record to the database */

	recH = DmNewRecord (stringdb_ref, &index, size);
	
	if (recH == 0) {
		err = DmGetLastErr();
		return NULL;
	}

	recP = MemHandleLock(recH);
	/* check for errors */

	err = DmWrite (recP, 0, p, size);

	if (err != errNone)
		return NULL;
		/* problem occurred */

	/* otherwise, return the pointer to the record */
	return recP;
}

int 
dbmem_size(void *s)
{
	return MemPtrSize(s);
}


int dbmem_fini() 
{
	/* loop over all records, unlock memory and release the record */
	int i;
	MemHandle recH;
	Err err;
	int num_records;
	int index;

	if (stringdb_ref == NULL) /* database not opened */
		return -1; 

	num_records = DmNumRecords(stringdb_ref);
	/* test for records */
	for (i=0; i < num_records;i++) {
		recH = DmQueryRecord (stringdb_ref, i);
	}
	
	/* release and remove records */
	/* faster to not remove records before deleting database */
	for (i=0; i < num_records;i++) {
		index = i; /* set to zero if DmRemoveRecord is called below */
		recH = DmQueryRecord (stringdb_ref, index);
		err =  MemHandleUnlock(recH);
		err =  DmReleaseRecord (stringdb_ref, index, false);
/* 		err =  DmRemoveRecord (stringdb_ref, index); */
	}

	/* close and delete the database */
	err = DmCloseDatabase(stringdb_ref);
	stringdb_ref = NULL;
	
	err = DmDeleteDatabase(cardNo, stringdb_localid);
	return 0;
}


PyObject *
dbmem_addPyObject(PyObject *s) {
	int saved_refcnt = s->ob_refcnt;
	PyObject *res;
	
	s->ob_refcnt = PALMDM_REFCNT;
	res = (PyObject *) dbmem_addentry((void *)s, MemPtrSize(s));
	s->ob_refcnt = saved_refcnt;

	return res;

}

PyObject *
dbmem_addPyCodeObject(PyCodeObject *v) 
{

	PyObject *res = (PyObject *)v;

	if (v == NULL) /* return the original object unchanged */
		return res;

	/* incref for ownership by res */
	Py_XINCREF(v->co_code);

	v->co_consts = dbmem_addPyTupleObject(v->co_consts);
	Py_XINCREF(v->co_consts);

	v->co_names = dbmem_addPyTupleObject(v->co_names);
	Py_XINCREF(v->co_names);

	v->co_varnames = dbmem_addPyTupleObject(v->co_varnames);
	Py_XINCREF(v->co_varnames);

	Py_XINCREF(v->co_filename);
	Py_XINCREF(v->co_name);
	Py_XINCREF(v->co_lnotab); /* force an intern on this */

	if ( (res = dbmem_addPyObject((PyObject *)v)) != NULL) {
		
		Py_DECREF(v);
		{
			char buf[30];
			sprintf(buf, "('marshalled', %d)", dbmem_size(res));
			DMESSAGE(buf);
		}
		
	}

	return res;
}


PyObject *
dbmem_addPyTupleObject(PyTupleObject *v) 
{

	PyObject *res = (PyObject *)v;
	int i, n;
	PyObject *item;

	if (!PyTuple_Check(v)) /* should raise an error here */
		return res;

	if (v == NULL) /* return the original object unchanged */
		return res;

	if ( (res = dbmem_addPyObject((PyObject *)v)) != NULL) {
		DMESSAGE("Setting tuple");

		/* incref for ownership by res */
		n = PyTuple_GET_SIZE(v);
		for (i = 0; i < n; i++) {
			item = PyTuple_GET_ITEM(v, i);
			Py_XINCREF(item);
		}

		Py_DECREF(v);
		{
			char buf[30];
			sprintf(buf, "('tupled', %d)", dbmem_size(res));
			DMESSAGE(buf);
		}
		
	}

	return res;
}

