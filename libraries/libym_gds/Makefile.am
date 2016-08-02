## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 997 2007-09-07 09:58:29Z matsunaga $

SUBDIRS = \
	ym_gds \
	. \
	tests

INCLUDES = \
	-I$(YMTOOLS_SRCDIR)/include \
	-I$(YMTOOLS_BUILDDIR)/include

YFLAGS = -d -v

CLEANFILES = \
	GdsParser.cc GdsParser.h y.output GdsParser.output

MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libym_gds.la

libym_gds_la_LIBADD = \
	$(YMTOOLS_BUILDDIR)/libraries/libym_utils/libym_utils.la
libym_gds_la_LDFLAGS = 
libym_gds_la_SOURCES = \
	Msg.cc \
	GdsParser.yy \
	GdsRecTable.h \
	GdsRecTable.cc \
	GdsRecMgr.cc \
	GdsRecord.cc \
	GdsData.h \
	GdsData.cc \
	GdsScanner.cc \
	GdsDumper.cc



