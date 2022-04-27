set opt(chan)           Channel/WirelessChannel  ;# channel type
set opt(prop)           Propagation/TwoRayGround ;# radio-propagation model
set opt(netif)          Phy/WirelessPhy          ;# network interface type
set opt(mac)            Mac/802_11               ;# MAC type
set opt(ifq)            Queue/DropTail/PriQueue  ;# interface queue type
set opt(ll)             LL                       ;# link layer type
set opt(ant)            Antenna/OmniAntenna      ;# antenna model
set opt(ifqlen)         50                       ;# max packet in ifq
set opt(nn)             9

set opt(adhocRouting)   AODV                     ;# routing protocol

set opt(x)              600    ;# x coordinate of topology
set opt(y)              600                      ;# y coordinate of topology

set opt(finish)              100                      ;# finish time

set ns [new Simulator]



# set up tracing
$ns use-newtrace
set tracefd  [open wireless.tr w]

set namfile [open wireless.nam w]

$ns trace-all $tracefd
$ns namtrace-all-wireless $namfile $opt(x) $opt(y)

# create  and define the topography object and layout
set topo [new Topography]

$topo load_flatgrid $opt(x) $opt(y)

# create an instance of General Operations Director, which keeps track of nodes and
# node-to-node reachability. The parameter is the total number of nodes in the simulation.

create-god $opt(nn)

# general node configuration

set chan1 [new $opt(chan)]


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
                 -macTrace ON

# create the bottom-row nodes as a node array $rownode(), and the moving node as $mover

set A [$ns node]
set B [$ns node]
set C [$ns node]
set D [$ns node]
set E [$ns node]
set F [$ns node]

set G [$ns node]
set H [$ns node]
set L [$ns node]

$A set X_ 200
$A set Y_ 500
$A set Z_ 0

$B set X_ 100
$B set Y_ 300
$B set Z_ 0

$C set X_ 300
$C set Y_ 400
$C set Z_ 0

$D set X_ 200
$D set Y_ 100
$D set Z_ 0

$E set X_ 300
$E set Y_ 200
$E set Z_ 0

$F set X_ 400
$F set Y_ 200
$F set Z_ 0

$G set X_ 400
$G set Y_ 400
$G set Z_ 0

$H set X_ 500
$H set Y_ 400
$H set Z_ 0

$L set X_ 500
$L set Y_ 200
$L set Z_ 0



# setup UDP connection, using CBR traffic

set tcp0 [new Agent/TCP]
set sink0 [new Agent/TCPSink]

set tcp1 [new Agent/TCP]
set sink1 [new Agent/TCPSink]

set tcp2 [new Agent/TCP]
set sink2 [new Agent/TCPSink]

set tcp3 [new Agent/TCP]
set sink3 [new Agent/TCPSink]

set tcp4 [new Agent/TCP]
set sink4 [new Agent/TCPSink]

set tcp5 [new Agent/TCP]
set sink5 [new Agent/TCPSink]

set tcp6 [new Agent/TCP]
set sink6 [new Agent/TCPSink]


$ns attach-agent $A $tcp0
$ns attach-agent $D $tcp1

$ns attach-agent $B $tcp2
$ns attach-agent $C $tcp3
$ns attach-agent $E $tcp4
$ns attach-agent $F $tcp5
$ns attach-agent $G $tcp6

$ns attach-agent $B $sink0
$ns attach-agent $C $sink1
$ns attach-agent $E $sink2
$ns attach-agent $G $sink3
$ns attach-agent $F $sink4

$ns attach-agent $H $sink5
$ns attach-agent $L $sink6

# A-C-E-G-L  => A-L
$ns connect $tcp0 $sink1
$ns connect $tcp3 $sink2
$ns connect $tcp4 $sink3
$ns connect $tcp6 $sink5

# A-C-G-H => A-H
$ns connect $tcp0 $sink1
$ns connect $tcp3 $sink3
$ns connect $tcp6 $sink5

#D-E-F-G-H => D-H
$ns connect $tcp1 $sink2
$ns connect $tcp4 $sink4
$ns connect $tcp5 $sink3
$ns connect $tcp6 $sink5

#D-E-F-L => D-L
$ns connect $tcp1 $sink2
$ns connect $tcp4 $sink4
$ns connect $tcp5 $sink6


set cbr1 [new Application/Traffic/CBR]
$cbr1 set packetSize_ 512
$cbr1 set rate_ 200Kb

$cbr1 attach-agent $tcp0
$cbr1 attach-agent $tcp1
$cbr1 attach-agent $tcp2
$cbr1 attach-agent $tcp3
$cbr1 attach-agent $tcp4
$cbr1 attach-agent $tcp5
$cbr1 attach-agent $tcp6

$ns at 0 "$cbr1 start"
$ns at $opt(finish) "$cbr1 stop"

# tell nam the initial node position (taken from node attributes)
# and size (supplied as a parameter)


$ns at $opt(finish) "finish"

proc finish {} {
    global ns tracefd namfile
    $ns flush-trace
    close $tracefd
    close $namfile
    exit 0
}

# begin simulation

$ns run
