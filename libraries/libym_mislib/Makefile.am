## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS = \
	ym_mislib \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS=-d -v

CLEANFILES = \
	mislib_grammer.cc mislib_grammer.h y.output mislib_grammer.output

MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES = libym_mislib.la

libym_mislib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libym_utils/libym_utils.la

libym_mislib_la_LDFLAGS = 

libym_mislib_la_SOURCES = \
	mislib_grammer.yy \
	MislibLex.h \
	MislibLex.cc \
	MislibPt.h \
	MislibPtImpl1.h \
	MislibPtImpl1.cc \
	MislibPtImpl2.h \
	MislibPtImpl2.cc \
	MislibParserImpl.h \
	MislibParserImpl.cc \
	MislibHandler.cc \
	MislibParser.cc
