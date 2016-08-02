# Macros for building Magus's configure
#
# $Id: tcl.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

# =====================================================================
# YM_CHECK_TCL
# ym_have_tcl holds the status (yes/no)
# if found, the following shell variables are defined
# TCL_MAJOR_VERSION
# TCL_MINOR_VERSION
# TCL_LDADD
# TCL_RDIR
# TCL_INCLUDES
#
# Also, the following C-macros are defined
# TCL_MAJOR_VERSION
# TCL_MINOR_VERSION
# HAVE_TCL
# =====================================================================
# Checks for tcl/tk environment
AC_DEFUN([YM_CHECK_TCL],[
# Checks for Tcl
ym_tmp_lib_list="lib"
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib_list="$ym_tmp_lib_list lib64"
fi
ym_tmp_path=""
for ym_tmp_dir in "$prefix $ac_default_prefix /usr /usr/local"; do
  for ym_tmp_lib in $ym_tmp_lib_list; do
    ym_tmp_path="$ym_tmp_path $ym_tmp_dir/$ym_tmp_lib"
  done
done
YM_CHECK_TCLLIB(tcl, $ym_tmp_path,
                [tcl8.5 tcl8.4 tcl8.3 tcl8.2 tcl8.1 tcl8.0 tcl8.0jp tcl],
                TCL_LIB_SPEC, TCL_MAJOR_VERSION, TCL_MINOR_VERSION)
if test $ym_tmp_found = no; then
#  AC_MSG_ERROR([Tcl is not found])
  ym_have_tcl=no
else
  ym_tmp_path="$ym_tmp_path $ym_tmp_dir"
  TCL_LDADD="$ym_tmp_ldadd $TCL_LIBS"
  TCL_RDIR=`echo $TCL_LIB_SPEC | sed -n -e 's/-L\(.*\) -l.*/\1/p'`
  TCL_MAJOR_VERSION=$ym_tmp_maj
  TCL_MINOR_VERSION=$ym_tmp_min
  AC_DEFINE_UNQUOTED(TCL_MAJOR_VERSION, $ym_tmp_maj, [Tcl major version])
  AC_DEFINE_UNQUOTED(TCL_MINOR_VERSION, $ym_tmp_min, [Tcl minor version])
  ym_tcl_ldadd=$ym_tmp_ldadd
  YM_CHECK_TCLINC(tcl, $TCL_INCLUDE_SPEC, $TCL_PREFIX, $TCL_VERSION)
  if test $ym_tmp_found = no; then
#  AC_MSG_ERROR([Tcl is not found])
    ym_have_tcl=no
  else
    ym_have_tcl=yes
    TCL_INCLUDES=$ym_tmp_incdir
    AC_DEFINE([HAVE_TCL], 1, [Define if you have Tcl library and headers])
  fi
fi

# Substitute output variables
AC_SUBST(TCL_LDADD)dnl
AC_SUBST(TCL_RDIR)dnl
AC_SUBST(TCL_INCLUDES)dnl
AC_SUBST(TCL_MAJOR_VERSION)dnl
AC_SUBST(TCL_MINOR_VERSION)dnl
])

# =====================================================================
# YM_CHECK_TK
# ym_have_tk holds the status (yes/no)
# if found, the following shell variables are defined
# TK_MAJOR_VERSION
# TK_MINOR_VERSION
# TK_LDADD
# TK_RDIR
# TK_INCLUDES
#
# Also, the following C-macros are defined
# TK_MAJOR_VERSION
# TK_MINOR_VERSION
# HAVE_TK
# =====================================================================
# Checks for tcl/tk environment
AC_DEFUN([YM_CHECK_TK],[
# Checks for Tcl
YM_CHECK_TCL
ym_tmp_lib_list="lib"
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib_list="$ym_tmp_lib_list lib64"
fi
ym_tmp_path=""
for ym_tmp_dir in "$prefix $ac_default_prefix /usr /usr/local"; do
  for ym_tmp_lib in $ym_tmp_lib_list; do
    ym_tmp_path="$ym_tmp_path $ym_tmp_dir/$ym_tmp_lib"
  done
done

ym_old_cppflags=$CPPFLAGS
CPPFLAGS="$CPPFLAGS $TCL_INCLUDES"

# Checks for Tk
ym_have_tk=no
YM_CHECK_TCLLIB(tk, $ym_tmp_path,
                [tk8.5 tk8.4 tk8.3 tk8.2 tk8.1 tk8.0 tk8.0jp tk],
                TK_LIB_SPEC, TK_MAJOR_VERSION, TK_MINOR_VERSION)
if test $ym_tmp_found = yes; then
  TK_LDADD="$ym_tmp_ldadd $ym_tcl_ldadd $TK_LIBS"
  TK_RDIR=`echo $TK_LIB_SPEC | sed -n -e 's/-L\(.*\) -l.*/\1/p'`
  TK_MAJOR_VERSION=$ym_tmp_maj
  TK_MINOR_VERSION=$ym_tmp_min
  AC_DEFINE_UNQUOTED(TK_MAJOR_VERSION, $ym_tmp_maj, [Tk major version])
  AC_DEFINE_UNQUOTED(TK_MINOR_VERSION, $ym_tmp_min, [Tk minor version])
  ym_tmp_path="$ym_tmp_path $ym_tmp_dir"
  YM_CHECK_TCLINC(tk, $TK_INCLUDE_SPEC, $TK_PREFIX, $TK_VERSION)
  if test $ym_tmp_found = yes; then
    ym_have_tk=yes
    AC_PATH_X
    if test "X$x_includes" != "X"; then
      ym_tmp_x_includes=-I$x_includes
    else
      ym_tmp_x_includes=
    fi
    TK_INCLUDES="$ym_tmp_incdir $ym_tmp_x_includes $TCL_INCLUDES"
    AC_DEFINE(HAVE_TK, 1, [Define if you have Tk library and headers])
    CPPFLAGS="$CPPFLAGS $TK_INCLUDES"
  fi
fi

CPPFLAGS=$ym_old_cppflags

# Substitute output variables
AC_SUBST(TK_LDADD)dnl
AC_SUBST(TK_RDIR)dnl
AC_SUBST(TK_INCLUDES)dnl
AC_SUBST(TK_MAJOR_VERSION)dnl
AC_SUBST(TK_MINOR_VERSION)dnl
])

# ================================================================
# YM_CHECK_TCLLIB(pkg, dirlist, subdirlist, libvar, libpath,
#                  major, minor)
# ================================================================
# Checks for tcl/tk/itcl/itk libraries
AC_DEFUN([YM_CHECK_TCLLIB],[
AC_MSG_CHECKING([for $1 library])
ym_tmp_found=no
ym_tmp_dir=NONE
AC_ARG_WITH($1,
            AS_HELP_STRING([--with-$1=DIR], [$1Config.sh is in DIR]),
            [
if test "x$withval" != x && test -r $withval/$1Config.sh; then
# file exists
  ym_tmp_dir=$withval
  ym_tmp_found=yes
fi],
[
dirlist="$2"
subdirlist="$3"
for dir in $dirlist
do
  if test -r $dir/$1Config.sh; then
    ym_tmp_dir=$dir
    ym_tmp_found=yes
  else
    for subdir in $subdirlist
    do
      if test -r $dir/$subdir/$1Config.sh; then
        ym_tmp_dir=$dir/$subdir
        ym_tmp_found=yes
        break
      fi
    done
  fi
  if test $ym_tmp_found = yes; then break; fi
done])

if test $ym_tmp_found = yes; then
# read config file
  . $ym_tmp_dir/$1Config.sh
  ym_tmp_ldadd=`eval "echo ${$4}"`
  ym_tmp_maj=`eval "echo ${$5}"`
  ym_tmp_min=`eval "echo ${$6}"`
  AC_MSG_RESULT([$ym_tmp_ldadd])
else
  AC_MSG_RESULT([not found])
fi
])

# ========================================================
# YM_CHECK_TCLINC(pkg, include_spec, prefix, version)
# ========================================================
AC_DEFUN([YM_CHECK_TCLINC],[
AC_MSG_CHECKING([for $1 headers])
ym_tmp_found=no
ym_tmp_dir=NONE
AC_ARG_WITH($1-includes,
            AS_HELP_STRING([--with-$1-includes=DIR], [$1.h is in DIR]),
            [
if test "x$withval" != x && test -r $withval/$1.h; then
# found
  ym_tmp_incdir="-I$withval"
  ym_tmp_found=yes
fi],)
if test $ym_tmp_found = no; then
# try to check using TCL_INCLUDE_SPEC
  ym_tmp_dir=`echo $2 | sed -n -e 's/-I\(.*\)/\1/p'`
  if test -r $ym_tmp_dir/$1.h; then
#   found
    ym_tmp_incdir="-I$ym_tmp_dir"
    ym_tmp_found=yes
  fi
fi
if test $ym_tmp_found = no; then
# First, check if cpp can find the header without '-I' option.
  AC_TRY_CPP([#include "$1.h"],
  [
  ym_tmp_incdir=""
  ym_tmp_found=yes
  ],[
  for ym_tmp_dir in \
    $3/include \
    $3/include/$1 \
    $3/include/$1$4 \
    $3/include/generic \
    $3/include/$1/generic \
    $3/include/$1$4/generic; do
    if test -r $ym_tmp_dir/$1.h; then
#     found
      ym_tmp_incdir="-I$ym_tmp_dir"
      ym_tmp_found=yes
      break
    fi
  done
  ])
fi

if test $ym_tmp_found = yes; then
  AC_MSG_RESULT([$ym_tmp_incdir])
else
  AC_MSG_RESULT([not found])
fi
])

# ========================================================
# YM_CHECK_TCLREADLINE
# ========================================================
AC_DEFUN([YM_CHECK_TCLREADLINE],[
AC_MSG_CHECKING([for tclreadline])
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib="lib64"
else
  ym_tmp_lib="lib"
fi
ym_tmp_found=no
ym_tmp_builtin_tclreadline=no
AC_ARG_WITH([tclreadline],
	AS_HELP_STRING([--with-tclreadline=DIR], [libtclreadline.la is in DIR]),
	[
if test "x$withval" != x; then
  if test $withval = builtin && test -r ../../tclreadline/libtclreadline.la; then
    ym_tmp_found=yes
    YM_LIBS_LIBTCLREADLINE='$(top_builddir)/../../tclreadline/libtclreadline.la'
    ym_tmp_builtin_tclreadline=yes
  else
    if test -r $withval/libtclreadline.la; then
      ym_tmp_found=yes
      YM_LIBS_LIBTCLREADLINE=$withval/libtclreadline.la
    fi
  fi
fi],[
#if test $ym_tmp_found = no; then
#  AC_CHECK_LIB([tclreadline], [Tclreadline_Init], [ym_tmp_found=yes])
#fi
if test $ym_tmp_found = no; then
  for dir in $prefix /usr /usr/local /opt; do
    ym_tmp_dir=$dir/$ym_tmp_lib
    if test -r $ym_tmp_dir/libtclreadline.la; then
      ym_tmp_found=yes
      YM_LIBS_LIBTCLREADLINE=$ym_tmp_dir/libtclreadline.la
      break
    fi
  done
fi
if test $ym_tmp_found = no && test -r  ../tclreadline/libtclreadline.la; then
  ym_tmp_found=yes
  YM_LIBS_LIBTCLREADLINE='$(top_builddir)/../tclreadline/libtclreadline.la'
  ym_tmp_builtin_tclreadline=yes
fi
])
if test "x$withval" = xno; then
  AC_MSG_RESULT([don't use tclreadline])
else
  if test $ym_tmp_found = no; then
    AC_MSG_ERROR([tclreadline is not found. Use --with-tclreadline to specify the directory containing libtclreadline.la])
  fi
  AC_MSG_RESULT([$YM_LIBS_LIBTCLREADLINE])
  AC_SUBST(YM_LIBS_LIBTCLREADLINE)

  AC_MSG_CHECKING([for tclreadline.h])
  ym_tmp_found=no
  if test $ym_tmp_builtin_tclreadline = yes && test -r ../tclreadline/tclreadline.h; then
    ym_tmp_found=yes
    TCLRL_INCLUDES='-I$(top_builddir)/../tclreadline'
  else
    AC_ARG_WITH([tclreadline-include],
      AS_HELP_STRING([--with-tclreadline-include=DIR], [tclreadline.h is in DIR]),
      [
        if test "x$withval" != x && test -r $withval/tclreadline.h; then
          ym_tmp_found=yes
          TCLRL_INCLUDES="-I$withval"
        fi], [
        if test $ym_tmp_found = no; then
          for dir in $prefix /usr /usr/local /opt; do
            ym_tmp_dir=$dir/include
            if test -r $ym_tmp_dir/tclreadline.h; then
              ym_tmp_found=yes
              TCLRL_INCLUDES="-I$ym_tmp_dir"
              break
            fi
          done
        fi
      ])
  fi
  if test $ym_tmp_found = no; then
    AC_MSG_ERROR([tclreadline.h is not found. use --with-tclreadline-include to specify the directory containing tclreadline.h])
  fi
  AC_MSG_RESULT([$TCLRL_INCLUDES])
  AC_SUBST(TCLRL_INCLUDES)
  AC_DEFINE(HAVE_TCLREADLINE, 1, [Define if having built-in tclreadline])
fi
])
# end of tcl.m4
