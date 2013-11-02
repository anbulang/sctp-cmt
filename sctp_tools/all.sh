#!/bin/bash

ssh virtualbox ./set_nic.sh off
ssh virtualbox "dmesg>/tmp/dmesg"
scp virtualbox:/tmp/dmesg /tmp/vb_dmesg
cat /tmp/vb_dmesg|
#filter the beginning '\0' out
tr -d '\000'| 
grep "sctp: cxz: sctp_outq_sack: trxpt"|
#filter out the non cwnd report logs
awk -F "|" {'print  $2 "\011" $3 "\011" $4 "\011" $6 "\011" $7 "\011" $8'}| 
#filter out 'cwnd='
sed  's/cwnd=//g' |
#filter out 'ssthresh='
sed 's/ssthresh=//g' >temp
gnuplot draw.pl -persist
#ssh virtualbox ./set_nic.sh on
