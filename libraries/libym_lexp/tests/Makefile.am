## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = \
#	base_test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

YM_BASE = $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

YM_LEXP = $(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la

#noinst_PROGRAMS = \
#	base_test

#base_test_SOURCES = \
#	base_test.cc

#base_test_LDADD =
