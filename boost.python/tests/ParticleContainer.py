import object_matrix as om

c = om.ParticleContainer(10.0, 10)
pidgen = om.ParticleIDGenerator(0)
spidgen = om.SpeciesIDGenerator(0)

id1 = pidgen()
id2 = pidgen()

spid1 = spidgen()

c[id1] = ((4., 3., 2.), 0.6, spid1)
c[id2] = ((5., 2., 3.), 0.7, spid1)

print c.getNeighborsWithinRadiusNoSort((4.5, 2.6, 2.4), 0.4)
print c.getNeighborsWithinRadiusNoSort((4.5, 2.6, 2.4), 0.2)

