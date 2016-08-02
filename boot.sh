#! /bin/sh

# Copyright (C) 2005-2010 Yusuke Matsunaga
# All rights reserved

# [概要]
#
# - configure スクリプトの生成
# - autogen スクリプトの生成
# - clean up
# を行うスクリプト
#
# [使用方法]
#
# ./boot
#   automake, autoconf などを使って configure スクリプトおよび Makefile.in
#   を生成する．
#
# ./boot autogen <build-dir> <install-dir> <compile-mode>
#   configure スクリプトを実行するための autogen スクリプトを生成する．
#   <build-dir> は実際にコンパイルを行うディレクトリ
#   <install-dir> はインストール先の根本のディレクトリ
#                  (その下に bin, include, lib が作られる)
#   <compile-mode> opt|debug
#
# ./boot clean
#   生成されたファイルを削除してチェックアウトした直後の状態に戻す
#
# ./boot debug
#   使用するツールのバージョンを出力する
#
# [環境変数]
#
# AUTOCONF
#   定義されていたら autoconf プログラムの代わりに用いられる．
#   例えば /opt/bin/autoconf とか autoconf259 の様な場合に用いる．
# AUTOCONF_FLAGS
#   定義されていたら autoconf に渡すオプションとして用いられる．
# AUTOHEADER
#   定義されていたら autoheader プログラムの代わりに用いられる．
# AUTOHEADER_FLAGS
#   定義されていたら autoheader に渡すオプションとして用いられる．
# AUTOMAKE
#   定義されていたら automake プログラムの代わりに用いられる．
# AUTOMAKE_FLAGS
#   定義されていたら automake に渡すオプションとして用いられる．
# ACLOCAL
#   定義されていたら aclocal プログラムの代わりに用いられる．
# ACLOCAL_FLAGS
#   定義されていたら aclocal に渡すオプションとして用いられる．
# LIBTOOLIZE
#   定義されていたら libtoolize プログラムの代わりに用いられる．
# LIBTOOLIZE_FLAGS
#   定義されていたら libtoolize に渡すオプションとして用いられる．
# M4
#   定義されていたら m4 プログラムの代わりに用いられる．
# FIND
#   定義されていたら find プログラムの代わりに用いられる．
# RM
#   定義されていたら rm プログラムの代わりに用いられる．


# function を使えるシェルかどうかのチェック
# GNU Autoconf/Automake/Libtool 21.1.4 のコードを元にしたけど
# 本に載っているコードはそのままでは引数を渡すことができない．
# また SHELL という環境変数がすでに定義済だとうまく動かない．

# zsh の場合
if test -n "$ZSH_VERSION"; then
  emulate sh
  NULLCMD=:
fi

# bash の場合
test -n "$BASH_VERSION" && set -o posix

TMP_SHELL="${TMP_SHELL-/bin/sh}"
if test x"$1" = x--re-executed; then
  shift
elif "$TMP_SHELL" -c 'foo () { exit 0; }; foo' 2>/dev/null; then
  :
else
  for cmd in sh bash ash bsh ksh zsh sh5; do
    args=${1+"$@"}
    set `IFS=":"; X="$PATH:/bin:/usr/bin:/usr/afsws/bin:/usr/ucb"; echo $X`
    for dir
    do
      shell="$dir/$cmd"
      if (test -f "$shell" || test -f "$shell.exe") &&
        "$shell" -c 'foo () { exit 0; }; foo' 2>/dev/null
      then
        SHELL="$shell" exec "$shell" "$0" --re-executed $args
      fi
    done
  done
  echo "Unable to locate a shell interpreter with function support" >&2
  exit 1
fi

# このファイルのあるディレクトリを BASEDIR にセットする．
basedir=`dirname $0`
BASEDIR=`cd $basedir; pwd`

# 共通な関数を読み込む
. $BASEDIR/etc/mk.subr

# プログラム名の設定
set_program

# 必要な変数の設定
AUTOGEN_COMMON="./autogen.common"

# サブモジュールのディレクトリ名
#SUBMODULE=`cat $BASEDIR/modules`
LIBRARIES=\
"libym_utils \
 libym_zstream \
 libym_lexp \
 libym_bdd \
 libym_npn \
 libym_sat \
 libym_mincov \
 libym_blif \
 libym_iscas89 \
 libym_cell \
 libym_verilog \
 libym_bnet \
 libym_bdn \
 libym_mvn \
 libym_sbj \
 libym_aig \
 libym_tgnet \
 libym_gds \
 libym_cec \
 libym_lutmap \
 libym_techmap \
 libym_seal \
 libym_tclpp"

PROGRAMS=\
"magus_tclsh \
 matpg \
 atpg \
 seal \
 lutmap"

# 第1引数に応じた処理を行う．
case "$1" in
  ""|boot)
    case $# in
      0|1)
	clean_config $BASEDIR/config
	clean $BASEDIR
	clean $BASEDIR/include
	for lib in $LIBRARIES; do
	    clean_config $BASEDIR/libraries/$lib
	    clean $BASEDIR/libraries/$lib
	done
	for prog in $PROGRAMS; do
	    clean_config $BASEDIR/programs/$prog
	    clean $BASEDIR/programs/$prog
	done
        boot $BASEDIR
	boot $BASEDIR/include
	for lib in $LIBRARIES; do
	    boot $BASEDIR/libraries/$lib
	done
	for prog in $PROGRAMS; do
	    boot $BASEDIR/programs/$prog
	done
        ;;
      2)
	clean_config $2
	clean $2
        boot $2
        ;;
      *) usage;;
    esac
    ;;
  clean)
    case $# in
      1)
        clean_config $BASEDIR/config
	clean $BASEDIR
	clean $BASEDIR/include
	for lib in $LIBRARIES; do
	    clean_config $BASEDIR/libraries/$lib
	    clean $BASEDIR/libraries/$lib
	done
	for prog in $PROGRAMS; do
	    clean_config $BASEDIR/programs/$prog
	    clean $BASEDIR/programs/$prog
	done
	;;
      2)
        clean_config $2
	clean $2
	;;
      *) usage;;
    esac
    ;;
  autogen)
    case $# in
      3|4)
	mk_autogen $BASEDIR $2 $3 $4
	;;
      *) usage;;
    esac
    ;;
  debug) display_version ;;
  *) usage;;
esac
