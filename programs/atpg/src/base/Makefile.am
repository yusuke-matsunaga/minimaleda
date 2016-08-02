## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_base.la

libatpg_base_la_SOURCES = \
	SaFault.cc \
	FaultMgr.cc \
	TestVector.cc \
	TvMgr.cc

libatpg_base_la_LDFLAGS =

libatpg_base_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la
