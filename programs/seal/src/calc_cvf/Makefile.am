## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libseal_calc_cvf.la

libseal_calc_cvf_la_SOURCES = \
	nsdef.h \
	EqElem.h \
	PoMark.h \
	PoMark.cc \
	NodeSet.h \
	SimNode.h \
	SimNode.cc \
	SnGate.h \
	SnGate.cc \
	SnInput.h \
	SnInput.cc \
	SnAnd.h \
	SnAnd.cc \
	SnOr.h \
	SnOr.cc \
	SnXor.h \
	SnXor.cc \
	SimFFR.h \
	SimFFR.cc \
	EventQ.h \
	EventQ.cc \
	CalcCvf.h \
	CalcCvf.cc \
	CvfCmd.h \
	CvfCmd.cc

libseal_calc_cvf_la_LDFLAGS =

libseal_calc_cvf_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_LEXP) \
	$(YM_LIBS_LIBYM_TCLPP) \
	$(YM_LIBS_LIBYM_UTILS)
