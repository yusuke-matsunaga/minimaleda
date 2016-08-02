## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 557 2007-04-09 05:05:15Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/magus_tclsh/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_test.la

libmagus_test_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la
libmagus_test_la_LDFLAGS = 
libmagus_test_la_SOURCES = \
	TestCmd.h \
	TestCmd.cc
