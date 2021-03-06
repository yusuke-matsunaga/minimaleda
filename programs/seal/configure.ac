# Process this file with autoconf to produce a configure script
dnl
# $Id: configure.ac 2507 2009-10-17 16:24:02Z matsunaga $

# ===================================================================
# Initial block
# ===================================================================
AC_REVISION($Rev: 2507 $)dnl

AC_INIT([seal], [1.0],
        [yusuke_matsunaga@ieee.org],
        [seal])dnl

AC_PREREQ(2.57)dnl

AC_COPYRIGHT([
Copyright (C) 2006-2008 Yusuke Matsunaga
All rights reserved.
])dnl

AC_CONFIG_SRCDIR([include/seal_nsdef.h])dnl

AC_CONFIG_AUX_DIR([config])dnl
AC_CONFIG_MACRO_DIR([config])dnl
AC_SUBST(ac_aux_dir)dnl

AM_INIT_AUTOMAKE()dnl

AC_DEFINE_UNQUOTED(SEAL_VERSION, "${VERSION}", [SEAL version])dnl


# ===================================================================
# Checks for programs
# ===================================================================
AC_ISC_POSIX
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_LIBTOOL
AC_CHECK_PROG([DOXYGEN],
	      [doxygen],
	      [doxygen Doxyfile],
	      [@echo \"You'll need to install 'doxygen'\"])
AC_SUBST(DOXYGEN)

if test $enable_shared = no; then
  AC_DEFINE(BUILD_STATIC, 1, [Define for doing static link])
  AM_LDFLAGS="-static"
  AC_SUBST(AM_LDFLAGS)
fi


# ===================================================================
# Checks for libraries
# ===================================================================


# ===================================================================
# Checks for header files
# ===================================================================


# ===================================================================
# Checks for typedefs, structures, and compiler characteristics
# ===================================================================


# ===================================================================
# Check for size of built-in types
# ===================================================================
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(unsigned char)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(unsigned int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(void*)
AC_CHECK_SIZEOF(size_t)

_AC_COMPUTE_INT([__alignof__ (double)], ym_alignof_double)
AC_DEFINE_UNQUOTED(ALIGNOF_DOUBLE, $ym_alignof_double, [Alignment of double])


# ===================================================================
# Check for return type of signal handler
# ===================================================================


# ===================================================================
# Checks for library functions
# ===================================================================


# ===================================================================
# Check for gmp
# ===================================================================
YM_CHECK_GMP


# ===================================================================
# Check for popt
# ===================================================================
YM_CHECK_POPT


# ===================================================================
# Check for TCL environment
# ===================================================================
YM_CHECK_TCL


# ===================================================================
# Check for tclreadline
# ===================================================================
YM_CHECK_TCLREADLINE


# ===================================================================
# Check for C++ language
# ===================================================================
AC_LANG_CPLUSPLUS


# ===================================================================
# Set 'seal' directory
# ===================================================================
AC_ARG_WITH(seal-library,
[  --with-seal-library=DIR package dependent directory],
[SEAL_LIBDIR=$withval],
[SEAL_LIBDIR='${prefix}/lib/AC_PACKAGE_NAME[]AC_PACKAGE_VERSION'])dnl
AC_SUBST(SEAL_LIBDIR)dnl

SEAL_DATADIR='${prefix}/share/AC_PACKAGE_NAME[]AC_PACKAGE_VERSION'
AC_SUBST(SEAL_DATADIR)dnl

SEAL_BINDIR='${bindir}'
AC_SUBST(SEAL_BINDIR)dnl


# ===================================================================
# Declare files to be generated by configure script
# ===================================================================
AC_CONFIG_FILES([
	Makefile
	include/Makefile
	src/Makefile
	src/testvect/Makefile
	src/bddsim/Makefile
	src/misc/Makefile
	src/io_cmds/Makefile
	src/calc_cvf/Makefile
	src/calc_svf/Makefile
	tclsrc/Makefile
	])


# ===================================================================
# Generate configured files
# ===================================================================
AC_OUTPUT

# End-Of-File
