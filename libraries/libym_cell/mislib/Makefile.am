## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_cell/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.1.1.1 $
## $Date: 2005/12/11 17:07:26 $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS = -d -v

CLEANFILES = \
	mislib_grammer.cc mislib_grammer.h y.output mislib_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_cell_mislib.la

libym_cell_mislib_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_cell_mislib_la_LDFLAGS =

libym_cell_mislib_la_SOURCES = \
	mislib_grammer.yy \
	CellMislibReader.cc \
	MislibLex.h \
	MislibLex.cc \
	MislibParser.h \
	MislibParser.cc \
	MislibPt.h \
	MislibPtImpl1.h \
	MislibPtImpl1.cc \
	MislibPtImpl2.h \
	MislibPtImpl2.cc
