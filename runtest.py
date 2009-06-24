import os
import sys
import unittest
from glob import glob

testdir = 'tests'

sys.path.append(testdir)

def buildSuite():
    suite = unittest.TestSuite()
    for mod in map(__import__,
        [ os.path.basename(os.path.splitext(file)[0])
            for file in glob(testdir + '/*_test.py') ]):
        suite.addTest(unittest.findTestCases(mod)) 
    return suite

unittest.main(defaultTest='buildSuite')
