AC_INIT
AC_CONFIG_SRCDIR([object_matrix_module.cpp])
AM_INIT_AUTOMAKE([species-preliminary],[0.0])
AM_PROG_LIBTOOL
AM_PATH_PYTHON(2.5)
AC_PROG_CXX

AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([unordered_map tr1/unordered_map boost/unordered_map.hpp boost/functional/hash.hpp])
AC_CHECK_TYPES([std::hash<int>],[],[],[
#include <functional>
])
AC_CHECK_HEADERS([tr1/functional], [
  AC_CHECK_TYPES([std::tr1::hash<int>],[],[],[
#include <tr1/functional>
  ])
])
AC_LANG_POP
AM_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT([])
