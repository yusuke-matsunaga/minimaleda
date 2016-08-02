## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include

YFLAGS=-d -v

CLEANFILES = \
	verilog_grammer.cc verilog_grammer.h y.output verilog_grammer.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_parser_main.la


############################################################
## libym_vl_parser_main.la
############################################################
libym_vl_parser_main_la_SOURCES = \
	verilog_grammer.yy \
	PuHierName.h \
	Parser_decl.h \
	Parser_expr.h \
	Parser_item.h \
	Parser_misc.h \
	Parser_port.h \
	Parser_stmt.h \
	Parser.cc \
	Parser_udp.cc \
	Parser_module.cc

libym_vl_parser_main_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_parser_main_la_LDFLAGS =
