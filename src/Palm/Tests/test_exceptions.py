# Python test set -- part 5, built-in exceptions

from test_support import *
from types import ClassType

print '5. Built-in exceptions'
# XXX This is not really enough, each *operation* should be tested!

def r(thing):
    if type(thing) == ClassType:
        print thing.__name__
    else:
        print thing

r(AttributeError)
import sys
try: x = sys.undefined_attribute
except AttributeError: pass

r(EOFError)
# eliminated file IO code

r(IOError)
try: open('this file does not exist', 'r')
except IOError: pass

r(ImportError)
try: import undefined_module
except ImportError: pass

r(IndexError)
x = []
try: a = x[10]
except IndexError: pass

r(KeyError)
x = {}
try: a = x['key']
except KeyError: pass

r(KeyboardInterrupt)
print '(not testable in a script)'

r(MemoryError)
print '(not safe to test)'

r(NameError)
try: x = undefined_variable
except NameError: pass

r(OverflowError)
x = 1
try:
        while 1: x = x+x
except OverflowError: pass

r(RuntimeError)
print '(not used any more?)'

r(SyntaxError)
try: exec '/\n'
except SyntaxError: pass

r(SystemError)
print '(hard to reproduce)'

r(SystemExit)
import sys
try: sys.exit(0)
except SystemExit: pass

r(TypeError)
try: [] + ()
except TypeError: pass

r(ValueError)
try: x = chr(10000)
except ValueError: pass

r(ZeroDivisionError)
try: x = 1/0
except ZeroDivisionError: pass

