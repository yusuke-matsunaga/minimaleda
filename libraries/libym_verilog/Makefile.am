## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	ym_verilog \
	common \
	parser \
	elaborator \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/parser/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog/elaborator/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_verilog.la

libym_verilog_la_SOURCES = \
	VlMgr.cc

libym_verilog_la_LIBADD = \
	common/libym_vl_common.la \
	parser/libym_vl_parser.la \
	elaborator/libym_vl_elab.la

libym_verilog_la_LDFLAGS =
