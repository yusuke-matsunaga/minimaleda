## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 697 2007-05-28 17:10:53Z matsunaga $

INCLUDES = \
	$(GTKGLEXTMM_CXXFLAGS) \
	$(YM_INCLUDES) \
	@POPT_INCLUDES@

EXTRA_DIST = $(TESTS)

MAINTAINERCLEANFILES = Makefile.in

TESTS_ENVIRONMENT = \
	top_builddir=$(top_builddir) \
	top_srcdir=$(top_srcdir)

TESTS =

testsubdir = testSubDir

distclean-local:
	-rm -rf $(testsubdir)

bin_PROGRAMS = \
	gears \
	led

gears_SOURCES = \
	GlvScene.h \
	GlvScene.cc \
	gears.cc


gears_LDADD = \
	$(GTKGLEXTMM_LIBS) -lpthread

led_SOURCES = \
	GlvScene.h \
	GlvScene.cc \
	LedScene.h \
	LedScene.cc \
	GateTemplate.h \
	GateObj.h \
	GateObj.cc \
	led.cc


led_LDADD = \
	$(YM_LIBS_LIBYM_UTILS) \
	$(GTKGLEXTMM_LIBS) -lpthread
