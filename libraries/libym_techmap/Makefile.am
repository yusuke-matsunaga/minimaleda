## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_techmap \
	patgen \
	. \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_techmap.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_techmap_la_SOURCES = \
	AreaCover.h \
	AreaCover.cc \
	CnGraph.cc \
	CnGraph_dump.cc \
	FuncGroup.h \
	MapRecord.h \
	MapRecord.cc \
	Match.h \
	Match.cc \
	PatGraph.h \
	PatMatcher.h \
	PatMatcher.cc \
	PatMgr.cc \
	RepFunc.h

libym_techmap_la_LIBADD = \
	patgen/libym_techmap_patgen.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sbj/libym_sbj.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cell/libym_cell.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_techmap_la_LDFLAGS =
