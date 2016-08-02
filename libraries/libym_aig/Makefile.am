## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_aig/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.2 $
## $Date: 2006/02/21 18:04:13 $

SUBDIRS = \
	ym_aig \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_aig.la

libym_aig_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_sat/libym_sat.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_npn/libym_npn.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_aig_la_LDFLAGS = 

libym_aig_la_SOURCES = \
	AigNode.cc \
	AigMgr.cc \
	AigMgrImpl.h \
	AigMgrImpl.cc \
	AigTemplate.h \
	AigTemplate.cc
