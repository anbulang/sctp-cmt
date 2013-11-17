#!/bin/bash

ssh virtualbox ./set_nic.sh off
ssh virtualbox "dmesg>/tmp/dmesg"
scp virtualbox:/tmp/dmesg /tmp/vb_dmesg

cat /tmp/vb_dmesg				\
|tr -d '\000'					\
|grep "==>trx|"					\
|awk -F " " {'print $2 "\x7c" $6'}		\
|awk -F "|" '{gsub("\[", "", $1); gsub("\]", "", $1); print $1, $3, $4, $5,
$6, $8 ,$9, $10, $11}'				\
|sed 's/c=//g'					\
|sed 's/ssh=//g'				\
|sed 's/fly=//g'				\
>./temp

gnuplot draw.pl -persist
ssh virtualbox ./set_nic.sh on

#cat /tmp/vb_dmesg|
#filter the beginning '\0' out
#tr -d '\000'| 
#grep "==>trxpt|"
##filter out the non cwnd report logs
#awk -F " " {'print  $1 "\x7c" $5'}|
#awk -F "|" '{gsub("\[", "", $1); gsub("\]", "", $1); print $1, $3, $4, $5, $7, $8,$9}'|
##filter out 'cwnd='
#sed  's/cwnd=//g' |
##filter out 'ssthresh='
#sed 's/ssthresh=//g' >temp
