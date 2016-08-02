
#lappend auto_path "~/var/sandbox/ymtools-MAT-SEAL-20080221/atpg/tclsrc/"

#package require Atpg_Utils 1.0

# 使用例
set fname $argv
set patnum 1000
::atpg::read_blif $fname
::atpg::set_fault
puts [::atpg::time]

::atpg::rtpg
puts "rtpg end"
puts "\# of total faults    : $::atpg::info(rep_fault_num)"
puts "\# of detedted faults : $::atpg::info(det_fault_num)"
puts "\# of remain faults   : $::atpg::info(remain_fault_num)"
puts [::atpg::time]

::atpg::dtpg
puts "dtpg end"
puts "\# of total faults    : $::atpg::info(rep_fault_num)"
puts "\# of detedted faults : $::atpg::info(det_fault_num)"
puts "\# of redundant faults: $::atpg::info(undet_fault_num)"
puts "\# of remain faults   : $::atpg::info(remain_fault_num)"
puts [::atpg::time]

exit
