from test_support import *

try:
    compile("","","")
    has_compiler = 1
except MissingFeatureError:
    has_compiler = 0

import sys
print 'test_b1'
import test_b1
del test_b1
unload('test_b1')

print 'test_b2'
import test_b2
del test_b2
unload('test_b2')

print "test_types"
import test_types
del test_types
unload('test_types')


print "test_pow"
import test_pow
del test_pow
unload('test_pow')

if has_compiler:
    print "test_grammar"
    import test_grammar

print "all tests passed"

