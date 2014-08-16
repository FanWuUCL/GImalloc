if [ -d curr ]; then
	cd curr
	rm *
	cd ..
fi

containsElement () {
  local e
  for e in "run.sh" "runall.sh" "callgrind.sh" "clean.sh" "malloc.c" "refine.sh" "refineall.sh"; do [[ "$e" == "$1" ]] && return 0; done
  return 1
}

for f in `ls`; do
	if [ -f $f ]; then
		containsElement $f
		if [ $? -eq 1 ]; then
			rm $f
			#echo $f
		fi
	fi
done
