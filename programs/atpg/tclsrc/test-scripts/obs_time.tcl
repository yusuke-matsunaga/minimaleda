
set patnum 1

set dpath "~/share/data/"
set clist1 {
MCNC.blifdata/C432.blif
MCNC.blifdata/C499.blif
MCNC.blifdata/C880.blif
MCNC.blifdata/C1355.blif
MCNC.blifdata/C1908.blif
MCNC.blifdata/C2670.blif
MCNC.blifdata/C3540.blif
MCNC.blifdata/C5315.blif
MCNC.blifdata/C6288.blif
MCNC.blifdata/C7552.blif
ISCAS.s/SYNCH/s5378.blif
ISCAS.s/SYNCH/s9234.blif
ISCAS.s/SYNCH/s13207.blif
ISCAS.s/SYNCH/s15850.blif
ISCAS.s/SYNCH/s35932.blif
ISCAS.s/SYNCH/s38417.blif
ISCAS.s/SYNCH/s38584.blif
}

set clist2 {
itc99/b10/b10.bench
itc99/b11/b11.bench
itc99/b12/b12.bench
itc99/b13/b13.bench
itc99/b14/b14.bench
itc99/b15/b15.bench
itc99/b17/b17.bench
itc99/b18/b18.bench
itc99/b19/b19.bench
itc99/b20/b20.bench
itc99/b21/b21.bench
itc99/b22/b22.bench
}

::atpg::stopwatch timer1
set patnum 1000

proc foo {} {
    global patnum
    global timer1

    timer1 reset
    timer1 start
    ::atpg::calc_obs -init
    set t1 [timer1 get_utime]

    timer1 reset
    timer1 start
    ::atpg::calc_obs -loop $patnum
    set t2 [timer1 get_utime]

    timer1 reset
    timer1 start
    ::atpg::calc_obs -init-dss
    set t3 [timer1 get_utime]

    timer1 reset
    timer1 start
    ::atpg::calc_obs -loop $patnum
    set t4 [timer1 get_utime]
    puts "non-dss: $t2 ($t1)\tdss: $t4 ($t3)"
    puts ""
}

foreach name $clist1 {
    puts "$name"
    set fname [file join $dpath $name]
    ::atpg::read_blif $fname

    foo
}
set patnum 100
foreach name $clist2 {
    puts "$name"
    set fname [file join $dpath $name]
    ::atpg::read_iscas89 $fname

    foo
}

exit
