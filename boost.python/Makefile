PYTHON = python

all: object_matrix.so

clean:
	$(PYTHON) setup.py clean -a
	rm -f object_matrix.so

object_matrix.so: object_matrix_module.cpp
	$(PYTHON) setup.py build_ext --inplace

.PHONY: all clean
