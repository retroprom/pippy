"""This module contains utilities for writing compiled python to
to a palm database."""


import marshal
import os
import imp
import tempfile
import struct

DEFAULT_CID = 'PyMd'
RESOURCE_BASE = 0x3e8
RESOURCE_INCR = 10


class frozenRecord:
    def __init__(self, name, code, size):
        self.name = name
        self.code = code
        self.size = size


def build_struct(module_filename, name=None):
    """Build the following structure:

    typedef struct _frozen_record {
        int size;
        char *name;
        unsigned char *code;
    } frozen_record;

    """
    fh = open(module_filename,'rb')
    
    if imp.get_magic() != fh.read(4):
        raise IOError, "File " + module_filename + " is not a valid pyc file"

    fh.seek(0)
    code = fh.read()
    code = code[8:]  # skip first 8 bytes
    length = len(code)
    n, ext = os.path.splitext(os.path.basename(module_filename))
    if not name:
        name = n
    
    return frozenRecord( name, code, length )


def convert_file(module_filename, name=None, baseAddress=RESOURCE_BASE):
    """Convert a prc file to the desired palm database representation."""
    global creatorID
    print "converting", module_filename

    s = build_struct(module_filename, name)

    localfile, ext = os.path.splitext(os.path.basename(module_filename))
    localfile = creatorID + ("0000%x" % baseAddress)[-4:] + '.bin'
    fh = open(localfile, 'wb')
    fh.write(struct.pack(">i", s.size) )
    fh.write(s.name)
    fh.write('\0')
    fh.write(s.code)
    fh.close()

def compiled_filename(filename):
    n, ext = os.path.splitext(filename)
    if ext == '.py':
        print "compiling", filename
        from py_compile import compile
        compile(filename)
        filename = filename + 'c'
    return filename


def run(module_filenames, main=None, baseAddress=RESOURCE_BASE):
    import types
    if type(module_filenames) not in (types.TupleType, types.ListType):
        module_filenames = [module_filenames]

    if main:
        cname = compiled_filename(module_filenames[0])
        convert_file(cname, '__main__', baseAddress)
        del module_filenames[0]
        baseAddress = baseAddress + RESOURCE_INCR
    
    for name in module_filenames:
        cname = compiled_filename(name)
        convert_file(cname, baseAddress=baseAddress)
        baseAddress = baseAddress + RESOURCE_INCR
        

if __name__ == '__main__':
    ## Need to add option for obtaining the name of the module
    ## - particularly for __main__ - maybe just add a flag --main?
    ##
    import sys
    import getopt
    optlist, files = getopt.getopt(sys.argv[1:], 'x',
                                  ['main', 'cid=', 'base_address='])

    creatorID = DEFAULT_CID
    main = None
    baseAddress = RESOURCE_BASE
    for opt, val in optlist:
        if opt == '--main':
            main = '__main__'
        if opt == '--cid':
            creatorID = val
            if len(val) > 4:
                raise ValueError, "Creator ID must be <= 4 bytes"
        if opt == '--base_address':
            baseAddress = val

    run(files, main, baseAddress)
    
    
# NOTES:

#  create a pdb database:
#
#  the following only works for .prc files!!!  pdb files have a
#  different format
#  build-prc -t dATA pymods2.pdb 'Python Modules-PyMd' PyMd *bin
#
