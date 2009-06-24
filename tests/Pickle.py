import pickle
import pickletools
import object_matrix
import copy_reg
from types import FunctionType, BuiltinFunctionType
from StringIO import StringIO

pg = object_matrix.ParticleIDGenerator(0)

stream = StringIO()
p = pickle.Pickler(stream)

id0 = pg()
id1 = pg()

p.dump(id0)
p.dump(id1)

stream.flush()

stream.seek(0)
up = pickle.Unpickler(stream)

_id0 = up.load()
_id1 = up.load()

assert id0 == _id0
assert id1 == _id1
