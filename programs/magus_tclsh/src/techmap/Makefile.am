## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_techmap_tcl.la

libmagus_techmap_tcl_la_SOURCES = \
	AreaMapCmd.h \
	AreaMapCmd.cc \
	Conv2SbjCmd.h \
	Conv2SbjCmd.cc \
	DumpCnCmd.h \
	DumpCnCmd.cc \
	DumpSbjCmd.h \
	DumpSbjCmd.cc \
	LoadPatCmd.h \
	LoadPatCmd.cc \
	TechmapCmd.h \
	TechmapCmd.cc

libmagus_techmap_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_techmap/libym_techmap.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sbj/libym_sbj.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mvn/libym_mvn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_techmap_tcl_la_LDFLAGS =
