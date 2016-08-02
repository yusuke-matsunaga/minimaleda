## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1921 2008-12-21 13:55:05Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_BUILDDIR) \
	-I$(YMTOOLS_SRCDIR)/include

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

#LIBYM_BNET = $(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

#bin_PROGRAMS = \
#	makenode \
#	bliftest \
#	iscas89test
