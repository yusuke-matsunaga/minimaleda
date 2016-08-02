## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2274 2009-06-10 07:45:29Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_equiv_tcl.la

libmagus_equiv_tcl_la_SOURCES = \
	EquivCmd.h \
	EquivCmd.cc

libmagus_equiv_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_cec/libym_cec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_equiv_tcl_la_LDFLAGS =
