## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIR =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd_bmm.la

libym_bdd_bmm_la_LIBADD = \
	$(GMP_LIBS)

libym_bdd_bmm_la_LDFLAGS =

libym_bdd_bmm_la_SOURCES = \
	BddMgrModern.h \
	BddMgrModern.cc \
	BmmNode.h \
	BmmVar.h \
	BmmVar.cc \
	BmmCompTbl.h \
	BmmCompTbl.cc \
	bmm_and.cc \
	bmm_ite.cc \
	bmm_sup.cc \
	bmm_count.cc \
	bmm_isop.cc \
	bmm_onepath.cc \
	bmm_symmetry.cc \
	bmm_vs.cc \
	bmm_ls.cc
