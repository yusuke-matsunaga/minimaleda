## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIR =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd_bmc.la

libym_bdd_bmc_la_LIBADD = \
	$(GMP_LIBS)

libym_bdd_bmc_la_LDFLAGS =

libym_bdd_bmc_la_SOURCES = \
	BddMgrClassic.h \
	BddMgrClassic.cc \
	BmcNode.h \
	BmcCompTbl.h \
	BmcCompTbl.cc \
	bmc_and.cc \
	bmc_ite.cc \
	bmc_sup.cc \
	bmc_count.cc \
	bmc_isop.cc \
	bmc_onepath.cc \
	bmc_symmetry.cc \
	bmc_vs.cc \
	bmc_ls.cc
