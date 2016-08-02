## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS = 

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_dtpg_sat.la

libatpg_dtpg_sat_la_SOURCES = \
	DtpgSat.h \
	DtpgSat.cc \
	tgnet2aig.cc

libatpg_dtpg_sat_la_LDFLAGS =

libatpg_dtpg_sat_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_aig/libym_aig.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sat/libym_sat.la
