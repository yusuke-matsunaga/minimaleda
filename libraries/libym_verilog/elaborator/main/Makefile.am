## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/parser/include \
	-I$(top_srcdir)/elaborator/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_elab_main.la


############################################################
## libym_vl_elab_main.la
############################################################
libym_vl_elab_main_la_SOURCES = \
	CfDict.cc \
	AttrDict.cc \
	ElbEnv.h \
	ElbEnv.cc \
	ElbProxy.h \
	ElbProxy.cc \
	ElbParamCon.h \
	ElbValue.h \
	UdpGen.h \
	UdpGen.cc \
	ModuleGen.h \
	ModuleGen.cc \
	DeclGen.h \
	DeclGen.cc \
	ItemGen.h \
	ItemGen_main.cc \
	ItemGen_taskfunc.cc \
	ItemGen_module_inst.cc \
	ItemGen_primitive.cc \
	StmtGen.h \
	StmtGen_main.cc \
	StmtGen_assign.cc \
	StmtGen_block.cc \
	StmtGen_cond.cc \
	ExprGen.h \
	ExprGen_main.cc \
	ExprGen_operation.cc \
	ExprGen_primary.cc \
	ExprGen_funccall.cc \
	ExprGen_error.cc \
	AttrGen.h \
	AttrGen.cc \
	Elaborator.cc


libym_vl_elab_main_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_elab_main_la_LDFLAGS =
