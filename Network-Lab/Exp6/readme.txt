> create a 50mb file.dat 
base64 /dev/urandom | head -c 52428800 > file.dat

> make a txt file 
ifstat -l -t -i lo 0.1>capture.txt

> open gnuplot
Terminal type is now 'qt'
gnuplot> set xdata time
gnuplot> set timefmt "%H:%M:%S"
gnuplot> set xrange["START_TIME":"END_TIME"]        //can be seen from capture.txt 
gnuplot> plot "capture.txt" using 1:2 with lines 
