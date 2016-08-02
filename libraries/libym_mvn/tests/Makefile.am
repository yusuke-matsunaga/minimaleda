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

LIBYM_MVN = ../libym_mvn.la

noinst_PROGRAMS = \
	makenode_test \
	read_test

makenode_test_SOURCES = \
	makenode_test.cc

makenode_test_LDADD = \
	$(LIBYM_MVN)

read_test_SOURCES = \
	read_test.cc

read_test_LDADD = \
	$(LIBYM_MVN)