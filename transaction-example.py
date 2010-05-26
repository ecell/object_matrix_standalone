import object_matrix as om
m = om.Model()
s1 = m.new_species_type()
s1["radius"] = ".02"
s1["D"] = ".1"
s2 = m.new_species_type()
s2["radius"] = ".03"
s2["D"] = ".2"

w = om.CyclicWorld(1.0, 10)
w.add_species(om.SpeciesInfo(s1.id, float(s1["radius"]), float(s1["D"])))
w.add_species(om.SpeciesInfo(s2.id, float(s2["radius"]), float(s2["D"])))

print list(w.species)
p1 = w.new_particle(s1.id, (.21, .23, .42))

tx = w.create_transaction()
print tx
p2 = tx.new_particle(s1.id, (.12, .23, .12))

print p1[1]
tx.update_particle((p1[0], om.Particle((.22, .24, .26), p1[1][1], p1[1][2])))
print list(tx.added_particles)
print list(tx.removed_particles)
print list(tx.modified_particles)
print list(tx)

tx.rollback()
