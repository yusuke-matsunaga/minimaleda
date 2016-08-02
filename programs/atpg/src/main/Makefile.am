## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libatpg_main.la

libatpg_main_la_SOURCES = \
	AtpgMgr.cc \
	Rtpg.cc \
	Dtpg.cc \
	TvBuff.cc

libatpg_main_la_LDFLAGS =

libatpg_main_la_LIBADD = \
	$(top_builddir)/src/dtpg_classic/libatpg_dtpg_cl.la \
	$(top_builddir)/src/dtpg_sat/libatpg_dtpg_sat.la \
	$(top_builddir)/src/fsim/libatpg_fsim.la \
	$(top_builddir)/src/fsim3/libatpg_fsim3.la \
	$(top_builddir)/src/fsimx/libatpg_fsimx.la \
	$(top_builddir)/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la
