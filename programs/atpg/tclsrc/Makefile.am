## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2043 2009-02-21 07:08:48Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

TCLSH = ${top_builddir}/src/atpg_tclsh/atpg_tclsh
SETUP = ${top_srcdir}/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/tclsrc


EXTRA_DIST = \
	atpg_utils/pkgIndex.tcl \
	atpg_utils/scripts/*.tcl \
	atpg_utils/scripts/tclIndex

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{atpg_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(ATPG_LIBDIR) "{atpg_utils}"
