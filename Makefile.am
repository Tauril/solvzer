AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I build-aux/m4

AM_CXXFLAGS = -std=c++14 -Wall -Wextra
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_CPPFLAGS += -I$(top_builddir)

bin_PROGRAMS = solvzer

EXTRA_DIST = bootstrap README.md

solvzer_SOURCES = 	src/cube.hh src/cube.cc 	\
			src/solvzer.cc
