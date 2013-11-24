#!/bin/bash

MALLOCTIMES=100000
# maximum malloc size is less than $MAXSIZE/32 MB
MAXSIZE=16
MAX_LITTLE_SIZE=2048
MAXLIFETIME=1000
MAX_LITTLE_LIFETIME=100
ratio=25000
FILENAME="mallocBase.txt"

if [ -f $FILENAME ]; then
	rm $FILENAME
fi

for i in $(seq 1 $MALLOCTIMES)
do
	if [ $RANDOM -gt $ratio ]
	then
		echo $(($RANDOM%$MAXSIZE*32768+$RANDOM)) $(($RANDOM%$MAXLIFETIME)) >> $FILENAME
	else
		echo $(($RANDOM%$MAX_LITTLE_SIZE)) $(($RANDOM%$MAX_LITTLE_LIFETIME)) >> $FILENAME
	fi
done

echo $FILENAME > testcases.txt

