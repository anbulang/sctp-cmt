set ytics 1500
set yrange[-1:60000]

set xlabel 'Second'
set xtics 0.1 autofreq
#set format x '%.1f'
set xtics out rotate by -80

set style line 1 lt 1 lw 3 pt 1
set style line 2 lt 2 lw 3 pt 1 
set style line 3 lt 3 lw 1 pt 1
set style line 4 lt 4 lw 1 pt 1


plot "./temp" using 1:3 title 'primary->cwnd' w l ls 1, \
"./temp" using 1:6 title 'retran->cwnd' w l ls 2, \
"./temp" using 1:4 title 'primary->ssthresh' w l ls 3, \
"./temp" using 1:7 title 'retran->ssthresh' w l ls 4

#plot "./temp" using 2 title 'primary->cwnd' with lines, \
#"./temp" using 3 title 'primary->ssthresh' with lines, \
#"./temp" using 5 title 'retran->cwnd' with lines, \
#"./temp" using 6 title 'retran->ssthresh' with lines

#plot "<(sed -n '1,8000p' ./temp)" using 2 title 'primary->cwnd' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 3 title 'primary->ssthresh' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 5 title 'retran->cwnd' with lines, \
#"<(sed -n '1,8000p' ./temp)" using 6 title 'retran->ssthresh' with lines


#using 2 every ::begin::end
#plot "<(sed -n '2000,4000p' ./temp)" using 2 title 'primary->cwnd' with lines, \
