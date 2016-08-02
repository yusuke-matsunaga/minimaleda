#/// @file read_file.tcl
#/// @brief ファイル読み込みのラッパ関数
#/// @author Yusuke Matsunaga (松永 裕介)
#/// 
#/// $Id: read_file.tcl 2043 2009-02-21 07:08:48Z matsunaga $
#///
#/// Copyright (C) 2005-2007 Yusuke Matsunaga
#/// All rights reserved.

# 名前空間を使うためのダミー
namespace eval ::matpg { }

# procedure: ::atpg::read_file
# USAGE readfile ?-blif|-iscas89? <回路ファイル名>
# 
# 引数のファイルを読み込む．
# 拡張子が ".blif" の場合は BLIF 形式
# 拡張子が ".bench" の場合は ISCAS89(.bench) 形式と仮定する．
# ただし，-blif オプションがあれば強制的に BLIF 形式と判定する．
# 同様に -iscas89 オプションがあれば強制的に ISCAS89 形式と判定する．
proc ::matpg::read_file args {
    # 引数の解析
    set blif 0
    set iscas89 0
    set fcount 0
    set fname ""
    foreach elem $args {
	switch -exact -- $elem {
	    "-blif"    {
		incr blif
	    }
	    "-iscas89" {
		incr iscas89
	    }
	    default {
		incr fcount
		set fname $elem
	    }
	}
    }

    # 引数の integrity check
    if { $fcount == 0 } {
	# ファイル名がない．[error]
	error "No filename specified"
    }
    if { $fcount > 1 } {
	# ファイル名が2つ以上ある．[error]
	error "More than one filenames specified"
    }
    if { $blif > 0 && $iscas89 > 0 } {
	# -blif と -iscas89 がともに指定されている．[error]
	error "Both -blif and -iscas89 specified"
    }
    if { $blif > 1 } {
	# -blif が二回以上指定されている．[warning]
	set blif 1
    }
    if { $iscas89 > 1 } {
	# -iscas89 が二回以上指定されている．[warning]
	set iscas89 1
    }
    
    # 形式指定のないときはファイル名から推測する．
    if { $blif == 0 && $iscas89 == 0 } {
	switch -glob -- $fname {
	    *\.blif  { set blif 1 }
	    *\.bench { set iscas89 1 }
	}
    }

    if { $blif == 0 && $iscas89 == 0 } {
	# 形式が不明．[error]
	error "No filetype(BLIF|ISCAS89) specified"
    }

    if { $blif == 1 } {
	read_blif $fname
    } elseif { $iscas89 == 1 } {
	read_iscas89 $fname
    }
}

# emacs を tcl モードにするためのおまじない
#;; Local Variables:
#;; mode: Tcl
#;; End:
