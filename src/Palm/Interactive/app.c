
#include "Python.h"
#include <PalmOS.h>
#include <PalmCompatibility.h>

struct _myfrozen {
	struct _frozen *frozen;
	VoidHand handle;
};
typedef struct _myfrozen myfrozen;

struct _frozen _PyImport_FrozenModules[] = {
	{"__main__", NULL, 0},
	{"all_palm_tests", NULL, 0},
	{"exceptions", NULL, 0},
	{"test_b1", NULL, 0},
	{"test_b2", NULL, 0},
	{"test_grammar", NULL, 0},
	{"test_pow", NULL, 0},
	{"test_socket", NULL, 0},
	{"test_support", NULL, 0},
	{"test_time", NULL, 0},
	{"test_types", NULL, 0},
    {0, 0, 0} /* sentinel */
};

VoidHand frozen_handles[] = {
0,0,0,0,0,0,0,0,0,0,0,
    0}; /* sentinel */
