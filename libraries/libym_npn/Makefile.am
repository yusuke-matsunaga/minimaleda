## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1508 2008-06-30 04:55:42Z matsunaga $

SUBDIRS = \
	ym_npn \
	utils \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

YM_BASE = $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

noinst_LTLIBRARIES = libym_npn.la

libym_npn_la_LIBADD = 
	$(YM_BASE)

libym_npn_la_LDFLAGS =
libym_npn_la_SOURCES = \
	w_table.h \
	TvFunc.cc \
	NpnRawSig.h \
	NpnRawSig.cc \
	NpnConf.h \
	NpnConf.cc \
	NpnMgr.cc \
	NpnMap.cc
