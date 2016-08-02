## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1293 2008-02-21 02:25:52Z matsunaga $

SUBDIRS =

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

LIBYM_SAT = $(YMTOOLS_BUILDDIR)/libraries/libym_sat/libym_sat.la

noinst_PROGRAMS = \
	dimacstest

bin_PROGRAMS = \
	ymsat \
	ymsat2

dimacstest_SOURCES = \
	dimacstest.cc
dimacstest_LDADD = \
	$(LIBYM_SAT)

ymsat_SOURCES = \
	ymsat.cc
ymsat_LDADD = \
	$(LIBYM_SAT)

ymsat2_SOURCES = \
	ymsat2.cc
ymsat2_LDADD = \
	$(LIBYM_SAT)
