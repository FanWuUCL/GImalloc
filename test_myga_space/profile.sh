#!/bin/bash

TIMES=100
RESULTFILE="result.txt"

for i in $(seq 1 $TIMES)
do
	echo testing $i
	./profiler >> $RESULTFILE
done

gawk 'BEGIN {sum1=0; sum2=0} /memory:/ {printf "%d\n",$2}' $RESULTFILE > old.txt
gawk 'BEGIN {sum1=0; sum2=0} /memory_new:/ {printf "%d\n",$2}' $RESULTFILE > new.txt

rm $RESULTFILE
