## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 958 2007-08-28 05:38:02Z matsunaga $

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_GDS = $(YMTOOLS_BUILDDIR)/libraries/libym_gds/libym_gds.la

noinst_PROGRAMS = \
	gdsparse \
	gdsprint \
	gdsencode

gdsparse_SOURCES = \
	gdsparse.cc
gdsparse_LDADD = \
	$(LIBYM_GDS)

gdsprint_SOURCES = \
	gdsprint.cc
gdsprint_LDADD = \
	$(LIBYM_GDS)

gdsencode_SOURCES = \
	gdsencode.cc
gdsencode_LDADD =
