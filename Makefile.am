EXTRA_DIST = \
	setup.py \
	runtest.py \
	tests/array_helper_test.cpp \
	tests/filters_test.cpp \
	tests/MatrixSpace_test.cpp \
	tests/Vector3_test.cpp \
	tests/SerialIDGenerator_test.cpp \
	tests/vmap_test.cpp \
	tests/model_test.cpp \
	tests/Model_test.py \
	tests/NetworkRules_test.py \
	tests/ReactionRule_test.py \
	peer/Exception.hpp \
	peer/GeneratorIteratorWrapper.hpp \
	peer/Identifier.hpp \
	peer/MatrixSpace.hpp \
	peer/ReactionRule.hpp \
	peer/SpeciesType.hpp \
	peer/enumerator.hpp \
	peer/pickle_support.hpp \
	peer/py_hash_support.hpp \
	peer/set_indexing_suite.hpp \
	peer/tuple_converters.hpp \
	peer/utils.hpp \
	peer/numpy/ndarray_converters.hpp \
	peer/numpy/pyarray_backed_allocator.hpp \
	peer/numpy/type_mappings.hpp \
	peer/numpy/wrapped_multi_array.hpp \
	BasicNetworkRulesImpl.cpp \
	NetworkRules.cpp \
	utils.cpp \
	BasicNetworkRulesImpl.hpp \
	Defs.hpp \
	Identifier.hpp \
	MatrixSpace.hpp \
	Model.hpp \
	NetworkRules.hpp \
	Particle.hpp \
	ParticleID.hpp \
	ReactionRule.hpp \
	SerialIDGenerator.hpp \
	Shell.hpp \
	ShellID.hpp \
	SpeciesInfo.hpp \
	SpeciesTypeID.hpp \
	Sphere.hpp \
	Vector3.hpp \
	array_helper.hpp \
	array_traits.hpp \
	exceptions.hpp \
	filters.hpp \
	generator.hpp \
	get_mapper_mf.hpp \
	utils.hpp \
	vmap.hpp \
	object_matrix_module.cpp

all-local: object_matrix.so

clean-local:
	$(PYTHON) setup.py clean -a
	rm -f tests/model_test
	rm -f object_matrix.so

check-local: tests/model_test tests/MatrixSpace_test tests/filters_test tests/array_helper_test tests/ReactionRule_test tests/SerialIDGenerator_test tests/World_test object_matrix.so
	tests/model_test
	tests/array_helper_test
	tests/SerialIDGenerator_test
	tests/filters_test
	tests/ReactionRule_test
	tests/MatrixSpace_test
	$(PYTHON) runtest.py

tests/array_helper_test: tests/array_helper_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/SerialIDGenerator_test: tests/SerialIDGenerator_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/filters_test: tests/filters_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/ReactionRule_test: tests/ReactionRule_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/MatrixSpace_test: tests/MatrixSpace_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/model_test: tests/model_test.cpp src/NetworkRules.cpp src/BasicNetworkRulesImpl.cpp src/Model.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

tests/World_test: tests/World_test.cpp
	$(CXX) -DHAVE_CONFIG_H -I. -Isrc -o $@ $^

object_matrix.so: object_matrix_module.cpp
	$(PYTHON) setup.py build_ext --inplace
