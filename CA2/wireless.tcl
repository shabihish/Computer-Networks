if { $argc != 2 } {
        puts "Invalid usage!"
        puts "Please try again."
	abort()
    }
set error_rate [lindex $argv 0]
set bandwidth [lindex $argv 1]


puts "error rate : $error_rate" 
puts "band width : $bandwidth Mb/Sec" 



set opt(chan)           Channel/WirelessChannel  ;# channel type
set opt(prop)           Propagation/TwoRayGround ;# radio-propagation model
set opt(netif)          Phy/WirelessPhy          ;# network interface type
set opt(mac)            Mac/802_11               ;# MAC type
set opt(ifq)            Queue/DropTail/PriQueue  ;# interface queue type
set opt(ll)             LL                       ;# link layer type
set opt(ant)            Antenna/OmniAntenna      ;# antenna model
set opt(ifqlen)         50                       ;# max packet in ifq
set opt(nn)             9                        ;# num of nodes
set opt(adhocRouting)   AODV                     ;# routing protocol
set opt(x)              1050                     ;# x coordinate of topology
set opt(y)              1050                     ;# y coordinate of topology
set opt(finish)              100.0               ;# finish time
set ns [new Simulator]                           ;# Simulator object

set name wireless

# Setup the output .tr files
set th0 [open throughput0.tr w]
set th1 [open throughput1.tr w]

$ns use-newtrace
set tracefd  [open $name.tr w]
set namfile [open $name.nam w]

$ns trace-all $tracefd
$ns namtrace-all-wireless $namfile $opt(x) $opt(y)

# Mac/802_11 set dataRate_                $val(datarate)
Mac/802_11 set bandwidth_ $bandwidth
Mac/802_11 set RTSThreshold_    1

set topo [new Topography]
$topo load_flatgrid $opt(x) $opt(y)

create-god $opt(nn)

set chan1 [new $opt(chan)]

proc UniformErr {} {
    set err [new ErrorModel]
    $err unit packet ;#it can be bit too
    $err set rate_ &error_rate
    return $err
}

$ns node-config -adhocRouting $opt(adhocRouting) \
                 -llType $opt(ll) \
                 -macType $opt(mac) \
                 -ifqType $opt(ifq) \
                 -ifqLen $opt(ifqlen) \
                 -antType $opt(ant) \
                 -propType $opt(prop) \
                 -phyType $opt(netif) \
                 -channel $chan1 \
                 -topoInstance $topo \
                 #-wiredRouting OFF \
                 -agentTrace ON \
                 -routerTrace ON \
                 -macTrace ON \
                 -IncomingErrProc UniformErr
                 #-txpower 0.002 \
                 #-rxpower 0.002


# Create the nodes
set A [$ns node]
set B [$ns node]
set C [$ns node]
set D [$ns node]
set E [$ns node]
set F [$ns node]
set G [$ns node]
set H [$ns node]
set L [$ns node]

# Set nodes' positions
$A set X_ 375
$A set Y_ 700
$A set Z_ 0

$B set X_ 200
$B set Y_ 437.5
$B set Z_ 0

$C set X_ 525
$C set Y_ 525
$C set Z_ 0

$D set X_ 375
$D set Y_ 175
$D set Z_ 0

$E set X_ 525
$E set Y_ 350
$E set Z_ 0

$F set X_ 700
$F set Y_ 350
$F set Z_ 0

$G set X_ 700
$G set Y_ 525
$G set Z_ 0

$H set X_ 875
$H set Y_ 525
$H set Z_ 0

$L set X_ 875
$L set Y_ 350
$L set Z_ 0

set tcp00 [new Agent/TCP]
set tcp01 [new Agent/TCP]
set tcp10 [new Agent/TCP]
set tcp11 [new Agent/TCP]

set sink00 [new Agent/TCPSink]
set sink01 [new Agent/TCPSink]
set sink10 [new Agent/TCPSink]
set sink11 [new Agent/TCPSink]

$ns attach-agent $A $tcp00
$ns attach-agent $A $tcp01

$ns attach-agent $D $tcp10
$ns attach-agent $D $tcp11

$ns attach-agent $H $sink00
$ns attach-agent $H $sink01

$ns attach-agent $L $sink10
$ns attach-agent $L $sink11

$ns connect $tcp00 $sink00
$ns connect $tcp01 $sink10

$ns connect $tcp10 $sink01
$ns connect $tcp11 $sink11

#not working
# set err [new ErrorModel]

# $err set rate_ 0.001
# $ns_ lossmodel $err $A $C
# #$ns lossmodel $err $tcp10 $E


set cbr00 [new Application/Traffic/CBR]
set cbr01 [new Application/Traffic/CBR]
set cbr10 [new Application/Traffic/CBR]
set cbr11 [new Application/Traffic/CBR]

$cbr00 set rate_ 200Mb
$cbr01 set rate_ 200Mb
$cbr10 set rate_ 200Mb
$cbr11 set rate_ 200Mb

$cbr00 set packetSize_ 64Kb
$cbr01 set packetSize_ 64Kb
$cbr10 set packetSize_ 64Kb
$cbr11 set packetSize_ 64Kb

$cbr00 set random_ 1
$cbr01 set random_ 1
$cbr10 set random_ 1
$cbr11 set random_ 1

#$cbr1 set interval_ 500us
#$cbr1 set packetSize_ 64
#$cbr1 set rate_ 200Kb
#$cbr1 set random_ 1

$cbr00 attach-agent $tcp00
$cbr01 attach-agent $tcp01
$cbr10 attach-agent $tcp10
$cbr11 attach-agent $tcp11


$ns at 0.0 "initialize"

$ns at 0.0 "$cbr00 start"
$ns at 0.0 "$cbr11 start"
$ns at 0.0 "$cbr10 start"
$ns at 0.0 "$cbr01 start"


$ns initial_node_pos $A 50
$ns initial_node_pos $B 50
$ns initial_node_pos $C 50
$ns initial_node_pos $D 50
$ns initial_node_pos $E 50
$ns initial_node_pos $F 50
$ns initial_node_pos $G 50
$ns initial_node_pos $H 50
$ns initial_node_pos $L 50

set time 1.0

proc initialize {} {
	global sink00 sink01 sink10 sink11 ns time

	$sink00 set bytes_ 0
	$sink01 set bytes_ 0
	$sink10 set bytes_ 0
	$sink11 set bytes_ 0

	set now [$ns now]
    $ns at [expr $now+$time] "record"

}

proc record {} {
	    global sink00 sink01 sink10 sink11
		global th0 th1 f2 time ns packet_size
		#Set the time after which the procedure should be called again

		#How many bytes have been received by the traffic sinks?
		set bw00 [$sink00 set bytes_]
		set bw01 [$sink01 set bytes_]
		set bw10 [$sink10 set bytes_]
		set bw11 [$sink11 set bytes_]

		#Get the current time
		set now [$ns now]

		#Calculate the bandwidth (in MBytes/s) and write it to the files
		set throughput0 [expr ($bw00*8)/$time + ($bw01*8)/$time]
		set throughput1 [expr ($bw10*8)/$time + ($bw11*8)/$time]

		puts $th0 "$now [expr $throughput0]"
		puts $th1 "$now [expr $throughput1]"

		#Reset the bytes_ values on the traffic sinks
		$sink00 set bytes_ 0
		$sink01 set bytes_ 0
		$sink10 set bytes_ 0
		$sink11 set bytes_ 0

		#Re-schedule the procedure
		$ns at [expr $now+$time] "record"
	}

proc finish {} {
    global ns tracefd namfile th0 th1 name
    $ns flush-trace

    close $namfile
    close $tracefd

    close $th0
    close $th1
    exec nam $name.nam
    exit 0
}

$ns at $opt(finish) "finish"

$ns run
