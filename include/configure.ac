# Process this file with autoconf to produce a configure script
#
# $Id: configure.ac 2507 2009-10-17 16:24:02Z matsunaga $

# ===================================================================
# Initial block
# ===================================================================
AC_INIT([ymtools_header], [1.0], 
        [yusuke_matsunaga@ieee.org],
        [ymtools_header])dnl

AC_REVISION($Rev: 2507 $)dnl

AC_PREREQ(2.57)dnl

AC_COPYRIGHT([
Copyright (C) 2005-2010 Yusuke Matsunaga
All rights reserved.
])dnl

AC_CONFIG_SRCDIR([ymnsdef.h])dnl

AC_CONFIG_AUX_DIR([../config])dnl
AC_CONFIG_MACRO_DIR([../config])dnl
AC_SUBST(ac_aux_dir)dnl

AM_INIT_AUTOMAKE()dnl


# ===================================================================
# Checks for programs
# ===================================================================
AC_ISC_POSIX
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# GCC のバージョンを $ym_gnuc に入れておく．
_AC_COMPUTE_INT([__GNUC__], ym_gnuc)


# ===================================================================
# Checks for libraries
# ===================================================================

AC_CHECK_LIB([z], [compress], [ym_have_libz=yes])


# ===================================================================
# Checks for header files
# ===================================================================
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([strings.h], [ym_have_strings_h=yes])
AC_CHECK_HEADERS([unistd.h], [ym_have_unistd_h=yes])
AC_CHECK_HEADERS([limits.h], [ym_have_limits_h=yes])
AC_CHECK_HEADERS([float.h], [ym_have_float_h=yes])
AC_CHECK_HEADERS([math.h], [ym_have_math_h=yes])
AC_CHECK_HEADERS([sys/time.h], [ym_have_sys_time_h=yes])
AC_CHECK_HEADERS([sys/stat.h], [ym_have_sys_stat_h=yes])
AC_CHECK_HEADERS([zlib.h], [ym_have_zlib_h=yes])


# ===================================================================
# Checks for typedefs, structures, and compiler characteristics
# ===================================================================
AC_TYPE_SIZE_T


# ===================================================================
# Check for size of built-in types
# ===================================================================
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(void*)
AC_CHECK_SIZEOF(size_t)


# ===================================================================
# Check for return type of signal handler
# ===================================================================


# ===================================================================
# Checks for system services
# ===================================================================


# ===================================================================
# Checks for other packages
# ===================================================================

# Check for zlib
if test "x$ym_have_zlib_h" = xyes -a "x$ym_have_libz" = xyes; then
   ym_have_zlib=1
else
   ym_have_zlib=0
fi

AC_CACHE_SAVE


# ===================================================================
# Generating ymtools.h
# ===================================================================
AC_CONFIG_COMMANDS([ymtools.h],[
ymtools="ymtools.h-tmp"
cat > $ymtools <<___EOF___
#ifndef YMTOOLS_H
#define YMTOOLS_H

/// @file ymtools.h
/// @brief ymtools に共通な型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// Do not edit! This file is generated from configure.in

//////////////////////////////////////////////////////////////////////
// 基本的なヘッダフィルのインクルード
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <sys/types.h>
___EOF___

if test "x$ac_cv_header_stdc" = xyes; then
  cat >> $ymtools << ___EOF___
#include <stdlib.h>
#include <string.h>
___EOF___
elif test "x$ym_have_strings_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <strings.h>
___EOF___
fi

if test "x$ym_have_unistd_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <unistd.h>
___EOF___
fi

if test "x$ym_have_limits_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <limits.h>
___EOF___
fi

if test "x$ym_have_float_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <float.h>
___EOF___
fi

if test "x$ym_have_math_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <math.h>
___EOF___
fi

if test "x$ac_cv_header_time" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/time.h>
#include <time.h>
___EOF___
elif test "x$ym_have_sys_time_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/time.h>
___EOF___
else
  cat >> $ymtools << ___EOF___
#include <time.h>
___EOF___
fi

if test "x$ym_have_sys_stat_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/stat.h>
___EOF___
fi

cat >> $ymtools << ___EOF___
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <new>
#include <utility>
#include <string>
#include <vector>
#include <list>
___EOF___

if test $ym_gnuc -ge 3; then
  cat >> $ymtools << ___EOF___
#include <ext/algorithm>
#include <ext/functional>
#include <ext/hash_set>
#include <ext/hash_map>
#define HASH_NAMESPACE __gnu_cxx
___EOF___
else
  cat >> $ymtools << ___EOF___
#include <algorithm>
#include <functional>
#include <hash_set>
#include <hash_map>
#define HASH_NAMESPACE std
___EOF___
fi

cat >> $ymtools << ___EOF___
#include "ymnsdef.h"
#include "ymassert.h"
#include "ymmath.h"
___EOF___

if test $ym_have_zlib=1; then
  cat >> $ymtools << ___EOF___
/* Define if you have zlib.h */
#define HAVE_ZLIB 1
___EOF___
fi

cat >> $ymtools << ___EOF___

//////////////////////////////////////////////////////////////////////
// 整数型の定義
//////////////////////////////////////////////////////////////////////

/// @brief 符号付き整数型
typedef int ymint;

/// @brief 符号付きlong型
typedef long ymlong;

/// @brief 符号無し整数型
typedef unsigned int ymuint;

/// @brief 符号無しlong型
typedef unsigned long ymulong;

/// @brief 8ビット符号付き整数型
typedef $ymint8 ymint8;

/// @brief 16ビット符号付き整数型
typedef $ymint16 ymint16;

/// @brief 32ビット符号付き整数型
typedef $ymint32 ymint32;

/// @brief 64ビット符号付き整数型
typedef $ymint64 ymint64;

/// @brief 8ビット符号なし整数型
typedef unsigned $ymint8 ymuint8;

/// @brief 16ビット符号なし整数型
typedef unsigned $ymint16 ymuint16;

/// @brief 32ビット符号なし整数型
typedef unsigned $ymint32 ymuint32;

/// @brief 64ビット符号なし整数型
typedef unsigned $ymint64 ymuint64;

/// @brief ポインタと同じサイズの符号なし整数型
typedef $ymptrint ympuint;

___EOF___

cat >> $ymtools << ___EOF___

BEGIN_NAMESPACE_HASH

// string/const string の hash_set/map 用のハッシュ関数クラス 
// hash<string>, hash<const string>定義．
// SGI STL の string クラス (v3.1 以降)にはこの定義は不要．
#if !defined(__SGI_STL_STRING) && !defined(DEFINE_HASH_STRING)
#define DEFINE_HASH_STRING

template<typename __Key> struct hash;

template <>
struct
hash<std::string>
{
  size_t
  operator()(const std::string& __x) const
  {
    unsigned long __h = 0; 
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;
    
    return size_t(__h);
  }
};

template <>
struct
hash<const std::string>
{
  size_t
  operator()(const std::string& __x) const {
    unsigned long __h = 0; 
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;
    
    return size_t(__h);
  }
};
 
#endif // __SGI_STL_STRING

END_NAMESPACE_HASH


#endif // YMTOOLS_H
___EOF___

if cmp -s $ymtools ymtools.h; then
  AC_MSG_NOTICE([ymtools.h is unchanged])
  rm -f $ymtools
else
  mv $ymtools ymtools.h
fi
],[
ac_cv_header_stdc=$ac_cv_header_stdc
ac_cv_header_time=$ac_cv_header_time
ym_have_strings_h=$ym_have_strings_h
ym_have_unistd_h=$ym_have_unistd_h
ym_have_limits_h=$ym_have_limits_h
ym_have_float_h=$ym_have_float_h
ym_have_math_h=$ym_have_math_h
ym_have_sys_time_h=$ym_have_sys_time_h
ym_have_sys_stat_h=$ym_have_sys_stat_h
ym_gnuc=$ym_gnuc
ymint8=char
case 2 in
$ac_cv_sizeof_short)
  ymint16=short
  ;;
$ac_cv_sizeof_int)
  ymint16=int
  ;;
esac
case 4 in
$ac_cv_sizeof_short)
  ymint32=short
  ;;
$ac_cv_sizeof_int)
  ymint32=int
  ;;
$ac_cv_sizeof_long)
  ymint32=long
  ;;
esac
case 8 in
$ac_cv_sizeof_int)
  ymint64=int
  ;;
$ac_cv_sizeof_long)
  ymint64=long
  ;;
$ac_cv_sizeof_long_long)
  ymint64='long long'
  ;;
esac
case $ac_cv_sizeof_voidp in
$ac_cv_sizeof_int)
  ymptrint='unsigned int'
  ;;
$ac_cv_sizeof_long)
  ymptrint='unsigned long'
  ;;
$ac_cv_sizeof_long_long)
  ymptrint='unsigned long long'
  ;;
esac
])


# ===================================================================
# Declare files to be generated by configure script
# ===================================================================
AC_CONFIG_FILES([Makefile])


# ===================================================================
# Generate configured files
# ===================================================================
AC_OUTPUT

# End-Of-File
