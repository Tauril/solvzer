AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I build-aux/m4

AM_DEFAULT_SOURCE_EXT = .cc

SUBDIRS = .

bin_PROGRAMS = src/solvzer

AM_CPPFLAGS = -I$(top_srcdir)/src
AM_CPPFLAGS += $(BOOST_CPPFLAGS)
AM_CPPFLAGS += -I$(top_builddir)

EXTRA_DIST = bootstrap README.md

solvzer_SOURCES = src/solvzer.cc

TESTS =
check_PROGRAMS =
