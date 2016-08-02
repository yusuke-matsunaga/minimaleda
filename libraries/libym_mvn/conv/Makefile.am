## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mvn_conv.la

libym_mvn_conv_la_SOURCES = \
	MvNodeMap.cc \
	Mvn2Sbj.cc

libym_mvn_conv_la_LDFLAGS =

libym_mvn_conv_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sbj/libym_sbj.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

