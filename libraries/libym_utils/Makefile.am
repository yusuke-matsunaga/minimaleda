## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $


SUBDIRS = \
	. \
	ym_utils \
	tests

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_utils.la

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

libym_utils_la_SOURCES = \
	ItvlCell.h \
	StrPool.h \
	FragAlloc.cc \
	SimpleAlloc.cc \
	UnitAlloc.cc \
	Binder.cc \
	DlList.cc \
	File.cc \
	FileLoc.cc \
	MsgHandler.cc \
	Generator.cc \
	ItvlCell.cc \
	ItvlMgr.cc \
	MFSet.cc \
	NameMgr.cc \
	RefCount.cc \
	StopWatch.cc \
	StrBuff.cc \
	StrPool.cc \
	RandGen.cc \
	random.cc

libym_utils_la_LIBADD =

libym_utils_la_LDFLAGS =

