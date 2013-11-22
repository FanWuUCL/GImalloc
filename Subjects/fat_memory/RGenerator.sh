#!/bin/bash

MALLOCTIMES=100000
# maximum malloc size is less than $MAXSIZE/32 MB
MAXSIZE=16
MAXLIFETIME=1000
FILENAME="mallocBase.txt"

if [ -f $FILENAME ]; then
	rm $FILENAME
fi

for i in $(seq 1 $MALLOCTIMES)
do
	echo $(($RANDOM%$MAXSIZE*32768+$RANDOM)) $(($RANDOM%$MAXLIFETIME)) >> $FILENAME
done

echo $FILENAME > testcases.txt

