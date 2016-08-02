## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_blif \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_blif.la

libym_blif_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_blif_la_LDFLAGS = 

libym_blif_la_SOURCES = \
	BlifDic.h \
	BlifDic.cc \
	BlifScanner.h \
	BlifScanner.cc \
	IdHash.h \
	IdHash.cc \
	BlifParserImpl.h \
	BlifParserImpl.cc \
	BlifHandler.cc \
	BlifParser.cc \
	BlifNetwork.cc \
	BlifNetworkHandler.h \
	BlifNetworkHandler.cc \
	BlifNetworkReader.cc
