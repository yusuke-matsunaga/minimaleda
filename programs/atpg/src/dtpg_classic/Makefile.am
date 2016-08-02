## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2203 2009-04-16 05:04:40Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCLRL_INCLUDES) \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in


noinst_LTLIBRARIES = \
	libatpg_dtpg_cl.la

libatpg_dtpg_cl_la_SOURCES = \
	dtpg_opt.h \
	LqElem.h \
	Gate.h \
	Gate.cc \
	GateConst.h \
	GateConst.cc \
	GateInput.h \
	GateInput.cc \
	GateBuf.h \
	GateBuf.cc \
	GateAnd.h \
	GateAnd.cc \
	GateOr.h \
	GateOr.cc \
	GateXor.h \
	GateXor.cc \
	GateSa.h \
	GateSa.cc \
	Network.h \
	Network.cc \
	t_analyze.cc \
	LvlQ.h \
	LvlQ.cc \
	Cfna.h \
	Cfna.cc \
	backtrace.cc \
	backtrace.h \
	PGraph.cc \
	PGraph.h \
	sense.h \
	sense.cc \
	SearchMgr.h \
	SearchMgr.cc \
	DtpgCl.h \
	DtpgCl.cc

libatpg_dtpg_cl_la_LIBADD = \
	$(top_builddir)/src/base/libatpg_base.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
