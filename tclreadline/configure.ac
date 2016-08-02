dnl -*- autoconf -*-
dnl FILE: "/home/joze/src/tclreadline/configure.in"
dnl LAST MODIFICATION: "Fre, 08 Dez 2000 01:36:25 +0100 (joze)"
dnl (C) 1998 - 2000 by Johannes Zellner, <johannes@zellner.org>
dnl $Id: configure.in,v 2.26 2000/12/08 00:45:45 joze Exp $
dnl ---
dnl tclreadline -- gnu readline for tcl
dnl http://www.zellner.org/tclreadline/
dnl Copyright (c) 1998 - 2000, Johannes Zellner <johannes@zellner.org>
dnl This software is copyright under the BSD license.

MAJOR=2
MINOR=1
PATCHLEVEL=0
VERSION=$MAJOR.$MINOR
PATCHLEVEL_STR=$VERSION.$PATCHLEVEL

AC_INIT([tclreadline], [2.1.0], [], [tclreadline])dnl
#AC_INIT(tclreadline.c)

AC_CONFIG_SRCDIR([tclreadline.c])dnl

#AM_CONFIG_HEADER(config.h)
AC_CONFIG_HEADERS([config.h])dnl

AC_PREREQ(2.59)
AC_REVISION($Revision: 2.26 $)
AC_CONFIG_AUX_DIR(./config)

AC_SUBST(MAJOR)
AC_SUBST(MINOR)
AC_SUBST(PATCHLEVEL)
AC_SUBST(VERSION)
AC_SUBST(PATCHLEVEL_STR)

#AM_INIT_AUTOMAKE(tclreadline, $PATCHLEVEL_STR)
AM_INIT_AUTOMAKE()dnl


AC_PROG_MAKE_SET

dnl AM_INIT_AUTOMAKE(tclreadline, $VERSION)

AC_CANONICAL_HOST
AC_PROG_CC
AC_EXEEXT
dnl AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_C_CONST
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LN_S


# -- WHICH TCL TO USE
AC_ARG_WITH(
    tcl,
    [  --with-tcl=DIR          where to look for tclConfig.sh],
    tcl_search=$withval,
    tcl_search=""
)

AC_MSG_CHECKING([which tclConfig.sh to use])
if test "x$tcl_search" != x && test -r $tcl_search/tclConfig.sh; then
  TCL_LIB_DIR=$tcl_search
else
  TCL_LIB_DIR=""
  arch=`uname -p`
  if test "X$arch" = "Xx86_64"; then
    ym_tmp_lib="lib64"
  else
    ym_tmp_lib="lib"
  fi
  ym_tmp_path="$prefix $ac_default_prefix /usr /usr/local"
  for dir in $ym_tmp_path; do
    for subdir in tcl8.4 tcl8.3 tcl8.2 tcl8.1 tcl8.0 tcl8.0jp tcl; do
      tmpdir=$dir/$ym_tmp_lib/$subdir
      if test -r $tmpdir/tclConfig.sh; then
        TCL_LIB_DIR=$tmpdir
        break
      fi
    done
  done
fi

if test -z "$TCL_LIB_DIR"; then
    AC_MSG_ERROR(Can't find Tcl libraries.  Use --with-tcl to specify the directory containing tclConfig.sh on your system.)
fi

. $TCL_LIB_DIR/tclConfig.sh
AC_MSG_RESULT($TCL_LIB_DIR/tclConfig.sh)
AC_MSG_CHECKING([for your tcl version])
AC_MSG_RESULT([$TCL_VERSION, patchlevel $TCL_PATCH_LEVEL])

# Check, if tcl_version is > 8.0
if test $TCL_MAJOR_VERSION -lt 8; then
    AC_MSG_ERROR(need tcl 8.0 or higher.)
fi


# -----------------------------------------------------------------------
#   Set up a new default --prefix.
# -----------------------------------------------------------------------

# this is the default anyway:
# AC_PREFIX_DEFAULT(/usr/local)

if test "${prefix}" = "NONE"; then
    prefix=$TCL_PREFIX
fi
TCLRL_LIBDIR="${prefix}/lib"
AC_SUBST(TCLRL_LIBDIR)
TCLRL_DIR="${TCLRL_LIBDIR}/tclreadline$VERSION"
AC_SUBST(TCLRL_DIR)

# HEADERS
AC_HEADER_STDC
AC_CHECK_HEADERS(string.h)

#--------------------------------------------------------------------
#   Check for tcl.h
#--------------------------------------------------------------------

AC_ARG_WITH(tcl-includes,
            [  --with-tcl-includes=DIR where to look for tcl.h],
            tcl_includes=$withval, tcl_includes=$prefix/include)

AC_MSG_CHECKING([where to look for tcl.h])
TCL_INCLUDE_DIR=""
if test -r $tcl_includes/tcl.h; then
  TCL_INCLUDE_DIR=$tcl_includes
else
  dnl try to check using TCL_INCLUDE_SPEC
  ym_tmp_dir=`echo $TCL_INCLUDE_SPEC | sed -e 's/-I\(.*\)/\1/g'`
  if test -r $ym_tmp_dir/tcl.h; then
    TCL_INCLUDE_DIR=$ym_tmp_dir
  else
    for dir in /usr/include /usr/local/include; do
      if test -r $dir/tcl.h; then
        TCL_INCLUDE_DIR=$dir
        break
      fi
    done
  fi
fi
if test -z "$TCL_INCLUDE_DIR"; then
    AC_MSG_ERROR([
  Can't find tcl.h.  Use --with-tcl-includes to specify the directory
  containing tcl.h on your system.])
else
    AC_MSG_RESULT($TCL_INCLUDE_DIR/tcl.h)
fi


# ---
# check for the terminal library.
# ---

AC_ARG_WITH(tlib-library,
    [  --with-tlib-library=DIR
		  lib spec to tlib (e.g. '-L/usr/local/lib -lncurses')],
    LIBS="$LIBS $withval",
    for tlib in ncurses termlib termcap curses; do
	brk=yes
	AC_CHECK_LIB($tlib, tputs, , brk=no)
	if test $brk = "yes"; then
	    HAVE_TLIB=yes
	    break
	fi
    done
    if test "x" = "x$HAVE_TLIB"; then
	AC_MSG_ERROR([
	    no terminal library found. Please retry with the --with-tlib argument.
	])
    fi
)


#--------------------------------------------------------------------
#   Check for readline.h
#--------------------------------------------------------------------

AC_ARG_WITH(readline-includes,
            [  --with-readline-includes=DIR readline include files in DIR],
            rl_includes=$withval, rl_includes="")

AC_MSG_CHECKING([where to look for readline.h])
READLINE_INCLUDE_DIR=""
for dir in $rl_includes /usr/local/include/readline /usr/local/include /usr/include/readline /usr/include ; do
    if test -r $dir/readline.h; then
        READLINE_INCLUDE_DIR=$dir
        break
    fi
done

if test -z "$READLINE_INCLUDE_DIR"; then
    AC_MSG_ERROR([
  Can't find readline.h.
  Use --with-readline-includes to specify the directory
  containing readline.h on your system.])
else
    AC_MSG_RESULT($READLINE_INCLUDE_DIR/readline.h)
	base="`(basename ${READLINE_INCLUDE_DIR}) 2>/dev/null`"
	if test "x${base}" != "xreadline"; then
		AC_DEFINE_UNQUOTED(READLINE_LIBRARY, 1, [ Define if we have libreadline. ])
	fi
fi

AC_ARG_WITH(readline-library,
    [  --with-readline-library=DIR
		  lib spec to readline (e.g. '-L/usr/local/lib -lreadline')],
    LIBS="$LIBS $withval",
    AC_CHECK_LIB(readline, rl_callback_read_char, ,
	AC_MSG_RESULT([
	    Your readline version does not support readline's alternate interface.
	    Please upgrade to readline >= 2.2 and retry.
	])
	exit
    )
)



# check for readline's (macro.c) private variable
# _rl_executing_macro.

AC_MSG_CHECKING([for _rl_executing_macro in -lreadline])
AC_TRY_LINK(,[
    extern char* _rl_executing_macro;
    _rl_executing_macro = (char*) 0;
],
    AC_MSG_RESULT(yes);
    AC_DEFINE(EXECUTING_MACRO_HACK, 1,
    [ Define if _rl_executing_macro is resolved in libreadline. ]),
    AC_MSG_RESULT(no))

# check for readline's rl_cleanup_after_signal

AC_MSG_CHECKING([for rl_cleanup_after_signal() in -lreadline])
AC_TRY_LINK(,[
    extern void rl_cleanup_after_signal();
    rl_cleanup_after_signal();
],
    AC_MSG_RESULT(yes);
    AC_DEFINE(CLEANUP_AFER_SIGNAL, 1,
    [ Define if rl_cleanup_after_signal is resolved in libreadline. ]),
    AC_MSG_RESULT(no))


AC_MSG_CHECKING([for the readline version number])
AC_TRY_RUN([
#include <stdio.h>
int main () {
    FILE *fp = fopen("conftestversion", "w");
    extern char *rl_library_version;
    fprintf(fp, "%s", rl_library_version);
    close(fp);
    return 0;
}],
    READLINE_VERSION=`cat conftestversion`
    AC_MSG_RESULT($READLINE_VERSION),
    AC_MSG_RESULT(failed),
    AC_MSG_RESULT(no cross compiling)
)


AC_ARG_ENABLE(tclshrl,
    [  --enable-tclshrl        build statically linked tclshrl],
    [dnl action if given
	case "${enableval}" in
	    yes) enable_static=true ;;
	    no)  enable_static=false ;;
	    *)   AC_MSG_ERROR(bad value ${enableval} for --enable-static) ;;
	esac
    ],
    [dnl action if not given
	enable_static=false
    ]
)
AM_CONDITIONAL(STATIC_TCLSHRL, test x$enable_static = xtrue)

AC_ARG_ENABLE(wishrl,
    [  --enable-wishrl         build statically linked wishrl],
    [dnl action if given
	case "${enableval}" in
	    yes)
		enable_static=true
		dnl source the tkConfig.sh which defines TK_LIB_SPEC
		. $TCL_LIB_DIR/tkConfig.sh
		AC_SUBST(TK_LIB_SPEC)
	    ;;
	    no)  enable_static=false ;;
	    *)   AC_MSG_ERROR(bad value ${enableval} for --enable-static) ;;
	esac
    ],
    [dnl action if not given
	enable_static=false
    ]
)
AM_CONDITIONAL(STATIC_WISHRL, test x$enable_static = xtrue)


AC_SUBST(TCL_INCLUDE_DIR)
AC_SUBST(TCL_LIB_SPEC)
AC_SUBST(READLINE_INCLUDE_DIR)


AC_CONFIG_FILES([
	Makefile 
	tclreadline.h
	tclreadlineInit.tcl
	tclreadlineSetup.tcl
	tclreadline.n 
	pkgIndex.tcl
	])

AC_OUTPUT
