CURR_PATH=`pwd`
memory_path="$CURR_PATH/../../../memory/bin/memory"
SUBJECT_PATH="$CURR_PATH/../../../Subjects"
ORIGINAL_PATH="$CURR_PATH/original"

MILU="$CURR_PATH/../../../Milu/bin/milu"
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="$SUBJECT_PATH/$2"
SUBJECT_PATH_TESTCASES="$SUBJECT_PATH_NAME/testcases"


echo $MILU
echo "Experiment: mutation sensitivity analysis"
echo "Subject: $SUBJECT_NAME"
echo "V. Feb 20, 2014"

echo "Compiling original"

cd $CURR_PATH
if [ ! -f fitness.txt ]; then
       touch fitness.txt 
       else
       rm fitness.txt
       touch fitness.txt
 fi

if [ ! -d "original" ]; then
    mkdir original
fi

cd $ORIGINAL_PATH 
if [ "$SUBJECT_NAME" == "cfrac" ]; then
gcc -shared -fPIC -m32 ../../malloc.c -o libmalloc.so
else
gcc -shared -fPIC ../../malloc.c -o libmalloc.so
fi
cp $SUBJECT_PATH/bin/$SUBJECT_NAME subject
cp -r $SUBJECT_PATH_TESTCASES/* .
cp $memory_path memory
./memory > fitness.txt
cat *.s > results.txt
echo "Ori " >> $CURR_PATH/fitness.txt
cat fitness.txt >> $CURR_PATH/fitness.txt

cd $CURR_PATH

echo "Start mutation analysis"
$MILU -h -m ../mop.txt -f ../func_list.txt ../malloc.c

