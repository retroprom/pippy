#ifndef PALM_SEGMENT_DEFS_H
#define PALM_SEGMENT_DEFS_H

/*
  Note:  All exported GLib library functions must appear in the first
  segment (unless the dispatch table is in the data segment - ask 
  Sokolov to explain this).  In any case, we need Py_Initialize and
  Py_Finalize() to appear in the main segment.
*/

#include "segments.h"

/* main segment - pilot-file index 1 */
#define SEG_MAIN

/* segment 0    - pilot-file index 2 */
#define SEG_PYTHONRUN_H DEF_SEGMENT(PALM_SEG_0)
#define SEG_PYTHONRUN_C DEF_SEGMENT(PALM_SEG_0)
/* the following moved Thu Mar  1 21:20:17 MST 2001 */
#define SEG_FILEOBJECT_H DEF_SEGMENT(PALM_SEG_0) 
#define SEG_LONGOBJECT_H DEF_SEGMENT(PALM_SEG_0)

#define SEG_DBMEM_H DEF_SEGMENT(PALM_SEG_0)

/* segment 1    - pilot-file index 3 */
#define SEG_ABSTRACT_H DEF_SEGMENT(PALM_SEG_1)
#define SEG_PYTHON_H   DEF_SEGMENT(PALM_SEG_1)
#define SEG_BITSET_H   DEF_SEGMENT(PALM_SEG_1)
#define SEG_BUFFEROBJECT_H DEF_SEGMENT(PALM_SEG_1)

/* segment 2    - pilot-file index 4 */
#define SEG_CLASSOBJECT_H DEF_SEGMENT(PALM_SEG_2)
#define SEG_COBJECT_H DEF_SEGMENT(PALM_SEG_2)
#define SEG_COMPLEXOBJECT_H DEF_SEGMENT(PALM_SEG_2)
#define SEG_DICTOBJECT_H DEF_SEGMENT(PALM_SEG_2)
#define SEG_MODSUPPORT_H DEF_SEGMENT(PALM_SEG_2)


/* segment 2A   - pilot-file index 5 */
#define SEG_COMPILE_H DEF_SEGMENT(PALM_SEG_2A)
/* segment 2B   - pilot-file index 6 */
#define SEG_CEVAL_H DEF_SEGMENT(PALM_SEG_2B)


/* segment 3    - pilot-file index 7 */
#define SEG_ERRCODE_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_EVAL_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_FLOATOBJECT_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_FRAMEOBJECT_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_FUNCOBJECT_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_GETARGS_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_GRAMINIT_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_IMPORT_H DEF_SEGMENT(PALM_SEG_3)

/* begin:  the following moved Fri Jul  7 16:01:07 PDT 2000 */
#define SEG_MYMALLOC_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_MYMATH_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_MYPROTO_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_MYSELECT_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_MYTIME_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_OSDEFS_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_INTRCHECK_H DEF_SEGMENT(PALM_SEG_3)
#define SEG_LONGINTREPR_H DEF_SEGMENT(PALM_SEG_3)
/* end:  moved Fri Jul  7 16:01:07 PDT 2000 */

/* segment 4    - pilot-file index 8 */
#define SEG_INTOBJECT_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_LISTOBJECT_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_MARSHAL_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_METHODOBJECT_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_MODULEOBJECT_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_OBJECT_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_OBJIMPL_H DEF_SEGMENT(PALM_SEG_4)
#define SEG_OPCODE_H DEF_SEGMENT(PALM_SEG_4)

/* segment 6    - pilot-file index 9 */
#define SEG_PATCHLEVEL_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PGENHEADERS_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PYDEBUG_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PYERRORS_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PYFPE_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PYSTATE_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_PYTHREAD_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_RANGEOBJECT_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_RENAME2_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_SLICEOBJECT_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_STRINGOBJECT_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_STRUCTMEMBER_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_THREAD_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_TRACEBACK_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_TUPLEOBJECT_H DEF_SEGMENT(PALM_SEG_6)
#define SEG_TYPEOBJECT_H DEF_SEGMENT(PALM_SEG_6)

/* segment 8   - pilot-file index 10 */
#define SEG_BLTINMODULE_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_SYSMODULE_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_ACCELER_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_GRAMMAR_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_METAGRAMMAR_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_NODE_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_PARSETOK_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_LISTNODE_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_PARSER_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_PGEN_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_TOKEN_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_TOKENIZER_H DEF_SEGMENT(PALM_SEG_8)
#define SEG_MYREADLINE_H DEF_SEGMENT(PALM_SEG_8)

/* extension module segment - segment 9  - pilot-file index 11 */
#define SEG_ERRNOMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_TIMEMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_PALMNETMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_SELECTMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_SOCKETMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_APPSMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_PALMSYSMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_DMMODULE_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_CONFIG_C DEF_SEGMENT(PALM_SEG_9)
#define SEG_CSTRINGIO_H DEF_SEGMENT(PALM_SEG_9)
#define SEG_PALMEMUMODULE_C DEF_SEGMENT(PALM_SEG_9)

/* extension module segment - segment 10  - pilot-file index 12 */
#define SEG_CPICKLE_C DEF_SEGMENT(PALM_SEG_10)
#define SEG_PALMWINMODULE_C DEF_SEGMENT(PALM_SEG_10)

/* not shown: pseglibc   - pilot-file index 13 */


#define SEG_ABSTRACT_C SEG_ABSTRACT_H
#define SEG_PYTHON_C SEG_PYTHON_H
#define SEG_PYTHON_C SEG_PYTHON_H
#define SEG_BITSET_C SEG_BITSET_H
#define SEG_BUFFEROBJECT_C SEG_BUFFEROBJECT_H

#define SEG_CSTRINGIO_C SEG_CSTRINGIO_H 
#define SEG_CEVAL_C SEG_CEVAL_H 
#define SEG_CLASSOBJECT_C SEG_CLASSOBJECT_H 
#define SEG_COBJECT_C SEG_COBJECT_H 
#define SEG_COMPILE_C SEG_COMPILE_H 
#define SEG_COMPLEXOBJECT_C SEG_COMPLEXOBJECT_H 
#define SEG_DICTOBJECT_C SEG_DICTOBJECT_H 

#define SEG_ERRCODE_C SEG_ERRCODE_H 
#define SEG_EVAL_C SEG_EVAL_H 
#define SEG_FILEOBJECT_C SEG_FILEOBJECT_H 
#define SEG_FLOATOBJECT_C SEG_FLOATOBJECT_H 
#define SEG_FRAMEOBJECT_C SEG_FRAMEOBJECT_H 
#define SEG_FUNCOBJECT_C SEG_FUNCOBJECT_H 
#define SEG_GETARGS_C SEG_GETARGS_H
#define SEG_GRAMINIT_C SEG_GRAMINIT_H 
#define SEG_IMPORT_C SEG_IMPORT_H 

#define SEG_INTOBJECT_C SEG_INTOBJECT_H 
#define SEG_INTRCHECK_C SEG_INTRCHECK_H 
#define SEG_LISTOBJECT_C SEG_LISTOBJECT_H 
#define SEG_LONGOBJECT_C SEG_LONGOBJECT_H 
#define SEG_LONGINTREPR_C SEG_LONGINTREPR_H 
#define SEG_MARSHAL_C SEG_MARSHAL_H 
#define SEG_METHODOBJECT_C SEG_METHODOBJECT_H
#define SEG_MODSUPPORT_C SEG_MODSUPPORT_H 

#define SEG_MODULEOBJECT_C SEG_MODULEOBJECT_H
#define SEG_MYMALLOC_C SEG_MYMALLOC_H 
#define SEG_MYMATH_C SEG_MYMATH_H 
#define SEG_MYPROTO_C SEG_MYPROTO_H 
#define SEG_MYSELECT_C SEG_MYSELECT_H 
#define SEG_MYTIME_C SEG_MYTIME_H 
#define SEG_OBJECT_C SEG_OBJECT_H 
#define SEG_OBJIMPL_C SEG_OBJIMPL_H 
#define SEG_OPCODE_C SEG_OPCODE_H 
#define SEG_OSDEFS_C SEG_OSDEFS_H 

#define SEG_PATCHLEVEL_C SEG_PATCHLEVEL_H
#define SEG_PGENHEADERS_C SEG_PGENHEADERS_H
#define SEG_PYDEBUG_C SEG_PYDEBUG_H
#define SEG_PYERRORS_C SEG_PYERRORS_H
#define SEG_PYFPE_C SEG_PYFPE_H
#define SEG_PYSTATE_C SEG_PYSTATE_H
#define SEG_PYTHREAD_C SEG_PYTHREAD_H
#define SEG_RANGEOBJECT_C SEG_RANGEOBJECT_H 
#define SEG_RENAME2_C SEG_RENAME2_H 
#define SEG_SLICEOBJECT_C SEG_SLICEOBJECT_H

#define SEG_STRINGOBJECT_C SEG_STRINGOBJECT_H 
#define SEG_STRUCTMEMBER_C SEG_STRUCTMEMBER_H 
#define SEG_THREAD_C SEG_THREAD_H 
#define SEG_TRACEBACK_C SEG_TRACEBACK_H 
#define SEG_TUPLEOBJECT_C SEG_TUPLEOBJECT_H 
#define SEG_TYPEOBJECT_C SEG_TYPEOBJECT_H

#define SEG_BLTINMODULE_C SEG_BLTINMODULE_H 
#define SEG_SYSMODULE_C SEG_SYSMODULE_H 

#define SEG_ACCELER_C SEG_ACCELER_H
#define SEG_GRAMMAR_C SEG_GRAMMAR_H 
#define SEG_NODE_C SEG_NODE_H 
#define SEG_PARSETOK_C SEG_PARSETOK_H 
#define SEG_PGEN_C SEG_PGEN_H 
#define SEG_LISTNODE_C SEG_LISTNODE_H 
#define SEG_METAGRAMMAR_C SEG_METAGRAMMAR_H 
#define SEG_PARSER_C SEG_PARSER_H 
#define SEG_TOKEN_C SEG_TOKEN_H 
#define SEG_TOKENIZER_C SEG_TOKENIZER_H 
#define SEG_MYREADLINE_C SEG_MYREADLINE_H 




#endif /* PALM_SEGMENT_DEFS_H */
