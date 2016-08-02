## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 558 2007-04-09 05:54:44Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(TK_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_misc_tcl.la

libmagus_misc_tcl_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	misc_init.cc

libmagus_misc_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

