## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1978 2009-02-06 12:29:16Z matsunaga $

SUBDIRS =

INCLUDES = \
	$(TCL_INCLUDES) \
	$(YM_INCLUDES) \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libseal_io_cmds.la

libseal_io_cmds_la_SOURCES = \
	SealMsgHandler.h \
	SealMsgHandler.cc \
	NtwkIoCmd.h \
	NtwkIoCmd.cc

libseal_io_cmds_la_LDFLAGS =

libseal_io_cmds_la_LIBADD = \
	$(YM_LIBS_LIBYM_TGNET) \
	$(YM_LIBS_LIBYM_BLIF) \
	$(YM_LIBS_LIBYM_ISCAS89)
