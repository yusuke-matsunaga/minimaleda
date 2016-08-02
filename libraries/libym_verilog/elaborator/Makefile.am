## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	include \
	main \
	elb_mgr \
	ei \
	.

INCLUDES =

CLEANFILES =

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_elab.la


############################################################
## libym_vl_elab.la
############################################################
libym_vl_elab_la_SOURCES =

libym_vl_elab_la_LIBADD = \
	main/libym_vl_elab_main.la \
	elb_mgr/libym_vl_elab_elb_mgr.la \
	ei/libym_vl_elab_ei.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_elab_la_LDFLAGS =
