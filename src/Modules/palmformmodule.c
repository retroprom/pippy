/*
  palmform module

  This module provides an interface to the PalmOS form objects
  and functions.

  METHODS

  draw()               # FrmDrawForm
  handleEvent()        # FrmHandleEvent
  setEventHandler()    # FrmSetEventHandler - would actually register globally so
                       # that FrmDispatchEvent will work properly
  setActive()          # FrmSetActive
  setFocus (fieldIndex)# FrmSetFocus


  MODULE FUNCTIONS

  getActiveForm()      # FrmGetActiveForm
  initForm(formId)     # FrmInitForm
  goto()               # FrmGotoForm


*/
#include "kludge.h"
#include "structmember.h"
#include "_palmutils.h"
#include "PalmCompatibility.h"
#include "set_a4.h"
#include "GLib_interface.h"
#include "palmeventmodule.h"

#define SHOW(flag, msg)
/* #define SHOW(flag, msg) ErrFatalDisplayIf(flag, msg) */

static PyObject *palmform_error;

#define PalmFormObject_Check(op) ((op)->ob_type == &PalmForm_Type)

/* Palm Form Object */
typedef struct {
	PyObject_HEAD

/* Palm Database Object */
	FormType *form;
} palmformobject;

static PyTypeObject PalmForm_Type;

static PyObject *
newform(FormType *form)
{
	/* NOTE: For the time being, form objects will not take possession of
	   the actual form - they simply behave as FormType containers.  I
	   think that this is only a problem if the FrmDeleteForm function
	   gets implemented.  Then, ownership becomes more important.
	*/

	palmformobject *frm;

	if (!( frm = PyObject_NEW(palmformobject, &PalmForm_Type) ))
		return NULL;

	frm->form = form;

	return (PyObject *) frm;
}

static void
formobj_dealloc(palmformobject *frm)
{
/* 	free(frm->form); */
	frm->form = NULL;
	PyMem_DEL(frm);
}

static PyObject*
formobj_draw(palmformobject *frm, PyObject *args)
{
		
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	
	FrmDrawForm (frm->form);

	Py_INCREF(Py_None);
	return Py_None;
}	
static PyObject*
formobj_handleEvent(palmformobject *frm, PyObject *args)
{
	EventType *event;
	PyObject *eventWrapper;
	Boolean res;

	if (!PyArg_ParseTuple(args, "O", &eventWrapper))
		return NULL;
	if (!PalmEventObject_Check(eventWrapper)) {
		PyErr_SetString(PyExc_ValueError, "Expecting a PalmEventObject");
		return NULL;
	}

	event = (EventType *) ((palmeventobject *)eventWrapper)->event;
	res = FrmHandleEvent (frm->form, event);

	return Py_BuildValue("i", (res ? 1 : 0));
}	


/* Handlers are stored using linked-list that behaves like a stack */

typedef struct _handler_item {
	UInt16 formID;
	PyObject *callback;
	struct _handler_item *next;
} handler_item;

handler_item *handlers = NULL;

static int
_addHandler(UInt16 formId, PyObject *func)
{
	handler_item *h;
	int status = -1;

	/* check for a handler with the same formId and replace */
	h = handlers;
	while (h != NULL) {
		if (h->formID == formId)
			break;
		h = h->next;
	}
	if (h == NULL) {
		h = (handler_item*) malloc(sizeof(handler_item));
		if (!h)
			return status;

		/* insert new item at the front of the list */
		if (handlers == NULL) {
			handlers = h;
			handlers->next = NULL;
			status = 0;
		}
		else {
			h->next = handlers;
			handlers = h;
			status = 0;
		}
	}
	else {
		Py_DECREF(h->callback);
		status = 0;
	}

	h->formID = formId;
	h->callback = func;
	Py_INCREF(h->callback);

	return status;
}

static Boolean
_genericEventHandler(EventType *evt)
{
	handler_item *h;
	UInt16 targetID;
	PyObject *res, *event;

	SET_A4_FOR_GLIB('PyLb');
	
	h = handlers;
/* 	targetID = evt->data.frmLoad.formID; */
	SHOW(1, "Getting formID");
	targetID = FrmGetActiveFormID();
	SHOW(1, "Got formID");
	while (h != NULL) {
		/* all forms have the formID as their first item, 
		   so we'll just use frmLoad to get it */
		if (h->formID == targetID)
			break;
		printf("h->formID, targetID = %d %d\n", h->formID,targetID);
		h = h->next;
	}

	if (h == NULL) {
		RESTORE_A4;
		return false;  /* unable to find handler - really should abort */
	}

	if ((event = Palm_WrapNewEvent(evt)) == NULL) {
		PyErr_SetString(palmform_error,
				"Problem wrapping event - memory error?");
		RESTORE_A4;
		return false;
	}
	SHOW(1, "Calling function");
	if (h->callback == NULL)
		SHOW(1, "Callback function is null");
	res = PyEval_CallObject(h->callback, Py_BuildValue("(O)", event));
	SHOW(1, "Done calling function");
	
	/* When the Python event loop is finally working, the following
	   PyErr_* calls should be eliminated and the checks for errors
	   placed in the event loop itself. */
	if (res == NULL) {
		         SHOW(1, "Got an error calling function");
		PyErr_Print();
		PyErr_Clear();
		RESTORE_A4;
		return false;
	}
	RESTORE_A4;
	return true;

}


static PyObject*
formobj_setEventHandler(palmformobject *frm, PyObject *args)
{
	PyObject *func;

	if (!PyArg_ParseTuple(args, "O", &func))
		return NULL;
	if (!PyCallable_Check(func)) {
		PyErr_SetString(PyExc_ValueError, "Object must be callable");
		return NULL;
	}
	if ( _addHandler( FrmGetFormId(frm->form), func ) == -1) {
		PyErr_SetString(PyExc_MemoryError, "Out of memory");
		return NULL;
	}
	FrmSetEventHandler (frm->form, _genericEventHandler);
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject*
formobj_setActive(palmformobject *frm, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	FrmSetActiveForm (frm->form);

	Py_INCREF(Py_None);
	return Py_None;
}	
static PyObject*
formobj_setFocus(palmformobject *frm, PyObject *args)
{
	Py_INCREF(Py_None);
	return Py_None;
}	



/* form object methods */
static PyMethodDef formobj_methods[] = {
	{"draw",		(PyCFunction)formobj_draw, METH_VARARGS},
	{"handleEvent",		(PyCFunction)formobj_handleEvent, METH_VARARGS},
	{"setEventHandler",	(PyCFunction)formobj_setEventHandler, METH_VARARGS},
	{"setActive",		(PyCFunction)formobj_setActive, METH_VARARGS},
	{"setFocus",		(PyCFunction)formobj_setFocus, METH_VARARGS},
	{NULL,	       	NULL}		/* sentinel */
};

/* form object attributes read directly from the form structure */
#define OFF(x) offsetof(FormType, x)
static struct memberlist formobj_memberlist[] = {
	{"formId",	T_INT,	        OFF(formId)},
	{"numObjects",	T_INT,	        OFF(numObjects)},
	{NULL}	/* Sentinel */
};

/* the following was taken from structmember.c and modified */
static PyObject *
listmethods(mlist)
	PyMethodDef *mlist;
{
	int i, n;
	PyObject *v;
	for (n = 0; mlist[n].ml_name != NULL; n++)
		;
	v = PyList_New(n);
	if (v != NULL) {
		for (i = 0; i < n; i++)
			PyList_SetItem(v, i,
				       PyString_FromString(mlist[i].ml_name));
		if (PyErr_Occurred()) {
			Py_DECREF(v);
			v = NULL;
		}
		else {
			PyList_Sort(v);
		}
	}
	return v;
}

static PyObject *
formobj_getattr(palmformobject *frm, PyObject *name)
{
	char *sname = PyString_AsString(name);
	PyObject *res;

	if (strcmp(sname, "__members__") == 0)
		return listmethods(formobj_methods);

	res = PyMember_Get((char*)frm->form, formobj_memberlist, sname);
	if (res == NULL)
		res = Py_FindMethod(formobj_methods, (PyObject *) frm, sname);

	return res;
}

static PyTypeObject PalmForm_Type = {
	PyObject_HEAD_INIT(NULL)
	0,
	"palmformobject",
	sizeof(palmformobject),
	0,
	(destructor)formobj_dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	0,                      /*tp_getattr*/
	0,                      /*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0,			/*tp_as_sequence*/
	0,			/*tp_as_mapping*/
	0,                      /*tp_hash*/
	0,			/*tp_call*/
	0,			/*tp_str*/
	(getattrofunc)formobj_getattr, /*tp_setattro*/
	0,			/*tp_setattro*/
};

/* Module functions */
static PyObject *
palmform_getActiveForm(PyObject *self, PyObject *args)
{
	FormType *form;

	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	
	form = FrmGetActiveForm();
	if (form == NULL) 
		return NULL;

	return newform(form);
}
static PyObject *
palmform_setActiveForm(PyObject *self, PyObject *args)
{
	palmformobject *frm;

	if (!PyArg_ParseTuple(args, "O:setActiveForm", &frm))
		return NULL;

	FrmSetActiveForm (frm->form);

	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject *
palmform_initForm(PyObject *self, PyObject *args)
{
	int resId;
	FormType *form;

	if (!PyArg_ParseTuple(args, "i:initForm", &resId))
		return NULL;

	form = FrmInitForm(resId);
	if (form == NULL)
		return NULL;

	return newform(form);
}
static PyObject *
palmform_gotoForm(PyObject *self, PyObject *args)
{
	int formId;
	if (!PyArg_ParseTuple(args, "i:form", &formId))
		return NULL;
	
	FrmGotoForm (formId);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
palmform_alert(PyObject *self, PyObject *args)
{
	int alertId, res;
	if (!PyArg_ParseTuple(args, "i:alert", &alertId))
		return NULL;
	
	res = FrmAlert(alertId);
	return PyInt_FromLong( (long) res );
}

static PyObject *
palmform_closeAllForms(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	
	FrmCloseAllForms();
	Py_INCREF(Py_None);
	return Py_None;
}
static PyObject *
palmform_saveAllForms(PyObject *self, PyObject *args)
{
	if (!PyArg_ParseTuple(args, ""))
		return NULL;
	
	FrmSaveAllForms();
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject*
palmform_dispatchEvent(PyObject *self, PyObject *args)
{
	Boolean result;
	PyObject *eventWrapper;
	EventType *event;
	Err err;

	if (!PyArg_ParseTuple(args, "O", &eventWrapper))
		return NULL;
	if (!PalmEventObject_Check(eventWrapper)) {
		PyErr_SetString(PyExc_ValueError, "Expecting a PalmEventObject");
		return NULL;
	}

	event = (EventType *) ((palmeventobject *)eventWrapper)->event;
	result = FrmDispatchEvent(event);

	return Py_BuildValue("i", (result ? 1 : 0));
}


static PyMethodDef palmform_methods[] = {
	{"getActiveForm",	(PyCFunction)palmform_getActiveForm, METH_VARARGS},
	{"setActiveForm",	(PyCFunction)palmform_setActiveForm, METH_VARARGS},
	{"initForm",	        (PyCFunction)palmform_initForm, METH_VARARGS},
	{"alert",	        (PyCFunction)palmform_alert, METH_VARARGS},
	{"gotoForm",	        (PyCFunction)palmform_gotoForm, METH_VARARGS},
	{"closeAllForms",	(PyCFunction)palmform_closeAllForms, METH_VARARGS},
	{"saveAllForms",	(PyCFunction)palmform_saveAllForms, METH_VARARGS},
	{"dispatchEvent",	(PyCFunction)palmform_dispatchEvent, METH_VARARGS},
	{NULL,	       	NULL}		/* sentinel */
};

static PyObject *palmeventmodule;

static void
_cleanup(void)
{
	handler_item *h = handlers, *next;
	
	/* clean up the event handlers */
	if (h != NULL)
		while (h->next) {
			next = h->next;
			Py_DECREF(h->callback);
			free(h);
			h = next;
		}

	/* decref palmevent module - no longer needed */
	Py_DECREF(palmeventmodule);
}


void initpalmform(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmform", palmform_methods, NULL);
	d = PyModule_GetDict(m);
	palmform_error = PyErr_NewException("palmform.error", NULL, NULL);
	if (palmform_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmform_error);

	/* Import the palmevent module so that events may be 
	   used. */
	palmeventmodule = PyImport_ImportModule("palmevent");

	if (PyErr_Occurred)
		PyErr_Clear();

	Py_AtExit(_cleanup);

}
