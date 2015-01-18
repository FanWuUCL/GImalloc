if [ $# -lt 1 ]; then
	echo "usage: motivatingExample.sh subjectName"
	exit 0
fi

subject=$1

while read line; do
	if [[ "$line" == Ori* ]]; then
		read memory
		read time
		echo $line $time $memory
	fi

	if [[ "$line" == M572* || "$line" == M596* || "$line" == M586* || "$line" == M583* || "$line" == M579* || "$line" == M590* || "$line" == M578* || "$line" == M595* || "$line" == M584* || "$line" == M580* || "$line" == M594* || "$line" == M574* || "$line" == M582* || "$line" == M576* || "$line" == M592* || "$line" == M589* || "$line" == M588* || "$line" == M573* || "$line" == M575* || "$line" == M585* ]]; then
		read memory
		read time
		echo $line $time $memory
	fi
done	< ${subject}_fitness.txt
