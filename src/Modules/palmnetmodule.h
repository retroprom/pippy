#ifndef _PALMNETMODULE_H_
#define _PALMNETMODULE_H_

#include "other/segment_defs.h"

PyObject *PalmNet_InitNetwork() SEG_PALMNETMODULE_C;
int PalmNet_EnsureOpen() SEG_PALMNETMODULE_C;
int PalmNet_IsOpen(void) SEG_PALMNETMODULE_C;
void initpalmnet(void) SEG_PALMNETMODULE_C;

#endif /* _PALMNETMODULE_H_ */
