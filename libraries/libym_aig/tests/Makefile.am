## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

EXTRA_DIST = local_defs $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

LIBYM_AIG = $(YMTOOLS_BUILDDIR)/libraries/libym_aig/libym_aig.la

noinst_PROGRAMS = \
	bnet2aig \
	genpat \
	genpat2 \
	enum_pat3

bnet2aig_SOURCES = \
	bnet2aig.cc
bnet2aig_LDADD = \
	$(LIBYM_AIG) \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

genpat_SOURCES = \
	GpNode.h \
	GpNode.cc \
	genpat.cc

genpat_LDADD = \
	$(LIBYM_AIG)

genpat2_SOURCES = \
	genpat2.cc

genpat2_LDADD =

enum_pat3_SOURCES = \
	enum_pat3.cc

enum_pat3_LDADD = \
	$(LIBYM_AIG)
