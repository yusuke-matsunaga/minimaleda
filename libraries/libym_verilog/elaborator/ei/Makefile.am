## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include \
	-I$(top_srcdir)/elaborator/include

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_elab_ei.la


############################################################
## libym_vl_elab_ei.la
############################################################
libym_vl_elab_ei_la_SOURCES = \
	EiFactory.h \
	EiFactory.cc \
	EiToplevel.h \
	EiToplevel.cc \
	EiUdp.h \
	EiUdp.cc \
	EiModule.h \
	EiModule.cc \
	EiPort.h \
	EiPort.cc \
	EiIODecl.h \
	EiIODecl.cc \
	EiDecl.h \
	EiDecl.cc \
	EiDeclHead.h \
	EiDeclHead.cc \
	EiDeclArray.h \
	EiDeclArray.cc \
	EiDeclElem.h \
	EiDeclElem.cc \
	EiParameter.h \
	EiParameter.cc \
	EiImpNet.h \
	EiImpNet.cc \
	EiGenvar.h \
	EiGenvar.cc \
	EiContAssign.h \
	EiContAssign.cc \
	EiProcess.h \
	EiProcess.cc \
	EiParamAssign.h \
	EiParamAssign.cc \
	EiPrimitive.h \
	EiPrimitive.cc \
	EiTaskFunc.h \
	EiTaskFunc.cc \
	EiGenBlock.h \
	EiGenBlock.cc \
	EiStmt.h \
	EiStmt.cc \
	EiAssignment.h \
	EiAssignment.cc \
	EiBlockScope.h \
	EiBlockScope.cc \
	EiBlockStmt.h \
	EiBlockStmt.cc \
	EiCondStmt.h \
	EiCondStmt.cc \
	EiMiscStmt.h \
	EiMiscStmt.cc \
	EiControl.h \
	EiControl.cc \
	EiDelay.h \
	EiDelay.cc \
	EiRange.h \
	EiRange.cc \
	EiExpr.h \
	EiExpr.cc \
	EiOperation.h \
	EiUnaryOp.h \
	EiUnaryOp.cc \
	EiBinaryOp.h \
	EiBinaryOp.cc \
	EiTernaryOp.h \
	EiTernaryOp.cc \
	EiConcatOp.h \
	EiConcatOp.cc \
	EiPrimary.h \
	EiPrimary.cc \
	EiBitSelect.h \
	EiBitSelect.cc \
	EiPartSelect.h \
	EiPartSelect.cc \
	EiVarPartSelect.h \
	EiVarPartSelect.cc \
	EiFuncCall.h \
	EiFuncCall.cc \
	EiConstant.h \
	EiConstant.cc \
	EiAttribute.h \
	EiAttribute.cc


libym_vl_elab_ei_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_elab_ei_la_LDFLAGS =
