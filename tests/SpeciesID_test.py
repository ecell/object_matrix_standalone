import object_matrix as om
import unittest

class SpeciesTypeIDTest(unittest.TestCase):
    def test(self):
        gen = om.SpeciesTypeIDGenerator(0)
        id1 = gen()
        id2 = gen()
        id3 = gen()

        print id1
        print id2
        print id3

        self.assertTrue( id1 < id2 )
        self.assertTrue( id2 < id3 )
        self.assertTrue( id1 < id3 )
        self.assertTrue( id1 != id2 )
        self.assertTrue( id2 != id3 )
        self.assertTrue( id1 != id3 )

        gen1 = om.SpeciesTypeIDGenerator(0)
        gen2 = om.SpeciesTypeIDGenerator(1)

        id1_1 = gen1()
        id1_2 = gen2()

        print id1_1
        print id1_2

        self.assertTrue( id1_1 != id1_2 )
