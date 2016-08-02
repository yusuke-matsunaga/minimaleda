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
	libym_vl_elab_elb_mgr.la


############################################################
## libym_vl_elab_elb_mgr.la
############################################################
libym_vl_elab_elb_mgr_la_SOURCES = \
	AttrHash.cc \
	ElbDecl.cc \
	ElbExpr.cc \
	ElbMgr.cc \
	ElbParameter.cc \
	ElbPrimitive.cc \
	ElbStmt.cc \
	ElbUserSystf.cc \
	ModuleHash.cc \
	ObjDict.cc \
	TagDict.cc \
	VlNamedObj.cc


libym_vl_elab_elb_mgr_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_elab_elb_mgr_la_LDFLAGS =
