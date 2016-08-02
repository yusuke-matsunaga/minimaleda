
#lappend auto_path "~/var/sandbox/ymtools-MAT-SEAL-20080221/atpg/tclsrc/"

package require Atpg_Utils 1.0

proc ppsfp_test { fname patnum module_list } {
    ::atpg::time

    set fd [open "dump_fsim_naive" w]
    puts "fsim_naive set"
    ::atpg::fsim_naive::read_blif $fname
    ::atpg::fsim_naive::set
    ::atpg::fsim_naive::set_fault
    puts $fd [::atpg::fsim_naive::dump]
    puts [::atpg::time]
    close $fd

    ::atpg::fsim_naive::set_random_vector -init 12345
    foreach module $module_list {
	set fd [open "dump_${module}" w]
	puts "$module set"
	::atpg::${module}::read_blif $fname
	::atpg::${module}::set
	::atpg::${module}::set_fault
	set fd [open "dump_${module}" w]
	puts $fd [::atpg::${module}::dump]
	puts [::atpg::time]
	close $fd
	::atpg::${module}::set_random_vector -init 12345
    }

    for { set i 0 } { $i < $patnum } { incr i } {
	set fd [open "out_fsim_naive" w]
	::atpg::fsim_naive::set_random_vector
	set x0 [::atpg::fsim_naive::ppsfp -trace]
  	puts $fd $x0
	close $fd

	set mismatch 0
	foreach module $module_list {
	    set fd [open "out_${module}" w]
	    ::atpg::${module}::set_random_vector
	    set x1 [::atpg::${module}::ppsfp -trace]
	    puts $fd $x1
	    close $fd

	    if [string compare $x0 $x1] {
		puts "fsim_naive and $module differ"
		incr mismatch
	    }
	}
	if { $mismatch > 0 } {
		break
	}
    }
}

# 使用例
set fname $argv
set patnum 1000
set module_list {
    "fsim_simple1"
    "fsim_simple2"
    "fsim_simple3"
    "fsim_simple5"
    "fsim_simple6"
}

ppsfp_test $fname $patnum $module_list
exit
