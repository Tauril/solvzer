AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I build-aux/m4

AM_CXXFLAGS = -std=c++14 -Wall -Wextra
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_CPPFLAGS += -I$(top_builddir)

bin_PROGRAMS = solvzer

EXTRA_DIST = bootstrap README.md

solvzer_SOURCES = 	src/misc/contract.hh src/misc/contract.cc	    \
			src/misc/indent.hh src/misc/indent.cc		    \
			src/cube/color.hh src/cube/color.cc		    \
			src/cube/face.hh src/cube/face.hxx src/cube/face.cc \
			src/cube/cube.hh src/cube/cube.hxx src/cube/cube.cc \
			src/cube/move.hh src/cube/move.hxx src/cube/move.cc \
			src/solvzer.cc
