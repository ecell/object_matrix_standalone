import object_matrix as om

gen = om.ParticleIDGenerator(0)
s = om.ParticleIDSet()

id1 = gen()
id2 = gen()
id3 = gen()

s.add(id1)
assert len(s) == 1
assert id1 in s
assert id2 not in s
assert id3 not in s

s.add(id2)
assert len(s) == 2
assert id1 in s
assert id2 in s
assert id3 not in s

s.add(id3)
assert len(s) == 3
assert id1 in s
assert id2 in s
assert id3 in s

s.remove(id3)
assert len(s) == 2
assert id1 in s
assert id2 in s
assert id3 not in s

s.remove(id1)
assert len(s) == 1
assert id1 not in s
assert id2 in s
assert id3 not in s

s.remove(id2)
assert len(s) == 0
assert id1 not in s
assert id2 not in s
assert id3 not in s


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

