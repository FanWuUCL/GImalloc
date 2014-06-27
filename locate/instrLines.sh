name=malloc.c

i=0
while read line; do
	((i+=1))
	if [[ "$line" == *preAddr* ]] || [[ "$line" == *postAddr* ]] || ([[ "$line" == *fprintf* ]] && ([[ "$line" == *start* ]] || [[ "$line" == *memory* ]])); then
		echo "$i, "
	fi
done < $name

