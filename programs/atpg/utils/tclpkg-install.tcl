# Magus(実際には Wand パッケージ) のインストールプログラム
#
# $Source: /proj/synth/CVS/Magus/utils/tclpkg-install.tcl,v $
# $Author: yoshin-t $
# $Revision: 1.4 $
# $Date: 2000/06/05 11:32:26 $

# file の最終更新日時をとってくる．
# file が存在しない場合には 0 を返す．
proc get_mtime {file} {
    if [file exists $file] {
	return [file mtime $file]
    } else {
	return 0
    }
}

# old_file が存在しないか new_file よりも古ければコピーする．
proc install_if_newer {new_file old_file} {
    if {[file isdirectory $new_file]} {
	# ディレクトリの場合
	if {![file isdirectory $old_file]} {
	    # 転送先のディレクトリがなければ
	    #(もしくはディレクトリでなければ)作っておく．
	    file mkdir $old_file
	    file attributes $old_file -permissions 00775
	}

	# ディレクトリ内の個々のファイルに対して再帰する．
	foreach file [glob -nocomplain [file join $new_file *]] {
	    set tail [file tail $file]
	    if {[string match CVS $tail]} {
		# このディレクトリはコピーしなくて良い
		continue
	    }
	    if {[string match *.in $tail]} {
		# このファイルもコピーしなくて良い．
		continue
	    }
	    if {[string match *~ $tail]} {
		# このファイルもコピーしなくて良い．
		continue
	    }
	    if {[string match \#* $tail]} {
		# このファイルもコピーしなくて良い．
		continue
	    }
	    install_if_newer $file [file join $old_file $tail]
	}
    } else {
	# ファイルの場合
	# $new_file が symbolic link の場合は実体を探す
	while {[file exists $new_file] && "[file type $new_file]" == "link"} {
	    set dir_name [file dirname $new_file]
	    set new_file [file join $dir_name [file readlink $new_file]]
	}
	if {! [file exists $new_file]} {
	    error "$new_file does not exist"
	}
	# 元のファイルの最終更新日時
	set old_mtime [get_mtime $old_file]
	# 新しいファイルの最終更新日時
	set new_mtime [file mtime $new_file]
	if {$new_mtime > $old_mtime} {
	    # 元のは古いか存在しない．
	    puts "Installing $new_file --> $old_file"
	    file copy -force $new_file $old_file
	    if [file executable $old_file] {
		file attributes $old_file -permissions 00775
	    } else {
		file attributes $old_file -permissions 00664
	    }
	}
    }
}

# pkgIndex.tcl を読んでバージョンを取り出す．
proc pkg_version {dir} {
    # pkgIndex ファイル
    set file [file join $dir pkgIndex.tcl]
    if ![file exists $file] {
	error "$file does not exist"
    }

    set fid [open $file r]
    set contents [read $fid]
    close $fid

    set pat {package ifneeded [a-zA-Z_0-9]+ ([0-9\.]+)}
    if ![regexp $pat $contents dummy v] {
	error "$file : No versioin number"
    }

    return $v
}

# このスクリプトは argv0 pkgroot subpkgs libpath
# という形式で呼び出す．
if { $argc != 3 } {
    error "Wrong # of args, shuld be $argv0 pkgroot libpath subpkgs"
}
set pkgroot [lindex $argv 0]
set libpath [lindex $argv 1]
set subpkgs [lindex $argv 2]

foreach mod $subpkgs {
    # ソースディレクトリ
    set src_dir [file join $pkgroot $mod]

    # このパッケージのバージョンを取り出す．
    set version [pkg_version $src_dir]

    # サブパッケージをインストールするディレクトリ
    set dst_dir [file join $libpath ${mod}${version}]

    # tcl のライブラリをインストールする．
    install_if_newer $src_dir $dst_dir
}

exit

# emacs を tcl モードにするためのおまじない
#;; Local Variables:
#;; mode: Tcl
#;; End:
