## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2275 2009-06-10 10:56:15Z matsunaga $


SUBDIRS = \
	include \
	libraries \
	programs

CLEANFILES = doxygen.warnings

MAINTAINERCLEANFILES = Makefile.in

EXTRA_DIST = \
	BUILDING \
	Doxyfile.in \
	aclocal.m4 \
	boot.sh \
	configure \
	configure.ac \
	etc/autogen.tmpl \
	etc/autotroll.mk \
	etc/common_defs \
	etc/mk.subr \
	m4macros/autotroll.m4 \
	m4macros/gmp.m4 
	m4macros/popt.m4 \
	m4macros/qscintilla2.m4 \
	m4macros/tcl.m4 \
	m4macros/ymlibs.m4 \
	mkautogen


dox:
	@DOXYGEN@



