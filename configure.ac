m4_define([SOLVZER_CLANG_PREREQ], [3.8])
m4_define([SOLVZER_GCC_PREREQ],   [5.0])

AC_PREREQ([2.64])

# Package set up.
AC_INIT([SolvZer], [1.0], [], [solvzer])

# Auxiliary files.
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

# Automake.
AM_INIT_AUTOMAKE([foreign -Wall silent-rules])

AM_SILENT_RULES([no])


## --------------------- ##
## C++ Compiler Set up.  ##
## --------------------- ##

# Look for a C++ compiler.
AC_LANG([C++])
AC_PROG_CXX

cxx_version=$($CXX --version |
              sed -n ['/^[^0-9]*\([0-9][0-9.]*[0-9]\).*$/{s//\1/;p;q;}'])
case $($CXX --version) in
  (*clang*)
    AS_VERSION_COMPARE([$cxx_version], [SOLVZER_CLANG_PREREQ],
                       [AC_MSG_WARN([clang++ >= SOLVZER_CLANG_PREREQ is required, have $cxx_version])])
    ;;
  (*g++*)
    AS_VERSION_COMPARE([$cxx_version], [SOLVZER_GCC_PREREQ],
                       [AC_MSG_WARN([g++ >= SOLVZER_GCC_PREREQ is required, have $cxx_version])])
    ;;
  (*)
    AC_MSG_WARN([g++ >= SOLVZER_GCC_PREREQ or clang++ >= SOLVZER_CLANG_PREREQ is recommended, have none])
    ;;
esac


## -------------------- ##
## Auxiliary programs.  ##
## -------------------- ##

# We don't need shared libraries, speed the compilation up.
LT_INIT([disable-shared])

AC_ARG_VAR([SOLVZER], [Rubik''s Cube Solving Robot])
AC_CHECK_PROGS([SOLVZER], [solvzer])


## ----------- ##
## Libraries.  ##
## ----------- ##

BOOST_REQUIRE([1.61])
BOOST_PROGRAM_OPTIONS


## ------- ##
## Tests.  ##
## ------- ##

AC_SUBST([GCC_PREREQ], [SOLVZER_PREREQ])


## --------------- ##
## Configuration.  ##
## --------------- ##

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
