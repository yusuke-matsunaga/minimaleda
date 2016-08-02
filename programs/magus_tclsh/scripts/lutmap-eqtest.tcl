magus::new_network a
#set dpath "."
#set dpath "~/share/data/MCNC.blifdata"
#s13207.sbj
#s15850.sbj
#s38417.sbj
#s38584.sbj
#s9234.sbj
#too_large.sbj
set dpath "~/var/sandbox/ymtools-MAT-20080630/data/lutmap"
set clist {
b9.sbj
f51m.sbj
rot.sbj
9symml.sbj
des.sbj
vda.sbj
C432.sbj
C499.sbj
C880.sbj
C1355.sbj
C1908.sbj
C2670.sbj
C3540.sbj
C5315.sbj
C6288.sbj
C7552.sbj
s27.sbj
s208.sbj
s298.sbj
s344.sbj
s349.sbj
s382.sbj
s386.sbj
s400.sbj
s420.sbj
s444.sbj
s510.sbj
s526.sbj
s526n.sbj
s641.sbj
s713.sbj
s820.sbj
s832.sbj
s838.sbj
s953.sbj
s1196.sbj
s1238.sbj
s1423.sbj
s1488.sbj
s1494.sbj
s5378.sbj
s35932.sbj
}
#set clist {s838.sbj}
foreach name $clist {
puts "$name"
puts " --lutmap--"
magus::read_blif -network a $dpath/$name
magus::copy_network a b
magus::bidecomp -network b -mindepth
magus::lutmap::conv2sbj -network b
magus::lutmap::area_map -method dag -resub 5
magus::lutmap::conv2bnet -dst_network c
puts [magus::time]
puts " --compare--"
puts [magus::equiv -loglevel 2  a c]
#puts [magus::equiv -loglevel 2  -minisat a c]
puts [magus::time]
puts ""
}
