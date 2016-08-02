## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_seal \
	. \
	tests

INCLUDES = \
	$(GMP_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_seal.la

libym_seal_la_SOURCES = \
	BddFsm.h \
	BddFsm.cc \
	IdxMapper.h \
	fsm_analysis.cc \
	LogicSim.h \
	LogicSim.cc \
	fsm_analysis2.cc \
	Matrix.h \
	Matrix.cc \
	SMatrix.h \
	SMatrix.cc \
	Dfs.h \
	Dfs.cc \
	gauss_elimination.cc \
	lu_decomp.cc \
	MCAnalysis.cc

libym_seal_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la

libym_seal_la_LDFLAGS = \
	$(GMP_LIBS)
