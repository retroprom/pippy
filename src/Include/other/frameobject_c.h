#ifndef FRAMEOBJECT_C_H
#define FRAMEOBJECT_C_H

#include "proto_defs.h"

DESTRUCTOR_PROTO(frame_dealloc, PyFrameObject) SEG_FRAMEOBJECT_C;
GETATTRFUNC_PROTO(frame_getattr, PyFrameObject) SEG_FRAMEOBJECT_C;
SETATTRFUNC_PROTO(frame_setattr, PyFrameObject) SEG_FRAMEOBJECT_C;


#endif /* FRAMEOBJECT_C_H */
