from test_support import *
import test_support
import sys
sys.modules['test.test_support'] = test_support

try:
    compile("print 1","<string>","exec")
    has_compiler = 1
except MissingFeatureError:
    has_compiler = 0

import sys

print 'test_b1'
import test_b1

print 'test_b2'
import test_b2

print "test_cpickle"
import test_cpickle

print "test_types"
import test_types

print "test_pow"
import test_pow

if has_compiler:
    print "test_grammar"
    import test_grammar

print "test.test_contains"
import test.test_contains

print "test_exceptions"
import test_exceptions

print "test.test_opcodes"
import test.test_opcodes

print "test.test_rfc822"
import test.test_rfc822

print "test_string"
import test_string

print "test_time"
import test_time

print "test.test_userdict"
import test.test_userdict

print "test.test_userlist"
import test.test_userlist

print "tests completed"

