import object_matrix as om


gen = om.SpeciesIDGenerator(0)
id1 = gen()
id2 = gen()
id3 = gen()

print id1
print id2
print id3

assert id1 < id2
assert id2 < id3
assert id1 < id3
assert id1 != id2
assert id2 != id3
assert id1 != id3

gen1 = om.SpeciesIDGenerator(0)
gen2 = om.SpeciesIDGenerator(1)

id1_1 = gen1()
id1_2 = gen2()

print id1_1
print id1_2

assert id1_1 != id1_2
