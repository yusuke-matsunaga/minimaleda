## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libseal_bddsim.la

libseal_bddsim_la_SOURCES = \
	BddsimCmd.h \
	BddsimCmd.cc \
	EventQ.h \
	EventQ.cc \
	SimNode.h \
	SimNode.cc \
	bddsim_init.cc \
	nsdef.h 

libseal_bddsim_la_LDFLAGS =

libseal_bddsim_la_LIBADD = \
	$(YM_LIBS_LIBYM_BDD)