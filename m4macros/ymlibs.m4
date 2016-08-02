# M4 Macros for using ymlibs
# author: Yusuke Matsunaga (松永 裕介)
#
# $Id: ymlibs.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

#==================================================================
# YM_CHECK_YMLIBS
#
# Description: check for 'ymlibs'
#==================================================================
AC_DEFUN([YM_CHECK_YMLIBS], [
AC_MSG_CHECKING([for ymlibs])
ym_tmp_found=no
ym_tmp_use_src=no
ym_tmp_ymlibsdir=../ymlibs
AC_ARG_WITH(ymlibs,
            AS_HELP_STRING([--with-ymlibs=DIR],
                           [ymConfig.sh is in DIR]),
[ #--with-ymlibs specified
if test "x$withval" != x && test -f $withval/ymConfig.sh; then
  #found
  ym_tmp_dir=$withval
  ym_tmp_found=yes
fi],
[ #not specified
if test -f $ym_tmp_ymlibsdir/ymConfig.sh; then
  #found source tree
  ym_tmp_dir=$ym_tmp_ymlibsdir
  ym_tmp_found=yes
  ym_tmp_use_src=yes
else
  arch=`uname -p`
  if test "X$arch" = "Xx86_64"; then
    ym_tmp_lib="lib64"
  else
    ym_tmp_lib="lib"
  fi
  ym_tmp_dirlist="$prefix/$ym_tmp_lib $ac_default_prefix/$ym_tmp_lib /usr/$ym_tmp_lib /usr/local/$ym_tmp_lib"
  ym_tmp_subdirlist="ymlibs1.0 ymlibs"
  for dir in $ym_tmp_dirlist
  do
    if test -f $dir/ymConfig.sh; then
      ym_tmp_dir=$dir
      ym_tmp_found=yes
    else
      for subdir in $ym_tmp_subdirlist
      do
        if test -f $dir/$subdir/ymConfig.sh; then
          ym_tmp_dir=$dir/$subdir
          ym_tmp_found=yes
          break
        fi
      done
    fi
    if test $ym_tmp_found = yes; then break; fi
  done
fi
])#end-of AC_ARG_WITH

if test $ym_tmp_found = yes; then
  #read config file
  . $ym_tmp_dir/ymConfig.sh

  if test $ym_tmp_use_src = yes; then
    YM_INCLUDES="\$(GMP_INCLUDES) -I\$(top_srcdir)/../ymlibs/include -I\$(top_builddir)/../ymlibs"
    ym_tmp_libdir='\$\(top_builddir\)/../ymlibs'
    for module in ${YM_LIBS_MODULES}
    do
      MODULE=`echo $module | sed -e y/$as_cr_letters/$as_cr_LETTERS/`
      modvar=`eval echo "$"YM_LIBS_LIBYM_${MODULE}`
      if test "x$modvar" != x; then
        eval YM_LIBS_LIBYM_${MODULE}="${ym_tmp_libdir}/libym_${module}/libym_${module}.la"
      fi
    done
    AC_MSG_RESULT([use source tree])
  else
    AC_MSG_RESULT([$ym_tmp_dir])
  fi
  AC_SUBST([YM_INCLUDES])
else
  AC_MSG_ERROR([not found])
fi
])


#==================================================================
# YM_CHECK_LIBS(libname)
#
#==================================================================
AC_DEFUN([YM_CHECK_LIBS], [
__YM_CHECK_LIBS_SUB($1, m4_toupper($1))
])

AC_DEFUN([__YM_CHECK_LIBS_SUB],[
ym_tmp_found=yes
AC_MSG_CHECKING([for $1])
if test "x${YM_LIBS_$2}" = x; then
  ym_tmp_found=no
  AC_MSG_ERROR([not found])
else
  AC_SUBST([YM_LIBS_$2])
  AC_MSG_RESULT([$YM_LIBS_$2])
fi
])


#==================================================================
# YM_BUILD_LIBRARY(libname, required-conditions)
#
# Description: libname をビルドするかどうかを判断し，設定する．
#              required-conditions にはカンマで区切られた shell 変数
#              のリストを渡す([]も可)．全ての変数が 1 の時のみこの
#              ライブラリはビルドされる．
#==================================================================
AC_DEFUN([YM_BUILD_LIBRARY],
[
ym_tmp_enable=${ym_[]$1[]_enable}
m4_ifval([$2], [
    m4_foreach([XXX], [$2], [
       if test "$ym_tmp_enable" = "1" -a "$XXX" != "1"; then
           ym_tmp_enable=0
	   AC_MSG_NOTICE(['$1' will be disabled because 'XXX' is disabled])
       fi
    ])
])
if test "$ym_tmp_enable" = "1"; then
    YM_ADD_LIBRARIES_SUBDIRS([$1])
    AC_CONFIG_SUBDIRS([libraries/$1])
    AC_MSG_NOTICE(['$1' is enabled])
fi
])


# ==================================================================
# YM_INIT_LIBRARIES_SUBDIRS
# 
# Description: Initialize 'YM_LIBRARIES_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_INIT_LIBRARIES_SUBDIRS], 
[
ym_libraries_subdirs=""
])


# ==================================================================
# YM_ADD_LIBRARIES_SUBDIRS(dirname)
#
# Description: add <dirname> to 'YM_LIBRARIES_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_ADD_LIBRARIES_SUBDIRS], 
[
ym_libraries_subdirs="$ym_libraries_subdirs $1"
])


# ==================================================================
# YM_FINISH_LIBRARIES_SUBDIRS
#
# Description: finalize 'YM_LIBRARIES_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_FINISH_LIBRARIES_SUBDIRS], [
AC_SUBST([YM_LIBRARIES_SUBDIRS], $ym_libraries_subdirs)
])


#==================================================================
# YM_BUILD_PROGRAM(progname, required-conditions)
#
# Description: progname をビルドするかどうかを判断し，設定する．
#              required-conditions にはカンマで区切られた shell 変数
#              のリストを渡す([]も可)．全ての変数が 1 の時のみこの
#              プログラムはビルドされる．
#==================================================================
AC_DEFUN([YM_BUILD_PROGRAM],
[
ym_tmp_enable=${ym_[]$1[]_enable}
m4_ifval([$2], [
    m4_foreach([XXX], [$2], [
       if test "$ym_tmp_enable" = "1" -a "$XXX" != "1"; then
           ym_tmp_enable=0
	   AC_MSG_NOTICE(['$1' will be disabled because 'XXX' is disabled])
       fi
    ])
])
if test "$ym_tmp_enable" = "1"; then
    YM_ADD_PROGRAMS_SUBDIRS([$1])
    AC_CONFIG_SUBDIRS([programs/$1])
    AC_MSG_NOTICE(['$1' is enabled])
fi
])


# ==================================================================
# YM_INIT_PROGRAMS_SUBDIRS
# 
# Description: Initialize 'YM_PROGRAMS_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_INIT_PROGRAMS_SUBDIRS], 
[
ym_pgorams_subdirs=""
])


# ==================================================================
# YM_ADD_PROGRAMS_SUBDIRS(dirname)
#
# Description: add <dirname> to 'YM_PROGRAMS_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_ADD_PROGRAMS_SUBDIRS], 
[
ym_programs_subdirs="$ym_programs_subdirs $1"
])


# ==================================================================
# YM_FINISH_PROGRAMS_SUBDIRS
#
# Description: finalize 'YM_PROGRAMS_SUBDIRS'
# ==================================================================
AC_DEFUN([YM_FINISH_PROGRAMS_SUBDIRS], [
AC_SUBST([YM_PROGRAMS_SUBDIRS], $ym_programs_subdirs)
])
