# Macros for checking popt package
#
# $Id: popt.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

# ===================================================================
# YM_CHECK_POPT(action-if-found, action-if-not-found)
# if found, the following shell-variables are defined
# POPT_INCLUDES
# POPT_LIBS
#
# Also, the following C-macro is defined
# HAVE_POPT
# ===================================================================
# Checks for popt package
AC_DEFUN([YM_CHECK_POPT],[dnl
AC_MSG_NOTICE([checking if popt is available])
#
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib="lib64"
else
  ym_tmp_lib="lib"
fi
# --with-popt-prefix
AC_ARG_WITH([popt-prefix],
[  --with-popt-prefix=DIR  popt.h is in $DIR/include],
[popt_list="$withval"],
[popt_list="$prefix $ac_default_prefix /usr /usr/local"])
#
# --with-popt-libdir
AC_ARG_WITH([popt-libdir],
[  --with-popt-libdir=DIR  libpopt is in $DIR],
[popt_libdir="$withval"],
[popt_libdir=""])

popt_found=0
libs_old=$LIBS
for p in $popt_list; do
  popt_include=$p/include
  if test -f $popt_include/popt.h; then
    POPT_INCLUDES="-I$popt_include";
    if test "x$popt_libdir" = "x"; then
      popt_libdir=$p/$ym_tmp_lib
    fi
    LIBS="$libs_old -L$popt_libdir"
    AC_CHECK_LIB([popt], [poptGetContext],
		 [popt_found=1
                  POPT_LIBS="-L$popt_libdir -lpopt"
	      	  AC_DEFINE([HAVE_POPT], 1, [Define if you have popt package])
	          AC_SUBST([POPT_INCLUDES])
	          AC_SUBST([POPT_LIBS])
	          break
		 ], [],
		 [-lc])
  fi
done
LIBS=$libs_old
if test $popt_found = 1; then
   AC_MSG_RESULT([popt is found at $POPT_LIBS])
else
   AC_MSG_RESULT([popt is not found])
fi
])dnl
# end of popt.m4
