## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 467 2007-03-22 07:54:05Z matsunaga $

SUBDIRS = \
	. \
	ym_zstream \
	tests

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

noinst_LTLIBRARIES = libym_zstream.la

libym_zstream_la_LIBADD = 
libym_zstream_la_LDFLAGS = -lz
libym_zstream_la_SOURCES = \
	zstream.cc
