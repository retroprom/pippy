import unittest
from modulefinder import _behead

class beheadTest(unittest.TestCase):
    path = '/a/b/c'
    def testEmpty(self):
        self.assertEquals(_behead('',self.path), self.path)
    def testSlash(self):
        self.assertEquals(_behead('/',self.path), 'a/b/c')
    def testOne(self):
        self.assertEquals(_behead('/a',self.path), 'b/c')
        self.assertEquals(_behead('/a/',self.path), 'b/c')
    def testTwo(self):
        self.assertEquals(_behead('/a/b',self.path), 'c')
        self.assertEquals(_behead('/a/b/',self.path), 'c')
    def testAll(self):
        self.assertEquals(_behead('/a/b/c',self.path), '')
        self.assertEquals(_behead('/a/b/c/',self.path), '')
    def testTooMuch(self):
        self.assertEquals(_behead('/a/b/c/x',self.path), self.path)
    
if __name__ == '__main__':
    unittest.main()
