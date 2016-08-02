## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 1920 2008-12-20 15:52:42Z matsunaga $

SUBDIRS = \
	ym_tgnet \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_tgnet.la

libym_tgnet_la_SOURCES = \
	NameHash.h \
	NameHash.cc \
	LogicMgr.h \
	LogicMgr.cc \
	TgNetwork.cc \
	TgBlifHandler.h \
	TgBlifHandler.cc \
	TgBlifReader.cc \
	TgIscas89Handler.h \
	TgIscas89Handler.cc \
	TgIscas89Reader.cc

libym_tgnet_la_LDFLAGS =

libym_tgnet_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_blif/libym_blif.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_iscas89/libym_iscas89.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_lexp/libym_lexp.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

