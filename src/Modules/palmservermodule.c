/*  First cut at exporting the PalmOS Data Manager interface */


#include "kludge.h"
#include "_palmutils.h"

#include "palm_socketmodule.h"
#include "http_interface.h"


static PyObject *palmserver_error;

static PyObject *palmserver_getattr(PyObject *dp, char *name);


static PyObject *
palmserver_wait_request(PyObject *self, PyObject *args)
{

	PySocketSockObject *sock;
	PyObject *result;
	int newfd;

	if (!PyArg_ParseTuple(args, "O", &sock))
		return NULL;

	if (!PySocketSock_Check(sock)) {
		PyErr_SetString(PyExc_ValueError, "Argument must be a socket");
		return NULL;
	}

	result = (PyObject *) serve(sock->sock_fd, &newfd);
	if (result == NULL) {
		Py_INCREF(Py_None);
		Py_INCREF(Py_None);
		return Py_BuildValue("OO", Py_None, Py_None);
	}

	return Py_BuildValue("iO", newfd, (PyObject *)result);
}


static PyObject *
palmserver_send_response(PyObject *self, PyObject *args)
{
	int newfd;
	PyObject *response;
	
	if (!PyArg_ParseTuple(args, "iO", &newfd, &response))
		return NULL;
	
	if (!PyTuple_Check(response)){
		PyErr_SetString(PyExc_ValueError, "Second argument must be a tuple");
		return NULL;
	}

	send_response(newfd, response);

	Py_INCREF(Py_None);
	return Py_None;
}


static PyMethodDef palmserver_methods[] = {
	{"wait_request",  (PyCFunction)palmserver_wait_request, METH_VARARGS},
	{"send_response",  (PyCFunction)palmserver_send_response, METH_VARARGS},
	{NULL, NULL} };



void initpalmserver(void) {
	PyObject *m, *d;

	m = Py_InitModule3("palmserver", palmserver_methods, NULL);
	d = PyModule_GetDict(m);
	palmserver_error = PyErr_NewException("palmserver.error", NULL, NULL);
	if (palmserver_error == NULL)
		return;

	PyDict_SetItemString(d, "error", palmserver_error);

}
