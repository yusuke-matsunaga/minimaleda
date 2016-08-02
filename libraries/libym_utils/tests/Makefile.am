## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $


EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS = \
	file.test \
	namemgr.test \
	combigen.test \
	permgen.test \
	multicombigen.test \
	multipermgen.test \
	strbuff.test \
	dllist.test \
	heaptree.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

noinst_PROGRAMS = \
	file_test \
	namemgr_test \
	combigen_test \
	permgen_test \
	multicombigen_test \
	multipermgen_test \
	strbuff_test \
	randgen_test \
	randpermgen_test \
	randcombigen_test \
	dllist_test \
	heaptree_test

YMUTILS = ../libym_utils.la

file_test_SOURCES = \
	file_test.cc 
file_test_LDADD = \
	$(YMUTILS)

namemgr_test_SOURCES = \
	namemgr_test.cc
namemgr_test_LDADD = \
	$(YMUTILS)

combigen_test_SOURCES = \
	combigen_test.cc
combigen_test_LDADD = \
	$(YMUTILS)

permgen_test_SOURCES = \
	permgen_test.cc
permgen_test_LDADD = \
	$(YMUTILS)

multicombigen_test_SOURCES = \
	multicombigen_test.cc
multicombigen_test_LDADD = \
	$(YMUTILS)

multipermgen_test_SOURCES = \
	multipermgen_test.cc
multipermgen_test_LDADD = \
	$(YMUTILS)

strbuff_test_SOURCES = \
	strbuff_test.cc
strbuff_test_LDADD = \
	$(YMUTILS)

randgen_test_SOURCES = \
	randgen_test.cc
randgen_test_LDADD = \
	$(YMUTILS)

randpermgen_test_SOURCES = \
	randpermgen_test.cc
randpermgen_test_LDADD = \
	$(YMUTILS)

randcombigen_test_SOURCES = \
	randcombigen_test.cc
randcombigen_test_LDADD = \
	$(YMUTILS)

dllist_test_SOURCES = \
	dllist_test.cc
dllist_test_LDADD = \
	$(YMUTILS)

heaptree_test_SOURCES = \
	heaptree_test.cc
heaptree_test_LDADD = \
	$(YMUTILS)
