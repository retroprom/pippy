import easytest
from modulefinder import _behead


class behead_Test(easytest.Experiment):

    def Erect(self):
        self.path = '/a/b/c'
    def Teardown(self):
        pass
    
    def testEmpty(self):
        assert _behead('', self.path) == self.path
    def testSlash(self):
        assert _behead('/',self.path) == 'a/b/c'
    def testOne(self):
        assert _behead('/a',self.path) == 'b/c'
        assert _behead('/a/',self.path) == 'b/c'
    def testTwo(self):
        assert _behead('/a/b',self.path) == 'c'
        assert _behead('/a/b/',self.path) == 'c'
    def testAll(self):
        assert _behead('/a/b/c',self.path) == ''
        assert _behead('/a/b/c/',self.path) == ''
    def testTooMuch(self):
        assert _behead('/a/b/c/x',self.path) == self.path

def verify(outcome):
    suite = easytest.Suite()
    suite.Enroll(behead_Test(name='testEmpty'))
    suite.Enroll(behead_Test(name='testSlash'))
    suite.Enroll(behead_Test(name='testOne'))
    suite.Enroll(behead_Test(name='testTwo'))
    suite.Enroll(behead_Test(name='testAll'))
    suite.Enroll(behead_Test(name='testTooMuch'))
    suite(outcome)

if __name__ == '__main__':
    import sys
    easytest.trust_but_verify(sys.modules[__name__])
    
