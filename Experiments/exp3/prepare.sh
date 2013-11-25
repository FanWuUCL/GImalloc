memory_path="../../../../../memory/bin/memory"
MILU="../../Milu/bin/milu"
SUBJECT_PATH="../../../../../Subjects"
SUBJECT_NAME="espresso"
SUBJECT_PATH_NAME="$SUBJECT_PATH/espresso-ab-1.0"
SUBJECT_PATH_TESTCASES="$SUBJECT_PATH_NAME/testcases"

echo "Experiment: mutation sensitivity analysis"
echo "Subject: Espresso"
echo "November 25, 2013"

$MILU -m mop.txt -f func_list.txt malloc.c 

for index in `ls milu_output/mutants/`
do
	cd milu_output/mutants/$index
	echo "Compiling mutant $index"
	gcc -shared -fPIC src/malloc.c -o libmalloc.so
    cp $SUBJECT_PATH/bin/$SUBJECT_NAME subject
    cp -r $SUBJECT_PATH_TESTCASES/* .
#	cp $SUBJECT_PATH/fat_memory subject
#	cp $SUBJECT_PATH/mallocBase.txt mallocBase.txt
#	cp $SUBJECT_PATH/testcases.txt testcases.txt
	cp $memory_path memory
	./memory > result.txt
	cd ../../..
done
