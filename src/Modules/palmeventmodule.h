#ifndef PALMEVENTMODULE_H
#define PALMEVENTMODULE_H

PyObject *Palm_WrapNewEvent(EventType *e);
void registerGenericEventHandler( Boolean (*func)(EventType *e) );
#define PalmEventObject_Check(op) ((op)->ob_type == &PalmEvent_Type)

/* Palm Event Object */
typedef struct {
	PyObject_HEAD

/* Palm Database Object */
	EventType *event;
} palmeventobject;

extern PyTypeObject PalmEvent_Type;

#endif /* PALMEVENTMODULE_H */
