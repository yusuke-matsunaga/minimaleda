## Process this file with automake to produce Makefile.in

## $Source: /export/home/project/synth/cvsrepos/ymtools/src/libym_bdd/npn/Makefile.am,v $
## $Author: matsunaga $
## $Revision: 1.3 $
## $Date: 2006/05/31 03:24:25 $

INCLUDES = \
	-I$(top_builddir) \
	-I$(top_srcdir) \
	$(GMP_INCLUDES)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_bdd_npn.la
libym_bdd_npn_la_LIBADD = \
	$(top_builddir)/src/libym_base/libym_base.la
libym_bdd_npn_la_LDFLAGS =
libym_bdd_npn_la_SOURCES = \
	WalshSig.cc \
	WalshSig.h \
	FuncEntry.h \
	RepInfo.cc \
	RepInfo.h \
	RepFunc.cc \
	RepFunc.h \
	FuncGroup.cc \
	FuncGroup.h \
	Matcher.cc \
	Matcher.h \
	NpnMap.cc \
	NpnMgr.cc
