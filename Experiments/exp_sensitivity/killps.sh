#!/bin/bash


#Version 1. 2011 ICSE EvaClone study, YJ.
#Version 2. 2013 Memory study, YJ.
#Version 3. 2014 Adapt to memory study, FW.

while [ 1 -gt 0 ]
do
    pid=$(ps -eo pid,etime,comm | grep "subject" | grep -v "..:[0][0-2]" | awk '{print $1}' )
    if [ ! -z "$pid" ]; then
		for i in $pid; do
        	kill $i
		done
    fi
	sleep 2 
done

