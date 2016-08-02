## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	lmtest

lmtest_SOURCES = \
	lmtest.cc

lmtest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_tgnet/libym_tgnet.la
