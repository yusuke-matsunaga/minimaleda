## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_simplify.la

libmagus_simplify_la_SOURCES = \
	simplify.h \
	LrwCmd.h \
	LrwCmd.cc
	simplify_init.cc

libmagus_simplify_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la

libmagus_simplify_la_LDFLAGS =
