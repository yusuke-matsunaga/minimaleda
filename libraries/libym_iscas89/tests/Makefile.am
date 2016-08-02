## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1097 2007-12-21 08:12:52Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	parser.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

noinst_PROGRAMS = \
	parsertest

parsertest_SOURCES = \
	parsertest.cc
parsertest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_iscas89/libym_iscas89.la
