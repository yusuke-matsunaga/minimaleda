## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap_patgen.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_patgen_la_SOURCES = \
	patgen_nsdef.h \
	PatGen.h \
	PatGen.cc \
	PgFuncMgr.h \
	PgFuncMgr.cc \
	PgNode.h \
	pg_dump.cc

libym_techmap_patgen_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_npn/libym_npn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_techmap_patgen_la_LDFLAGS =
