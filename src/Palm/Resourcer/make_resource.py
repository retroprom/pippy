#
# Adapted from makefreeze.py
#


import marshal
import string
import struct

DEFAULT_CID = 'PyMd'
RESOURCE_BASE = 0x3e8
RESOURCE_INCR = 10



# Write a file containing frozen code for the modules in the dictionary.

header = """
#include "Python.h"
#include <PalmOS.h>
#include <PalmCompatibility.h>

struct _myfrozen {
	struct _frozen *frozen;
	VoidHand handle;
};
typedef struct _myfrozen myfrozen;

struct _frozen _PyImport_FrozenModules[] = {
"""

trailer1 = """\
    {0, 0, 0} /* sentinel */
};

VoidHand frozen_handles[] = {
"""

trailer2 = """
    0}; /* sentinel */
"""

def make_resource(resfp, outfp, dict, debug=0, entry_point=None, odir=None):
    done = []
    mods = dict.keys()
    mods.sort()
    for mod in mods:
        m = dict[mod]
        #mangled = string.join(string.split(mod, "."), "__")
        mangled = mod
        if m.__code__:
            if debug:
                print "freezing", mod, "..."
            str = marshal.dumps(m.__code__)
            size = len(str)
            if m.__path__:
                # Indicate package by negative size
                size = -size
            done.append((mod, mangled, size))
            write_resource(resfp, mangled, str, size, odir)
    if debug:
        print "generating table of frozen modules"
    outfp.write(header)
    for mod, mangled, size in done:
        outfp.write('\t{"%s", NULL, 0},\n' % (mod,))
    outfp.write(trailer1)
    outfp.write((len(done)) * '0,')
    if entry_point: outfp.write(entry_point)
    outfp.write(trailer2)


creatorID = DEFAULT_CID
address = RESOURCE_BASE
import os

def write_resource(resfp, name, str, size, odir=None):
    global address
    localfile = creatorID + ("0000%x" % address)[-4:] + '.bin'
    if odir:
        localfile = os.path.join(odir, localfile)
    fh = open(localfile, 'wb')
    fh.write(struct.pack(">i", size) )
    fh.write(name)
    fh.write('\0')
    fh.write(str)
    resfp.write('DATA ' + "\"" + creatorID +"\" ID 0x" + ("0000%x" % address)[-4:] + " \"" + localfile + "\"")
    resfp.write(os.linesep)
    address = address + RESOURCE_INCR
