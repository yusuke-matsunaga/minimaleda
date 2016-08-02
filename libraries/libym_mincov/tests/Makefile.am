## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)

YFLAGS = -d -v

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES = \
	test_lex.cc \
	test_parser.h \
	test_parser.cc \
	test_parser.output \
	y.output

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_MINCOV = $(YMTOOLS_BUILDDIR)/libraries/libym_mincov/libym_mincov.la

noinst_PROGRAMS = \
	matrix_test \
	maxclique_test \
	mincov

matrix_test_SOURCES = \
	TestMatrix.h \
	TestMatrix.cc \
	test_parser.yy \
	test_lex.ll

matrix_test_LDADD = $(LIBYM_MINCOV)

maxclique_test_SOURCES = \
	maxclique_test.cc

maxclique_test_LDADD = $(LIBYM_MINCOV)

mincov_SOURCES = \
	mincov.cc

mincov_LDADD = $(LIBYM_MINCOV)
