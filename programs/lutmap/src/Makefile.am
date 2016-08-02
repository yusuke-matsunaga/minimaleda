## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

AM_CXXFLAGS = \
	-DLUTMAP_LIBDIR='"$(LUTMAP_LIBDIR)"' \
	-DLUTMAP_DATADIR='"$(LUTMAP_DATADIR)"' \
	-DLUTMAP_BINDIR='"$(LUTMAP_BINDIR)"'

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	lutmap

lutmap_SOURCES = \
	lutmap.cc

lutmap_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lutmap/libym_lutmap.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sbj/libym_sbj.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mvn/libym_mvn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
