## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_mincov \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_mincov.la

libym_mincov_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_mincov_la_LDFLAGS = 

libym_mincov_la_SOURCES = \
	MincovCost.cc \
	MincovMatrix.cc \
	MaxClique.h \
	MaxClique.cc \
	MincovSolver.cc
