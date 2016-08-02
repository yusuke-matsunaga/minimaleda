## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	$(TCLRL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_PROGRAMS = \
	ym_tclsh

ym_tclsh_SOURCES = \
	tclAppInit.cc \
	HelloCmd.h \
	HelloCmd.cc \
	RandCmd.h \
	RandCmd.cc \
	TimeCmd.h \
	TimeCmd.cc \
	OptTestCmd.h \
	OptTestCmd.cc


ym_tclsh_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(YM_LIBS_LIBTCLREADLINE) \
	$(TCL_LDADD)
