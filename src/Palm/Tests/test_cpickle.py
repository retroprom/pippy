# Test the cPickle module

# Modified for the Palm
DATA = """(lp0
I0
aL1L
aI2
a(S'abc'
p1
g1
(i__main__
C
p2
(dp3
S'foo'
p4
I1
sS'bar'
p5
I2
sbg2
tp6
ag6
aI5
a.
"""

BINDATA = ']q\000(K\000L1L\012K\002(U\003abcq\001h\001(c__main__\012C\012q\002oq\003}q\004(U\003fooq\005K\001U\003barq\006K\002ubh\003tq\007h\007K\005e.'

import cPickle

class C:
    def __cmp__(self, other):
        return cmp(self.__dict__, other.__dict__)

import __main__
__main__.C = C

def dotest():
    c = C()
    c.foo = 1
    c.bar = 2
    x = [0, 1L, 2]
    y = ('abc', 'abc', c, c)
    x.append(y)
    x.append(y)
    x.append(5)
    print "dumps()"
    s = cPickle.dumps(x)
    print "loads()"
    x2 = cPickle.loads(s)
    if x2 == x: print "ok"
    else: print "bad"
    print "loads() DATA"
    x2 = cPickle.loads(DATA)
    if x2 == x: print "ok"
    else: print "bad"
    print "dumps() binary"
    s = cPickle.dumps(x, 1)
    print "loads() binary"
    x2 = cPickle.loads(s)
    if x2 == x: print "ok"
    else: print "bad"
    print "loads() BINDATA"
    x2 = cPickle.loads(BINDATA)
    if x2 == x: print "ok"
    else: print "bad"

    # Test protection against closed files  <eliminated>

    # Test specific bad cases
    for i in range(10):
        try:
            x = cPickle.loads('garyp')
        except cPickle.BadPickleGet, y:
            del y
        else:
            print "unexpected success!"
            break
    

dotest()
