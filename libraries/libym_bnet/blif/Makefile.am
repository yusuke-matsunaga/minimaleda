## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bnetblifreader.la

libym_bnetblifreader_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la

libym_bnetblifreader_la_LDFLAGS = 

libym_bnetblifreader_la_SOURCES = \
	BNetBlifHandler.h \
	BNetBlifHandler.cc \
	BNetBlifReader.cc
