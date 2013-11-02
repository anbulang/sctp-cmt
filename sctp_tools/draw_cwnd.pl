plot "./temp" using 2 title 'primary->cwnd' with lines, \
"./temp" using 5 title 'retran->cwnd' with lines

#plot "<(sed -n '1,8000p' ./temp)" using 2 title 'primary->cwnd' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 3 title 'primary->ssthresh' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 5 title 'retran->cwnd' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 6 title 'retran->ssthresh' with lines


#using 2 every ::begin::end
#plot "<(sed -n '2000,4000p' ./temp)" using 2 title 'primary->cwnd' with lines, \
