## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS = \
	ym_bdd \
	base \
	bmc \
	bmm \
	dec \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd.la

libym_bdd_la_LIBADD = \
	base/libym_bdd_base.la \
	bmc/libym_bdd_bmc.la \
	bmm/libym_bdd_bmm.la \
	dec/libym_bdd_dec.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_bdd_la_LDFLAGS =

libym_bdd_la_SOURCES =

