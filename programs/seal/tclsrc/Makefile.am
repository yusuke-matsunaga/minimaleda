## Process this file with automake to produce Makefile.in

## $Id: Makefile.am 2507 2009-10-17 16:24:02Z matsunaga $

SUBDIRS =

MAINTAINERCLEANFILES = Makefile.in

TCLSH = ${top_builddir}/src/seal_tclsh
SETUP = ${top_srcdir}/utils/tclpkg-setup.tcl
INSTALL = ${top_srcdir}/utils/tclpkg-install.tcl
PKGSRC = ${top_srcdir}/tclsrc


EXTRA_DIST = \
	seal_utils/pkgIndex.tcl \
	seal_utils/scripts/*.tcl \
	seal_utils/scripts/tclIndex

all-local :
	$(TCLSH) $(SETUP) $(PKGSRC) "{seal_utils}"

install-data-local :
	$(TCLSH) $(INSTALL) $(PKGSRC) $(SEAL_LIBDIR) "{seal_utils}"
