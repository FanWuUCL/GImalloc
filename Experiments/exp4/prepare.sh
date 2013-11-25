CURR_PATH=`pwd`
memory_path="$CURR_PATH/../../memory/bin/memory"
SUBJECT_PATH="$CURR_PATH/../../Subjects"
ORIGINAL_PATH="$CURR_PATH/original"

MILU="../../Milu/bin/milu"
SUBJECT_NAME="cfrac"
SUBJECT_PATH_NAME="$SUBJECT_PATH/cfrac"
SUBJECT_PATH_TESTCASES="$SUBJECT_PATH_NAME/testcases"


echo "Experiment: mutation sensitivity analysis"
echo "Subject: Espresso"
echo "November 25, 2013"

echo "Compiling original"

cd $CURR_PATH
if [ ! -f fitness.txt ]; then
       touch fitness.txt 
       else
       rm fitness.txt
       touch fitness.txt
 fi

cd $ORIGINAL_PATH 
gcc -shared -fPIC -m32 malloc.c -o libmalloc.so
cp $SUBJECT_PATH/bin/$SUBJECT_NAME subject
cp -r $SUBJECT_PATH_TESTCASES/* .
cp $memory_path memory
./memory > fitness.txt
cat *.s > results.txt
echo "Original: " >> $CURR_PATH/fitness.txt
cat fitness.txt >> $CURR_PATH/fitness.txt

cd $CURR_PATH

$MILU -m mop.txt -f func_list.txt malloc.c 

for index in `ls milu_output/mutants/`
do
	cd milu_output/mutants/$index
    echo "Compiling mutant $index"
	gcc -shared -fPIC -m32 src/malloc.c -o libmalloc.so
    cp $SUBJECT_PATH/bin/$SUBJECT_NAME subject
    cp -r $SUBJECT_PATH_TESTCASES/* .
	cp $memory_path memory
	./memory > fitness.txt
    cat *.s > results.txt
    if diff results.txt $ORIGINAL_PATH/results.txt >/dev/null ; then
          echo "tests: 1" >> fitness.txt
          else
                echo "tests: 0" >> fitness.txt 
    fi
    echo "Mutant: $index" >> $CURR_PATH/fitness.txt
    cat fitness.txt >> $CURR_PATH/fitness.txt
	cd ../../..

done
