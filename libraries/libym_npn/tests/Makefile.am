## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir) \
	$(TCL_INCLUDES) \
	@POPT_INCLUDES@

EXTRA_DIST = $(TESTS)

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

YM_BASE = $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

YM_TCLPP = $(YMTOOLS_BUILDDIR)/libraries/libym_tclpp/libym_tclpp.la 

YM_BDD = $(YMTOOLS_BUILDDIR)/libraries/libym_bdd/libym_bdd.la

YM_NPN = $(YMTOOLS_BUILDDIR)/libraries/libym_npn/libym_npn.la

#if BUILD_LIBYM_TCLPP
#NPN_CHECK = npn_check
#else
#NPN_CHECK =
#endif

noinst_PROGRAMS = \
	tvfunc_test \
	tvfunc_test2 \
	npn_check3 \
	$(NPN_CHECK)

tvfunc_test_SOURCES = \
	TvFuncTest.h \
	TvFuncTest.cc \
	tvfunc_test.cc
tvfunc_test_LDADD = \
	$(YM_NPN) \
	$(YM_BASE) \
	@POPT_LIBS@

tvfunc_test2_SOURCES = \
	tvfunc_test2.cc
tvfunc_test2_LDADD = \
	$(YM_NPN) \
	$(YM_BASE) \
	@POPT_LIBS@

npn_check3_SOURCES = \
	npn_check3.cc
npn_check3_LDADD = \
	$(YM_NPN) \
	$(YM_BDD) \
	$(YM_BASE) \
	@POPT_LIBS@

#npn_check_SOURCES = \
#	TvFuncTest.h \
#	TvFuncTest.cc \
#	TvFuncConv.h \
#	TvFuncConv.cc \
#	TvFuncTestCmd.h \
#	TvFuncTestCmd.cc \
#	TvFuncTimeCmd.h \
#	TvFuncTimeCmd.cc \
#	FuncCmd.h \
#	FuncCmd.cc \
#	npn_check.cc
#npn_check_LDADD = \
#	$(YM_NPN) \
#	$(YM_TCLPP) \
#	$(YM_BASE)
