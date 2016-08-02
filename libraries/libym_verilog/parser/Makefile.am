## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS = \
	include \
	main \
	scanner \
	pt_mgr \
	spt \
	cpt \
	.

INCLUDES =

CLEANFILES = 

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = \
	libym_vl_parser.la


############################################################
## libym_vl_parser.la
############################################################
libym_vl_parser_la_SOURCES =

libym_vl_parser_la_LIBADD = \
	main/libym_vl_parser_main.la \
	scanner/libym_vl_parser_scanner.la \
	pt_mgr/libym_vl_parser_pt_mgr.la \
	spt/libym_vl_parser_spt.la \
	cpt/libym_vl_parser_cpt.la \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la

libym_vl_parser_la_LDFLAGS =
