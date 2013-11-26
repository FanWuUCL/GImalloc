echo "Run mutation sensitivity analysis experiment"
echo "November 25, 2013"


CURR_PATH=`pwd`
RUN_EXP="false"

if [ ! -f fitness.txt ]; then
       touch fitness.txt 
       else
       rm fitness.txt
       touch fitness.txt
 fi

if [ "$1" == "espresso" ]; then

SUBJECT_NAME="espresso"
SUBJECT_PATH_NAME="espresso-ab-1.0"
RUN_EXP="true"

fi

if [ "$1" == "cfrac" ]; then

SUBJECT_NAME="cfrac"
SUBJECT_PATH_NAME="cfrac"
RUN_EXP="true"

fi

if [ "$1" == "gawk" ]; then

SUBJECT_NAME="gawk"
SUBJECT_PATH_NAME="gawk-4.0.2"
RUN_EXP="true"

fi

if [ "true" == "$RUN_EXP" ]; then

if [ ! -d "$SUBJECT_NAME" ]; then
    mkdir $SUBJECT_NAME 
fi
cp prepare.sh $SUBJECT_NAME 
cd $SUBJECT_NAME 
./prepare.sh $SUBJECT_NAME $SUBJECT_PATH_NAME

cd $CURR_PATH
#rm -r $SUBJECT_NAME

else
    echo "Unknown experiment!"
fi




