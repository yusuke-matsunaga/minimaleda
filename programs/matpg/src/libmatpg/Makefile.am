## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1617 2008-07-27 16:56:12Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in


noinst_LTLIBRARIES = \
	libmatpg.la

libmatpg_la_SOURCES = \
	backtrace.cc \
	backtrace.h \
	calc_val.cc \
	cfna.cc \
	cfna.h \
	dtpg.cc \
	f_sim.cc \
	f_sim.h \
	fault.cc \
	fault.h \
	gate.cc \
	gate.h \
	imp_buf.cc \
	imp_other.cc \
	imp_sa.cc \
	imp_simple.cc \
	imp_xor.cc \
	learn.cc \
	learn.h \
	lvlq.cc \
	maqueue.cc \
	maqueue.h \
	matpg.h \
	network.cc \
	network.h \
	pgraph.cc \
	pgraph.h \
	rtpg.cc \
	sa_node.cc \
	sa_node.h \
	search_mgr.cc \
	search_mgr.h \
	sense.cc \
	sense.h \
	t_analyze.cc \
	t_analyze.h \
	testpat.cc \
	testpat.h \
	used_gate.cc \
	used_gate.h \
	val.h

libmatpg_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
