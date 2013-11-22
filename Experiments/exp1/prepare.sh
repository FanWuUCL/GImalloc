memory_path="../../../../../memory/bin/memory"
MILU="../../Milu/bin/milu"
SUBJECT_PATH="../../../../../Subjects/fat_memory"

 $MILU -m mop.txt -f func_list.txt malloc_pp.c 

for index in `ls milu_output/mutants/`
do
	cd milu_output/mutants/$index
	pwd
	gcc -shared -fPIC src/malloc_pp.c -o libmalloc.so
	cp $SUBJECT_PATH/fat_memory subject
	cp $SUBJECT_PATH/mallocBase.txt mallocBase.txt
	cp $SUBJECT_PATH/testcases.txt testcases.txt
	cp $memory_path memory
	./memory > result.txt
	cd ../../..
done
