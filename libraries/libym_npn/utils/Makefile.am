## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 33 2006-07-16 14:58:17Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

YM_BASE = $(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

noinst_PROGRAMS = \
	symtablegen \
	tblgen \
	npn_tablegen \
	mpx

symtablegen_SOURCES = \
	symtablegen.cc

tblgen_SOURCES = \
	tblgen.cc

npn_tablegen_SOURCES = \
	npn_tablegen.cc
npn_tablegen_LDADD = \
	$(YM_BASE)

mpx_SOURCES = \
	mpx.cc
mpx_LDADD =
