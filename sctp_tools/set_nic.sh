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
    sudo tc qdisc del dev eth0 root netem
    sudo tc qdisc del dev eth1 root netem
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
#    tc qdisc add dev eth0 root netem delay 200ms 40ms 25% loss 15.3% 25% duplicate 1% corrupt 0.1% reorder 25% 50%
#    tc qdisc add dev eth1 root netem delay 200ms 40ms 25% loss 15.3% 25% duplicate 1% corrupt 0.1% reorder 25% 50%

    # long fat channel
    sudo tc qdisc add dev eth0 root handle 1:0 netem #  delay 40ms 
    sudo tc qdisc add dev eth0 parent 1:0 handle 2:0 tbf burst 40kb limit 4kb rate 1mbps

    # long fat channel
    sudo tc qdisc add dev eth1 root handle 4:0 netem # delay 40ms
    sudo tc qdisc add dev eth1 parent 4:0 handle 5:0 tbf burst 40kb limit 4kb rate 1mbps
    
    # long fat channel
#    sudo tc qdisc add dev eth0 root handle 1:0 netem  delay 400ms 40ms 10%
#    sudo tc qdisc add dev eth0 parent 1:0 handle 2:0 tbf burst 40kb limit 40kb rate 1mbps

    # long fat channel
#    sudo tc qdisc add dev eth1 root handle 1:0 netem  delay 400ms 40ms 10%
#    sudo tc qdisc add dev eth1 parent 1:0 handle 2:0 tbf burst 40kb limit 40kb rate 1mbps
    
    # long dealy and low bd channel
#    sudo tc qdisc add dev eth1 root handle 1:0 netem  delay 40ms 4ms 2%
#    sudo tc qdisc add dev eth1 parent 1:0 handle 2:0 tbf burst 20kb limit 20kb rate 256kbps
    exit
fi

####
EOF
#####
