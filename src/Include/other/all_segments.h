#ifndef PALM_ALL_SEGMENTS_H
#define PALM_ALL_SEGMENTS_H

/*
  Note:  All exported GLib library functions must appear in the first
  segment (unless the dispatch table is in the data segment - ask 
  Sokolov to explain this).  In any case, we need Py_Initialize and
  Py_Finalize() to appear in the main segment.
*/

#include "segments.h"
#include "segment_defs.h"

#endif /* PALM_ALL_SEGMENTS_H */
