import test_support

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

print "test_contains"
import test_contains

print "test_exceptions"
import test_exceptions

print "test_opcodes"
import test_opcodes

print "test_rfc822"
import test_rfc822

print "test_string"
import test_string

print "test_time"
import test_time

print "test_userdict"
import test_userdict

print "test_userlist"
import test_userlist

print "tests completed"

