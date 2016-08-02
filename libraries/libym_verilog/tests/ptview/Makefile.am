## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

include $(top_srcdir)/../../etc/autotroll.mk

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/parser/scanner \
	-I$(top_builddir)/parser/main

EXTRA_DIST =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
	ptview

ptview_SOURCES = \
	VerilogView.h \
	VerilogView.cc \
	VerilogSyntaxHighlighter.h \
	VerilogSyntaxHighlighter.cc \
	TokenListModel.h \
	TokenListModel.cc \
	TokenView.h \
	TokenView.cc \
	ParseTreeModel.h \
	ParseTreeModel.cc \
	ParseTreeView.h \
	ParseTreeView.cc \
	PtNode.h \
	PtNode.cc \
	PtNode_root.h \
	PtNode_root.cc \
	PtNode_udp.h \
	PtNode_udp.cc \
	PtNode_module.h \
	PtNode_module.cc \
	PtNode_port.h \
	PtNode_port.cc \
	PtNode_decl.h \
	PtNode_decl.cc \
	PtNode_item.h \
	PtNode_item.cc \
	PtNode_stmt.h \
	PtNode_stmt.cc \
	PtNode_expr.h \
	PtNode_expr.cc \
	PtNode_misc.h \
	PtNode_misc.cc \
	GoToLine.h \
	GoToLine.cc \
	ptview_main.cc

nodist_ptview_SOURCES = \
	VerilogView.moc.cc \
	TokenView.moc.cc \
	ParseTreeView.moc.cc \
	GoToLine.moc.cc

ptview_CXXFLAGS = $(QT_CXXFLAGS) $(AM_CXXFLAGS)

ptview_CPPFLAGS = $(QT_CPPFLAGS) $(AM_CPPFLAGS)

ptview_LDFLAGS  = $(QT_LDFLAGS) $(LDFLAGS)

ptview_LDADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_verilog/libym_verilog.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la \
	$(QT_LIBS) \
	$(LIDADD)

BUILT_SOURCES = \
	VerilogView.moc.cc \
	TokenView.moc.cc \
	ParseTreeView.moc.cc \
	GoToLine.moc.cc

