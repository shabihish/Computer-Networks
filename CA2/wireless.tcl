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

set opt(x)              1050    ;# x coordinate of topology
set opt(y)              1050                      ;# y coordinate of topology

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
                 # -txpower 0.02 \
                 # -rxpower 0.02





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

$A set X_ 350
$A set Y_ 875
$A set Z_ 0

$B set X_ 175
$B set Y_ 525
$B set Z_ 0

$C set X_ 525
$C set Y_ 700
$C set Z_ 0

$D set X_ 350
$D set Y_ 175
$D set Z_ 0

$E set X_ 525
$E set Y_ 350
$E set Z_ 0

$F set X_ 700
$F set Y_ 350
$F set Z_ 0

$G set X_ 700
$G set Y_ 700
$G set Z_ 0

$H set X_ 875
$H set Y_ 700
$H set Z_ 0

$L set X_ 875
$L set Y_ 350
$L set Z_ 0


set tcp [new Agent/TCP]
set sink [new Agent/TCPSink]
$ns attach-agent $A $tcp
$ns attach-agent $H $sink
$ns connect $tcp $sink

set cbr1 [new Application/Traffic/CBR]
$cbr1 set interval_ 500us
$cbr1 set packetSize_ 10
$cbr1 set rate_ 200Kb
$cbr1 set random_ 1


$cbr1 attach-agent $tcp

$ns at 1.0 "$cbr1 start"


set tcp2 [new Agent/TCP]
set sink2 [new Agent/TCPSink]
$ns attach-agent $D $tcp2
$ns attach-agent $L $sink2
$ns connect $tcp2 $sink2

set cbr2 [new Application/Traffic/CBR]
$cbr2 set interval_ 500us
$cbr2 set packetSize_ 10
$cbr2 set rate_ 200Kb
$cbr2 set random_ 1


$cbr2 attach-agent $tcp2
$ns at 1.0 "$cbr2 start"



# setup UDP connection, using CBR traffic

# set tcp0 [new Agent/TCP]
# # set tcp01 [new Agent/TCP]
# set sink0 [new Agent/TCPSink]
# # set sink01 [new Agent/TCPSink]

# set tcp1 [new Agent/TCP]
# # set tcp11 [new Agent/TCP]

# set sink1 [new Agent/TCPSink]
# # set sink11 [new Agent/TCPSink]

# $ns attach-agent $A $tcp0
# # $ns attach-agent $A $tcp01
# $ns attach-agent $D $tcp1
# # $ns attach-agent $D $tcp11

# $ns attach-agent $H $sink0
# # $ns attach-agent $H $sink1
# $ns attach-agent $L $sink1
# # $ns attach-agent $L $sink11


#$ns connect $tcp0 $sink0

# $ns connect $tcp0 $sink1

# #$ns connect $tcp1 $sink1

# $ns connect $tcp1 $sink1


# set cbr1 [new Application/Traffic/CBR]
# $cbr1 set interval_ 500us
# # $cbr1 set packetSize_ 10
# # $cbr1 set rate_ 200Kb
# $cbr1 set random_ 1

# $cbr1 attach-agent $tcp0
# #$cbr1 attach-agent $tcp11

# set cbr2 [new Application/Traffic/CBR]
# $cbr2 set interval_ 800us
# # $cbr2 set packetSize_ 10
# # $cbr2 set rate_ 200Kb
# $cbr2 set random_ 1

# $cbr2 attach-agent $tcp1
#$cbr2 attach-agent $tcp0


#$ns at 0 "$cbr1 start"
# $ns at 0 "$cbr1 stop"

#$ns at 0 "$cbr2 start"
# $ns at $opt(finish) "$cbr2 stop"

# tell nam the initial node position (taken from node attributes)
# and size (supplied as a parameter)


$ns at $opt(finish) "finish"

$ns initial_node_pos $A 20
$ns initial_node_pos $B 20
$ns initial_node_pos $C 20
$ns initial_node_pos $D 20
$ns initial_node_pos $E 20
$ns initial_node_pos $F 20
$ns initial_node_pos $G 20
$ns initial_node_pos $H 20
$ns initial_node_pos $L 20


proc finish {} {
    global ns tracefd namfile
    $ns flush-trace
    close $tracefd
    close $namfile
    exit 0
}

# begin simulation

$ns run
