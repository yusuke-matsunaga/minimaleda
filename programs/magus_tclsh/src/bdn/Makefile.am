## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_logbase_tcl.la

libmagus_logbase_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdn/libym_bdn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la
libmagus_logbase_tcl_la_LDFLAGS =
libmagus_logbase_tcl_la_SOURCES = \
	MvnReadVerilog.h \
	MvnReadVerilog.cc \
	logbase_init.cc
