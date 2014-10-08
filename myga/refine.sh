if [ $# -lt 1 ]; then
	echo "Invoke script followed by subject name"
	exit 0
fi

subject=$1
shift

# prepare environment
./clean.sh

if [ ! $subject == "cfrac" ]; then
	gcc -shared -fPIC -O3 -o libmalloc.so malloc.c -DINSTRUMENT2=1
else
	gcc -m32 -shared -fPIC -O3 -o libmalloc.so malloc.c -DINSTRUMENT2=1
fi

echo "Prepareing experiment environment on $subject"
cp $subject/src/$subject subject
cp $subject/subjectSetting.h src/subjectSetting.h
cp $subject/testcases.txt testcases.txt
if [ -d testcases ]; then
	rm -r testcases
fi
if [ $subject == "bash" ]; then
	cp $subject/testcases/* .
elif [ $subject == "abc" ]; then
	cp $subject/testcases/* .
elif [ $subject == "mencoder" ]; then
	cp $subject/testcases/* .
else
	cp -r $subject/testcases .
fi
if [ -d curr ]; then
	rm -r curr
fi
mkdir curr
	
# compile and run myga
echo "Running refinement on >>>$subject<<<"
cd src
make
cp myga ../myga
cp memory ../memory
cd ..
echo "memory"
./memory
	
# store result

cp -r curr $subject/
