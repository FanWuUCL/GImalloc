#!/bin/bash


#Version 1. 2011 ICSE EvaClone study, YJ.
#Version 2. 2013 Memory study, YJ.

while [ 1 -gt 0 ]
    do
        pid=$(ps -eo pid,etime,comm | grep "subject" | grep -v "..:[0][0-3]" | awk '{print $1}' )
        if [ ! -z $pid ]; then
            kill $pid
        fi
sleep   3 
done

