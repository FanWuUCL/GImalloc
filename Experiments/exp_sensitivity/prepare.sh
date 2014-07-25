# 2014.07.25 sometimes deadlock still happens. when it happens, set CONTINUE to 0 and CONTINUE_FROM to the number that causes deadlock, then carry on the script. FW

CURR_PATH=`pwd`
SUBJECT_PATH="$CURR_PATH/../../../Subjects"
ORIGINAL_PATH="$CURR_PATH/original"

MILU="$CURR_PATH/../../../Milu/bin/milu"
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="$SUBJECT_PATH/$2"
SUBJECT_PATH_TESTCASES="$SUBJECT_PATH_NAME/testcases"
#memory_path="$CURR_PATH/../../../memory/bin/memory"
memory_path="$SUBJECT_PATH_NAME/memory"
CONTINUE=1
CONTINUE_FROM=1

if [ $CONTINUE == 1 ]; then
	export LD_LIBRARY_PATH="/usr/lib/llvm-3.2/lib/"
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
else
	cd $CURR_PATH
	rm ../"$SUBJECT_NAME"_fitness.txt 
fi

for index in `ls milu_output/mutants/`
do
	if [ $CONTINUE == 1 ]; then
		cd milu_output/mutants/$index
		echo "Compiling mutant $index"
		if [ "$SUBJECT_NAME" == "cfrac" ]; then
		gcc -shared -fPIC -m32 src/malloc.c -o libmalloc.so
		else
		gcc -shared -fPIC src/malloc.c -o libmalloc.so
		fi
		cp $SUBJECT_PATH/bin/$SUBJECT_NAME subject
		cp -r $SUBJECT_PATH_TESTCASES/* .
		cp $memory_path memory
		./memory > fitness.txt
		cat *.s > results.txt
		if diff results.txt $ORIGINAL_PATH/results.txt >/dev/null ; then
		      #echo "tests: 1" >> fitness.txt
		      echo "1" >> fitness.txt
		      else
		            #echo "tests: 0" >> fitness.txt 
		            echo "0" >> fitness.txt 
		fi
		#echo "Mutant: $index" >> $CURR_PATH/fitness.txt
		echo "M$index" >> $CURR_PATH/fitness.txt
		#cat line >> $CURR_PATH/fitness.txt
		#echo "" >> $CURR_PATH/fitness.txt
		cat fitness.txt >> $CURR_PATH/fitness.txt
		cd ../../..
	elif [ $index == $CONTINUE_FROM ]; then
		CONTINUE=1
		echo M$index >> $CURR_PATH/fitness.txt
		echo 0 >> $CURR_PATH/fitness.txt
		echo 0 >> $CURR_PATH/fitness.txt
		echo 0 >> $CURR_PATH/fitness.txt
	fi

done
cd $CURR_PATH
#echo "Subject: $SUBJECT_NAME" >> ../fitness.txt
cat fitness.txt >> ../"$SUBJECT_NAME"_fitness.txt

