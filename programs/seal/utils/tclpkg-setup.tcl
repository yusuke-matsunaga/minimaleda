# インデックスファイルを作るためのスクリプト
#
# $Id:$


# file の最終更新日時をとってくる．
# file が存在しない場合には 0 を返す．
proc get_mtime {file} {
    if [file exists $file] {
	return [file mtime $file]
    } else {
	return 0
    }
}

# dir 自体もしくはその下にある *.tcl のパターンにマッチするファイル
# の最終更新日時を得る．
proc get_mtime_dir {dir} {
    set mtime [file mtime $dir]
    foreach file [glob -nocomplain [file join $dir *.tcl]] {
	set mtime1 [file mtime $file]
	if {$mtime1 > $mtime} {
	    set mtime $mtime1
	}
    }
    return $mtime
}

# このスクリプトは argv0 pkgroot subpkgs 
# という形式で呼び出す．
if { $argc != 2 } {
    error "Wrong # of args, shuld be $argv0 pkgroot subpkgs"
}
set pkgroot [lindex $argv 0]
set subpkgs [lindex $argv 1]

foreach mod $subpkgs {
    # このサブモジュールの根本のパス
    set moddir [file join $pkgroot $mod]

    # スクリプトがあるディレクトリ
    set scriptdir [file join $moddir scripts]

    # インデックスファイル
    set index [file join $scriptdir tclIndex]

    # $scriptdir の更新日時
    set dir_mtime [get_mtime_dir $scriptdir]

    # インデックスファイルの更新日時
    set idx_mtime [get_mtime $index]

    # ディレクトリの更新日時とインデックスファイルの更新日時を比較
    # してインデックスファイルのほうが古ければ作り直す．
    if {$dir_mtime > $idx_mtime} {
	puts "creating $index"
	auto_mkindex $scriptdir *.tcl
    }
}
exit

# emacs を tcl モードにするためのおまじない
#;; Local Variables:
#;; mode: Tcl
#;; End:
