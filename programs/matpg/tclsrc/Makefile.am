## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2043 2009-02-21 07:08:48Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

TCLSH = ${top_builddir}/src/matpg_tclsh/matpg_tclsh
SETUP = ${top_srcdir}/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/tclsrc


EXTRA_DIST = \
	matpg_utils/pkgIndex.tcl \
	matpg_utils/scripts/*.tcl \
	matpg_utils/scripts/tclIndex

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{matpg_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(MATPG_LIBDIR) "{matpg_utils}"
