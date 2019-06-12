#! /usr/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
#	$1. test name
#	$2. # threads
#	$3. # iterations per thread
#	$4. # lists
#	$5. # operations performed (threads x iterations x (ins + lookup + delete))
#	$6. run time (ns)
#	$7. run time per operation (ns)
#
# output:
#	lab2_list-1.png ... cost per operation vs threads and iterations
#	lab2_list-2.png ... threads and iterations that run (un-protected) w/o failure
#	lab2_list-3.png ... threads and iterations that run (protected) w/o failure
#	lab2_list-4.png ... cost per operation vs number of threads
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","

# how many threads/iterations we can run without failure (w/o yielding)
set title "List-1: Number of Threads vs Throughput"
set xlabel "Threads"
set logscale x 2
set ylabel "Throughput (operations/sec)"
set logscale y 10
set output 'lab2b_1.png'

# grep out only single threaded, un-protected, non-yield results
plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	title 'Mutex Lock' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/$7) \
	title 'Spin Lock' with linespoints lc rgb 'green'


# lab2-2.png
set title "List-2: Number of Threads vs Average Time Per Operation"
set xlabel "Threads"
set logscale x 2
set xrange [1:25]
set ylabel "Avg Time per Operation (ns)"
set logscale y 10
set output 'lab2b_2.png'

plot \
     "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
        title 'Wait-for-lock time' with linespoints lc rgb 'blue', \
     "< grep 'list-none-s,[0-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
        title 'Average time per operation' with linespoints lc rgb 'green'


# lab2-3.png
set title "List-3: Synchronization with Four Sub-Lists"
set xrange [0.75:40]
set yrange [0.75:100]
set xlabel "Threads"
set ylabel "# of Successful Iterations"
set logscale y 10
set output 'lab2b_3.png'
plot \
    "< grep 'list-id-none,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3)\
	with points lc rgb "blue" title "No Protection", \
    "< grep 'list-id-m,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	with points lc rgb "red" title "Mutex Lock", \
    "< grep 'list-id-s,[0-9]*,[0-9]*,4,' lab2b_list.csv" using ($2):($3) \
	with points lc rgb "green" title "Spin Lock"


# lab2-4.png
set title "List-4: Mutex Performance of Sub-Lists"
set xlabel "Threads"
set logscale x 2
unset xrange
set xrange [0.75:]
set ylabel "Operations per Second"
set logscale y
set yrange [10000: 10000000]
set output 'lab2b_4.png'
plot \
     "< grep -e 'list-none-m,[0-9][2]\\?,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7))\
        title 'List=1' with linespoints lc rgb 'red', \
     "< grep -e 'list-none-m,.*,1000,4' lab2b_list.csv" using ($2):(1000000000/($7))\
        title 'List=4' with linespoints lc rgb 'blue', \
     "< grep -e 'list-none-m,.*,1000,8' lab2b_list.csv" using ($2):(1000000000/($7))\
        title 'List=8' with linespoints lc rgb 'purple', \
     "< grep -e 'list-none-m,.*,1000,16' lab2b_list.csv" using ($2):(1000000000/($7))\
        title 'List=16' with linespoints lc rgb 'orange'

set title "List-5: Spin Lock Performance of Sub-Lists"
set xlabel "Number of Threads"
set xrange [0.75:]
set ylabel "Operations per Second"
set logscale x 2
set logscale y
set output 'lab2b_5.png'
plot \
     "< grep 'list-none-s,.*,1000,1,' lab2b_list.csv" \
        using ($2):(1000000000/($7)) \
	    title 'lists=1' with linespoints lc rgb 'red', \
     "< grep 'list-none-s,.*,1000,4,' lab2b_list.csv" \
        using ($2):(1000000000/($7)) \
	    title 'lists=4' with linespoints lc rgb 'blue', \
     "< grep 'list-none-s,.*,1000,8,' lab2b_list.csv" \
        using ($2):(1000000000/($7)) \
	    title 'lists=8' with linespoints lc rgb 'purple', \
     "< grep 'list-none-s,.*,1000,16,' lab2b_list.csv" \
        using ($2):(1000000000/($7)) \
	    title 'lists=16' with linespoints lc rgb 'orange'
