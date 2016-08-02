## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 169 2006-08-22 09:17:46Z matsunaga $

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/libraries/libym_verilog

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_vl_sim.la

libym_vl_sim_la_LIBADD =
libym_vl_sim_la_LDFLAGS =
libym_vl_sim_la_SOURCES = \
	SimEngine.h \
	SimEventQueue.h \
	SimCaLinkQueue.h \
	SimNode.h \
	SimDelayNode.h \
	SimEventNode.h \
	SimRepeatNode.h \
	SimForkNode.h \
	SimPcaNode.h \
	SimCaLink.h \
	SimScope.h \
	SimStmtGraph.h \
	SimEngine.cc \
	SimNode.cc \
	SimDelayNode.cc \
	SimEventNode.cc \
	SimRepeatNode.cc \
	SimForkNode.cc \
	SimPcaNode.cc \
	SimCaLink.cc \
	SimScope.cc \
	SimStmtGraph.cc