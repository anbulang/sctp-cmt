#!/bin/bash
#
## Calomel.org 
## https://calomel.org/network_loss_emulation.html
#
# Usage: ./calomel_netem.sh $argument

if [ $# -eq 0 ]
then
    echo ""
    echo "    Calomel.org       calomel_netem.sh"
    echo "------------------------------------------"
    echo "off     = clear all netem profiles"
    echo "show    = show active profiles"
    echo "on   = enable Samoa netem profile"
    echo ""
    exit
fi

if [ $1 = "off" ]
then
    echo "netem profile off"
    sudo tc qdisc del dev eth0 root 
    sudo tc qdisc del dev eth1 root 
    exit
fi

if [ $1 = "show" ]
then
    echo "show netem profile"
    tc qdisc show dev eth0
    tc qdisc show dev eth1
    exit
fi

if [ $1 = "on" ]
then
    echo "netem profile on"
    sudo tc qdisc add dev eth0 root tbf rate 0.5mbit		\
	    burst 5kb latency 70ms peakrate 1mbit	\
	    minburst 1540
    sudo tc qdisc add dev eth1 root tbf rate 0.5mbit		\
	    burst 5kb latency 70ms peakrate 1mbit	\
	    minburst 1540
    exit
fi

####
EOF
#####
