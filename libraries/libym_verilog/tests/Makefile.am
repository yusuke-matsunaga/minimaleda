## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = . vltest 
if WITH_QT
SUBDIRS += ptview vlview
endif

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS=-d -v

EXTRA_DIST = local_defs $(TESTS)

CLEANFILES = \
	bvcalc.cc bvcalc.h bvcalc.output bvlex.cc

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

#TESTS = yacc.test

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	bvcalc

noinst_PROGRAMS = \
	testgen


#############################################################
# bvcalc
#############################################################

bvcalc_SOURCES = \
	bvcalc.yy \
	bvlex.ll

bvcalc_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la


#############################################################
# testgen
#############################################################

testgen_SOURCES = \
	testgen.cc

testgen_LDADD =