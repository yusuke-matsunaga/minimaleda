# Macros for checking gmp package
#
# $Id: gmp.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

# ===================================================================
# YM_CHECK_GMP
# if found, the following shell-variables are defined
# GMP_INCLUDES
# GMP_LIBS
#
# Also, the following C-macro is defined
# HAVE_GMP
# ===================================================================
# Checks for gmp package
AC_DEFUN([YM_CHECK_GMP],[dnl
AC_MSG_NOTICE([checking if gmp is available])
#
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib="lib64"
else
  ym_tmp_lib="lib"
fi
# --with-gmp-prefix
AC_ARG_WITH([gmp-prefix],
[  --with-gmp-prefix=DIR  gmp.h is in $DIR/include],
[gmp_list="$withval"],
[gmp_list="$prefix $ac_default_prefix /usr /usr/local"])
#
# --with-gmp-libdir
AC_ARG_WITH([gmp-libdir],
[  --with-gmp-libdir=DIR  libgmp is in $DIR],
[gmp_libdir="$withval"],
[gmp_libdir=""])

gmp_found=0
GMP_INCLUDES=
GMP_LIBS=
libs_old=$LIBS
for p in $gmp_list; do
  gmp_include=$p/include
  if test -f $gmp_include/gmp.h; then
    GMP_INCLUDES="-I$gmp_include";
    if test "x$gmp_libdir" = "x"; then
      gmp_libdir=$p/$ym_tmp_lib
    fi
    LIBS="$libs_old -L$gmp_libdir"
    AC_CHECK_LIB([gmp], [__gmpz_init],
		 [gmp_found=1
                  GMP_LIBS="-L$gmp_libdir -lgmp"
	      	  AC_DEFINE([HAVE_GMP], 1, [Define if you have gmp package])
	          AC_SUBST([GMP_INCLUDES])
	          AC_SUBST([GMP_LIBS])
	          break
		 ], [],
		 [-lc])
  fi
done
LIBS=$libs_old
if test $gmp_found = 1; then
   AC_MSG_RESULT([gmp is found at $GMP_LIBS])
else
   AC_MSG_RESULT([gmp is not found, see http://gmplib.org/])
fi
])dnl
# end of gmp.m4
