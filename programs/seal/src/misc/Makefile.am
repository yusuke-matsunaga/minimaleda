## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libseal_misc.la

libseal_misc_la_SOURCES = \
	TimeCmd.h \
	TimeCmd.cc \
	misc_init.cc
libseal_misc_la_LIBADD = \
	$(YM_LIBS_LIBYM_TCLPP)
