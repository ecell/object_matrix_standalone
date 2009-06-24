import object_matrix as om
import unittest

class ParticleIDTestCase(unittest.TestCase):
    def test(self):
        gen = om.ParticleIDGenerator(0)
        id1 = gen()
        id2 = gen()
        id3 = gen()

        s = om.ParticleIDSet()
        s.add(id1)
        self.assertEqual(len(s), 1)
        self.assertTrue(id1 in s)
        self.assertTrue(id2 not in s)
        self.assertTrue(id3 not in s)

        s.add(id2)
        self.assertEqual(len(s), 2)
        self.assertTrue(id1 in s)
        self.assertTrue(id2 in s)
        self.assertTrue(id3 not in s)

        s.add(id3)
        self.assertEqual(len(s), 3)
        self.assertTrue(id1 in s)
        self.assertTrue(id2 in s)
        self.assertTrue(id3 in s)

        s.remove(id3)
        self.assertEqual(len(s), 2)
        self.assertTrue(id1 in s)
        self.assertTrue(id2 in s)
        self.assertTrue(id3 not in s)

        s.remove(id1)
        self.assertEqual(len(s), 1)
        self.assertTrue(id1 not in s)
        self.assertTrue(id2 in s)
        self.assertTrue(id3 not in s)

        s.remove(id2)
        self.assertEqual(len(s), 0)
        self.assertTrue(id1 not in s)
        self.assertTrue(id2 not in s)
        self.assertTrue(id3 not in s)

    def test_gen(self):
        gen = om.ParticleIDGenerator(0)
        id1 = gen()
        id2 = gen()
        print id1.lot, id1.serial
        print id2.lot, id2.serial

        gen = om.ParticleIDGenerator(1)
        id1 = gen()
        id2 = gen()
        print id1.lot, id1.serial
        print id2.lot, id2.serial

