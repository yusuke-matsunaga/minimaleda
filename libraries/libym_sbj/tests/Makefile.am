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

LIBYM_SBJ = $(YMTOOLS_BUILDDIR)/libraries/libym_sbj/libym_sbj.la
LIBYM_BNET = $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

bin_PROGRAMS = \
	read_blif \
	mindepth_test

read_blif_SOURCES = \
	read_blif.cc

read_blif_LDADD = \
	$(LIBYM_SBJ) \
	$(LIBYM_BNET)

mindepth_test_SOURCES = \
	mindepth_test.cc

mindepth_test_LDADD = \
	$(LIBYM_SBJ) \
	$(LIBYM_BNET)
