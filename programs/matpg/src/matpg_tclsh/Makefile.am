## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1617 2008-07-27 16:56:12Z matsunaga $

SUBDIRS =

AM_CXXFLAGS = \
	-DMATPG_LIBDIR='"$(MATPG_LIBDIR)"' \
	-DMATPG_DATADIR='"$(MATPG_DATADIR)"' \
	-DMATPG_BINDIR='"$(MATPG_BINDIR)"'

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/src/libmatpg

MAINTAINERCLEANFILES = Makefile.in


noinst_LTLIBRARIES = \
	libmatpg_tcl.la

bin_PROGRAMS = \
	matpg_tclsh


libmatpg_tcl_la_SOURCES = \
	MatpgMsgHandler.h \
	MatpgMsgHandler.cc \
	ReadBlifCmd.h \
	ReadBlifCmd.cc \
	ReadIscas89Cmd.h \
	ReadIscas89Cmd.cc \
	SetFaultCmd.h \
	SetFaultCmd.cc \
	PrintFaultCmd.h \
	PrintFaultCmd.cc \
	PrintStatsCmd.h \
	PrintStatsCmd.cc \
	PrintPatCmd.h \
	PrintPatCmd.cc \
	RtpgCmd.h \
	RtpgCmd.cc \
	RfsimCmd.h \
	RfsimCmd.cc \
	DtpgCmd.h \
	DtpgCmd.cc \
	matpg_init.cc

libmatpg_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

matpg_tclsh_SOURCES = \
	tclAppInit.cc

matpg_tclsh_LDADD = \
	libmatpg_tcl.la \
	$(top_builddir)/src/libmatpg/libmatpg.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LADD)
