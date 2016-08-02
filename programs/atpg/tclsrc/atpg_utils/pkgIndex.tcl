# @file read_file.tcl
# @brief auto-loading 用の情報を納めたインデックスファイル
# @author Yusuke Matsunaga (松永 裕介)
#
# $Id: atpg_utils.tcl 1366 2008-04-09 02:08:37Z matsunaga $
#
# Copyright (C) 2005-2007 Yusuke Matsunaga
# All rights reserved.

# 本当は pkg_mkIndex でつくるものだが
# Effective Tcl/Tk に書かれた通りに手で作っている．

# 名前空間を使うためのダミー
namespace eval ::atpg { }

proc ::atpg::utils_init {dir ver} {
    package require Tcl
    package require Atpg

    # バージョン
    set ::atpg::pkg_version(utils) $ver

    # パッケージ情報の設定
    package provide Atpg_Utils $ver

    # ライブラリパスの設定
    set ::atpg::library_path(utils) $dir
    set ::env(ATPG_UTILS_LIBRARY) $dir
    lappend ::auto_path [file join $dir scripts]
}

package ifneeded Atpg_Utils 1.0 [list ::atpg::utils_init $dir 1.0]

# emacs を tcl モードにするためのおまじない
#;; Local Variables:
#;; mode: Tcl
#;; End:
