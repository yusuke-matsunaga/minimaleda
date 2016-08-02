## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(YMTOOLS_SRCDIR)/libraries/libym_verilog \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/scanner \
	-I$(top_builddir)/parser/main \
	@POPT_INCLUDES@

YFLAGS=-d -v

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	vltest


#############################################################
# vltest
#############################################################

vltest_SOURCES = \
	VlDumper.h \
	VlDumper.cc \
	VlDumperImpl.h \
	VlDumperImpl.cc \
	VlDumperImpl_scope.cc \
	VlDumperImpl_module.cc \
	VlDumperImpl_decl.cc \
	VlDumperImpl_item.cc \
	VlDumperImpl_stmt.cc \
	VlDumperImpl_expr.cc \
	VlTestLineWatcher.h \
	VlTestLineWatcher.cc \
	vltest_rawlex.cc \
	vltest_lex.cc \
	vltest_parse.cc \
	vltest_elaborate.cc \
	vltest_main.cc

vltest_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	@POPT_LIBS@
