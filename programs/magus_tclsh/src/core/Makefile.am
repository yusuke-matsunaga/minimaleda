## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	$(TCL_INCLUDES) \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libmagus_core_tcl.la

libmagus_core_tcl_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_mvn/libym_mvn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bdn/libym_bdn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_bnet/libym_bnet.la
libmagus_core_tcl_la_LDFLAGS =
libmagus_core_tcl_la_SOURCES = \
	MagMgr.cc \
	NetHandle.cc \
	BNetHandle.h \
	BNetHandle.cc \
	BdnHandle.h \
	BdnHandle.cc \
	MvnHandle.h \
	MvnHandle.cc \
	MagCmd.cc \
	NewBNetwork.h \
	NewBNetwork.cc \
	NewBdn.h \
	NewBdn.cc \
	NewMvn.h \
	NewMvn.cc \
	ClrNetwork.h \
	ClrNetwork.cc \
	DelNetwork.h \
	DelNetwork.cc \
	CopyNetwork.h \
	CopyNetwork.cc \
	ChgNetwork.h \
	ChgNetwork.cc \
	PushNetwork.h \
	PushNetwork.cc \
	PopNetwork.h \
	PopNetwork.cc \
	CurNetwork.h \
	CurNetwork.cc \
	ListNetwork.h \
	ListNetwork.cc \
	ForNetwork.h \
	ForNetwork.cc \
	core_init.cc
