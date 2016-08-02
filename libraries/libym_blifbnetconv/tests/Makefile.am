## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	read_blif.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	bliftest

bliftest_SOURCES = \
	bliftest.cc
bliftest_LDADD = \
	 $(YMTOOLS_BUILDDIR)/libraries/libym_blifbnetconv/libym_blifbnetconv.la \
	 $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la
