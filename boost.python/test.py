from object_matrix import *
import numpy

pGen = ParticleIDGenerator(0)
sGen = SpeciesIDGenerator(0)

pCont = ParticleContainer(1.0, 3)

sID = sGen()
pID = pGen()

p = (numpy.array([0.5, 0.5, 0.5]), 0.01, sID)

# insert particle
pCont[pID] = p

print pCont.contains(pID)

# change radius of particle and re-insert it with same particle ID
pCont[pID] = (pCont[pID][0], 0.00002, pCont[pID][2])

print pCont.contains(pID)
