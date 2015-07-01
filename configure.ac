AC_INIT([rust-mode], [0.1])

AC_CONFIG_AUX_DIR([script])
AM_INIT_AUTOMAKE([-Wall -Werror subdir-objects foreign])

AM_PROG_AR
LT_INIT([dlopen])

AC_PROG_CXX
AC_LANG([C++])
AX_CXX_COMPILE_STDCXX_11
AC_PROG_INSTALL

AC_CHECK_TYPES([ptrdiff_t])
AC_PREREQ

AC_CONFIG_FILES([Makefile
				 src/Makefile])
AC_CONFIG_HEADERS([config.hpp])
AC_CONFIG_MACRO_DIRS([m4])

AC_OUTPUT
