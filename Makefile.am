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
	src/BasicNetworkRulesImpl.cpp \
	src/NetworkRules.cpp \
	src/utils.cpp \
	src/BasicNetworkRulesImpl.hpp \
	src/Defs.hpp \
	src/Identifier.hpp \
	src/MatrixSpace.hpp \
	src/Model.hpp \
	src/NetworkRules.hpp \
	src/Particle.hpp \
	src/ParticleID.hpp \
	src/ReactionRule.hpp \
	src/SerialIDGenerator.hpp \
	src/Shell.hpp \
	src/ShellID.hpp \
	src/SpeciesInfo.hpp \
	src/SpeciesTypeID.hpp \
	src/Sphere.hpp \
	src/Vector3.hpp \
	src/array_helper.hpp \
	src/array_traits.hpp \
	src/exceptions.hpp \
	src/filters.hpp \
	src/generator.hpp \
	src/get_mapper_mf.hpp \
	src/utils.hpp \
	src/utils/array_helper.hpp \
	src/utils/array_traits.hpp \
	src/utils/fun_composition.hpp \
	src/utils/fun_wrappers.hpp \
	src/utils/get_default_impl.hpp \
	src/utils/get_mapper_mf.hpp \
	src/utils/memberwise_compare.hpp \
	src/utils/pair.hpp \
	src/utils/pointer_preds.hpp \
	src/utils/range.hpp \
	src/utils/range_support.hpp \
	src/utils/reference_or_instance.hpp \
	src/vmap.hpp \
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
