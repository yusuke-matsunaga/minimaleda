## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_blifbnetconv \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_blifbnetconv.la

libym_blifbnetconv_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_blifbnetconv_la_LDFLAGS = 

libym_blifbnetconv_la_SOURCES = \
	BlifBNetConv.cc
