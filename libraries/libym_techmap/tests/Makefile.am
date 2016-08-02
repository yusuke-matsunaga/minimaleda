## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_techmap

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

LIBYM_TECHMAP = $(YMTOOLS_BUILDDIR)/libraries/libym_techmap/libym_techmap.la
LIBYM_BNET = $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

bin_PROGRAMS = \
	patmgr_test \
	pgfuncmgr_test \
	areacover_test

patmgr_test_SOURCES = \
	patmgr_test.cc

patmgr_test_LDADD = \
	$(LIBYM_TECHMAP)

pgfuncmgr_test_SOURCES = \
	pgfuncmgr_test.cc

pgfuncmgr_test_LDADD = \
	$(LIBYM_TECHMAP)

areacover_test_SOURCES = \
	areacover_test.cc

areacover_test_LDADD = \
	$(LIBYM_TECHMAP) \
	$(LIBYM_BNET)
