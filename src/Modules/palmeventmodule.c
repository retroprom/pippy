/*
  palmevent module

  This module provides an interface to the PalmOS event objects
  and functions.

*/

#include "kludge.h"
#include "structmember.h"

#include "_palmutils.h"
#include "palmeventmodule.h"
#include "PalmCompatibility.h"
#include "set_a4.h"
#include "GLib_interface.h"
#include "io.h"

#define SHOW(flag, msg)
/* #define SHOW(flag, msg) ErrFatalDisplayIf(flag, msg) */

static PyObject *palmevent_error;

static PyObject *
newevent(EventType *event)
{
	/* are we going to copy the event structure into some
	   other structure? */
	palmeventobject *e;

/* 	SHOW(1,"newevent 1"); */

	if (!( e = PyObject_NEW(palmeventobject, &PalmEvent_Type) ))
		return NULL;

/* 	SHOW(1,"newevent 2"); */
	e->event = event;
/* 	SHOW(1,"newevent 3"); */

	return (PyObject *) e;
}

PyObject *
Palm_WrapNewEvent(EventType *e)
{
	return newevent(e);
}


static void
eventobj_dealloc(palmeventobject *e)
{
	free(e->event);
	e->event = NULL;
	PyMem_DEL(e);
}


#define OFF(x) offsetof(EventType, x)

static struct memberlist eventobj_memberlist[] = {
	{"eType",	T_INT,	        OFF(eType)},
	{"penDown",	T_UBYTE,	OFF(penDown)},
	{"tapCount",	T_UBYTE,	OFF(tapCount)},
	{"screenX",	T_INT,	        OFF(screenX)},
	{"screenY",	T_INT,	        OFF(screenY)},
	{NULL}	/* Sentinel */
};

static PyObject *
eventobj_getattr(palmeventobject *e, PyObject *name)
{
	char *sname = PyString_AsString(name);
	int id = -1;

	if (strcmp(sname, "data") == 0) {
		/* for now, just return the ID of the object in the
		   data structure */
		switch(e->event->eType) {
		case frmLoadEvent:
			id = e->event->data.frmLoad.formID;
			break;
		case frmOpenEvent:
			id = e->event->data.frmOpen.formID;
			break;
		case frmGotoEvent:
			id = e->event->data.frmGoto.formID;
			break;
		case frmCloseEvent:
			id = e->event->data.frmClose.formID;
			break;
		case frmUpdateEvent:
			id = e->event->data.frmUpdate.formID;
			break;
		case frmTitleEnterEvent:
			id = e->event->data.frmTitleEnter.formID;
			break;
		case frmTitleSelectEvent:
			id = e->event->data.frmTitleSelect.formID;
			break;
		case menuEvent:
			id = e->event->data.menu.itemID;
			break;
		case ctlEnterEvent:
			id = e->event->data.ctlEnter.controlID;
			break;
		case ctlSelectEvent:
			id = e->event->data.ctlSelect.controlID;
			break;
		case ctlRepeatEvent:
			id = e->event->data.ctlRepeat.controlID;
			break;
		case fldEnterEvent:
			id = e->event->data.fldEnter.fieldID;
			break;
		case fldHeightChangedEvent:
			id = e->event->data.fldHeightChanged.fieldID;
			break;
		case fldChangedEvent:
			id = e->event->data.fldChanged.fieldID;
			break;
		case lstEnterEvent:
			id = e->event->data.lstEnter.listID;
			break;
		case lstExitEvent:
			id = e->event->data.lstExit.listID;
			break;
		case lstSelectEvent:
			id = e->event->data.lstSelect.listID;
			break;
		default:
			PyErr_SetString(palmevent_error, "Attribute is not implemented");
			return NULL;
			break;
		}
		return Py_BuildValue("i", id);
	}

	return PyMember_Get((char *)e->event, eventobj_memberlist, sname);
}


PyTypeObject PalmEvent_Type = {
	PyObject_HEAD_INIT(NULL)
	0,
	"palmeventobject",
	sizeof(palmeventobject),
	0,
	(destructor)eventobj_dealloc, /*tp_dealloc*/
	0,			/*tp_print*/
	0,                      /*tp_getattr*/
	0,			/*tp_setattr*/
	0,			/*tp_compare*/
	0,			/*tp_repr*/
	0,			/*tp_as_number*/
	0,			/*tp_as_sequence*/
	0,			/*tp_as_mapping*/
	0,                      /*tp_hash*/
	0,			/*tp_call*/
	0,			/*tp_str*/
	(getattrofunc)eventobj_getattr, /*tp_getattro*/
	0,			/*tp_setattro*/
};









static PyObject *
palmevent_Err(UInt16 err)
{
	PyObject *v;

	v = palmutil_buildErrObject(errno);
	if (v != NULL) {
		PyErr_SetObject(palmevent_error, v);
		Py_DECREF(v);
	}

	return NULL;
}

static PyObject *
palmevent_ErrFromErrno()
{
	return palmevent_Err(errno);
}





static PyObject *
palmevent_getevent(PyObject *self, PyObject *args)
{
	Int32 timeout = -1;  /* wait indefinitely */
	EventType *event;

/* 	SHOW(1,"getevent 1"); */


/* 	if (!PyArg_ParseTuple(args, "|l:getevent", &timeout)) { */
/* 		PyErr_SetString(palmevent_error, "Must supply a timeout"); */
/* 		return NULL; */
/* 	} */



/* 	SHOW(1,"getevent 2"); */

	if ( (event = (EventType *) malloc(sizeof(EventType)) ) == NULL)
		return NULL;

/* 	SHOW(1,"getevent 3"); */
	EvtGetEvent (event, -1);

	return newevent(event);

}

static PyObject *
palmevent_eventavail(PyObject *self, PyObject *args)
{
	Boolean avail;

	if (!PyArg_ParseTuple(args, ""))
		return NULL;

	avail = EvtEventAvail();

	return Py_BuildValue("i", (avail ? 1 : 0));
}


static PyObject *
palmevent_syseventavail(PyObject *self, PyObject *args)
{
	Boolean avail;
	int ignore = 0;

	if (!PyArg_ParseTuple(args, "|i:syseventavail", &ignore))
		return NULL;

	avail = EvtSysEventAvail((ignore ? true : false));

	return Py_BuildValue("i", (avail ? 1 : 0));
}

static PyObject*
palmevent_syshandleevent(PyObject *self, PyObject *args)
{
	Boolean result;
	PyObject *eventWrapper;
	EventType *event;

	if (!PyArg_ParseTuple(args, "O", &eventWrapper))
		return NULL;
	if (!PalmEventObject_Check(eventWrapper)) {
		PyErr_SetString(PyExc_ValueError, "Expecting a PalmEventObject");
		return NULL;
	}

	event = (EventType *) ((palmeventobject *)eventWrapper)->event;
	result = SysHandleEvent(event);

	return Py_BuildValue("i", (result ? 1 : 0));
}

Boolean (*genericHandler)(EventType *e) = NULL;

void
registerGenericEventHandler( Boolean (*func)(EventType *e) )
{
	genericHandler = func;
}
	

static PyObject*
palmevent_genericHandleEvent(PyObject *self, PyObject *args)
{
	/* This function returns a call to a generic event handler
	   (written in C).  The argument is a palmeventobject instance. */
	EventType *e;
	PyObject *eventWrapper, *result;
	Boolean res = false;

	SET_A4_FOR_GLIB('PyLb');
	
	SHOW(1, "genericHandleEvent 1\n");
	if (genericHandler == NULL)
		goto done;
	SHOW(1, "genericHandleEvent 2\n");
	if (!PyArg_ParseTuple(args, "O", &eventWrapper))
		goto done;
	SHOW(1, "genericHandleEvent 3\n");
	if (!PalmEventObject_Check(eventWrapper)) {
		PyErr_SetString(PyExc_ValueError, "Expecting a PalmEventObject");
		goto done;
	}
	SHOW(1, "genericHandleEvent 4\n");

	e = (EventType *) ((palmeventobject *)eventWrapper)->event;
	SHOW(1, "genericHandleEvent 5\n");

	res = genericHandler(e);
  done: 
	
	result = Py_BuildValue("i", (res ? 1 : 0));
	RESTORE_A4;
	return result;
}

static PyObject *
palmevent_ioHandleEvent(PyObject *self, PyObject *args)
{
	/* This is a test code for directly calling the 
	   ioHandleEvent routine. */

	EventType *e;
	PyObject *eventWrapper, *result;
	Boolean res = false;

	if (!PyArg_ParseTuple(args, "O", &eventWrapper))
		goto done;
	if (!PalmEventObject_Check(eventWrapper)) {
		PyErr_SetString(PyExc_ValueError, "Expecting a PalmEventObject");
		goto done;
	}

	e = (EventType *) ((palmeventobject *)eventWrapper)->event;

/* 	{ */
/* 		char buf[100]; */
		
/* 		MemSet(buf, 100, '\0'); */
/* 		sprintf(buf, "palmevent: %d", e->eType); */
/* 		ErrAlertCustom(0,buf,0,0); */
/* 	} */

	res = ioHandleEvent(e);
  done:
	
	result = Py_BuildValue("i", (res ? 1 : 0));
	return result;
}




static PyObject*
palmevent_menuhandleevent(PyObject *self, PyObject *args)
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
	result = MenuHandleEvent(NULL, event, &err);

	return Py_BuildValue("i", (result ? 1 : 0));
}



static PyMethodDef palmevent_methods[] = {
	{"getevent",               (PyCFunction)palmevent_getevent, METH_VARARGS},
	{"syseventavail",          (PyCFunction)palmevent_syseventavail, METH_VARARGS},
	{"eventavail",             (PyCFunction)palmevent_eventavail, METH_VARARGS},
	{"syshandleevent",         (PyCFunction)palmevent_syshandleevent, METH_VARARGS},
	{"menuhandleevent",        (PyCFunction)palmevent_menuhandleevent, METH_VARARGS},
	{"genericHandleEvent",     (PyCFunction)palmevent_genericHandleEvent, METH_VARARGS},
	{"ioHandleEvent",         (PyCFunction)palmevent_ioHandleEvent, METH_VARARGS},
	{NULL, NULL} };


void initpalmevent(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmevent", palmevent_methods, NULL);
	d = PyModule_GetDict(m);
	palmevent_error = PyErr_NewException("palmevent.error", NULL, NULL);
	if (palmevent_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmevent_error);

	if (PyErr_Occurred)
		PyErr_Clear();

}
