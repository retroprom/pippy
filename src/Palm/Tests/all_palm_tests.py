from test_support import *

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

# print "test_grammar"
# import test_grammar

print "all tests passed"

