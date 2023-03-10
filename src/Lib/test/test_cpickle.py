# Test the cPickle module

DATA = """(lp0
I0
aL1L
aF2.0
ac__builtin__
complex
p1
(F3.0
F0.0
tp2
Rp3
a(S'abc'
p4
g4
(i__main__
C
p5
(dp6
S'foo'
p7
I1
sS'bar'
p8
I2
sbg5
tp9
ag9
aI5
a.
"""

BINDATA = ']q\000(K\000L1L\012G@\000\000\000\000\000\000\000c__builtin__\012complex\012q\001(G@\010\000\000\000\000\000\000G\000\000\000\000\000\000\000\000tq\002Rq\003(U\003abcq\004h\004(c__main__\012C\012q\005oq\006}q\007(U\003fooq\010K\001U\003barq\011K\002ubh\006tq\012h\012K\005e.'

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
    x = [0, 1L, 2.0, 3.0+0j]
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

    # Test protection against closed files
    import tempfile, os
    fn = tempfile.mktemp()
    f = open(fn, "w")
    f.close()
    try:
        cPickle.dump(123, f)
    except IOError:
        pass
    else:
        print "dump to closed file should raise IOError"
    f = open(fn, "r")
    f.close()
    try:
        cPickle.load(f)
    except IOError:
        pass
    else:
        print "load from closed file should raise IOError"
    os.remove(fn)

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
