AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I build-aux/m4

AM_DEFAULT_SOURCE_EXT = .cc

AM_CXXFLAGS = -std=c++14 -Wall -Wextra
AM_CPPFLAGS = -I$(top_srcdir)/src -I$(top_builddir)/src
AM_CPPFLAGS += -I$(top_builddir)
AM_CPPFLAGS += $(OPENCV_CFLAGS)

EXTRA_DIST = bootstrap README.md

## ------------ ##
## libsolvzer.  ##
## ------------ ##

lib_LTLIBRARIES = libsolvzer.la
libsolvzer_la_SOURCES = src/misc/contract.hh src/misc/contract.cc	     \
			 src/misc/indent.hh src/misc/indent.cc		     \
			 src/cube/color.hh src/cube/color.cc		     \
			 src/cube/corner.hh				     \
			 src/cube/edge.hh				     \
			 src/cube/facelet.hh src/cube/facelet.cc 	     \
			 src/cube/face.hh src/cube/face.hxx src/cube/face.cc \
			 src/cube/cube.hh src/cube/cube.hxx src/cube/cube.cc \
			 src/cube/move.hh src/cube/move.cc		     \
			 src/cube/coord.hh src/cube/coord.cc		     \
			 src/cube/search.hh src/cube/search.cc		     \
			 src/detect/detector.hh src/detect/detector.cc	     \
			 src/detect/displayer.hh src/detect/displayer.cc     \
			 src/misc/display.hh src/misc/display.cc	     \
			 src/misc/controller.hh src/misc/controller.cc	     \
			 src/misc/state.hh src/misc/state.cc		     \
			 src/misc/texture.hh src/misc/texture.cc	     \
			 src/solvzer.cc

libsolvzer_la_LDFLAGS = $(shell pkg-config --cflags --libs sdl2)
libsolvzer_la_LDFLAGS += $(shell pkg-config --cflags --libs SDL2_ttf)
libsolvzer_la_LDFLAGS += $(shell pkg-config --cflags --libs SDL2_image)
libsolvzer_la_LDFLAGS += -lpthread
libsolvzer_la_LIBADD = $(OPENCV_LIBS)

bin_PROGRAMS = src/solvzer

src_solvzer_LDADD = libsolvzer.la

# Required to prevent `error: object `src/solvzer.$(OBJECT)' created both
# with libtool and without'
libsolvzer_la_CXXFLAGS = $(AM_CXXFLAGS)

TESTS = src/cube/search-test
check_PROGRAMS = src/cube/search-test
src_cube_search_test_LDADD = libsolvzer.la
