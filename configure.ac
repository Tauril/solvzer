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

AM_SILENT_RULES([yes])


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

# Enable C++ 2014 support.
AC_CANONICAL_HOST
# Passing -stdlib=libc++ to clang++ is a requirement on OS X to enable
# full C++ 2011 support.  For more information on this, see
# http://cplusplusmusings.wordpress.com/2012/07/05/clang-and-standard-libraries
# We don't try to enable it by default on other platforms, as libc++
# is principally supported by OS X.
case $host_os in
  (darwin*)
    AX_CHECK_COMPILE_FLAG([-std=c++14 -stdlib=libc++],
                          [CXXFLAGS="$CXXFLAGS -std=c++14 -stdlib=libc++"
                           stdpass=true])
    ;;
esac

if ! "${stdpass-false}"; then
  AX_CHECK_COMPILE_FLAG([-std=c++14],
                        [CXXFLAGS="$CXXFLAGS -std=c++14"
                         stdpass=true])
fi

if ! "${stdpass-false}"; then
  AC_ERROR([unable to turn on C++14 mode with this compiler])
fi

# Try to use pipes between compiler stages to speed the compilation up.
AX_CHECK_COMPILE_FLAG([-pipe], [CXXFLAGS="$CXXFLAGS -pipe"])


## -------------------- ##
## Auxiliary programs.  ##
## -------------------- ##

AM_PROG_AR

# We don't need shared libraries, speed the compilation up.
LT_INIT([disable-shared])

AC_ARG_VAR([SOLVZER], [Rubik''s Cube Solving Robot])
AC_CHECK_PROGS([SOLVZER], [solvzer])


## ----------- ##
## Libraries.  ##
## ----------- ##

BOOST_REQUIRE([1.61])
BOOST_PROGRAM_OPTIONS

PKG_CHECK_MODULES([OPENCV], [opencv])

AC_CHECK_HEADERS([SDL2/SDL.h], [],
  [AC_MSG_ERROR(You need the SDL development library.)])

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
