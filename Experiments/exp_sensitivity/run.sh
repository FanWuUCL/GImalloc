echo "Run mutation sensitivity analysis experiment"
echo "November 25, 2013"


CURR_PATH=`pwd`
RUN_EXP="false"
killall killps.sh
./killps.sh &

if [ "$1" == "espresso" ]; then

SUBJECT_NAME=$1
SUBJECT_PATH_NAME="espresso-ab-1.0"
RUN_EXP="true"

fi

if [ "$1" == "cfrac" ]; then

SUBJECT_NAME=$1
SUBJECT_PATH_NAME="cfrac"
RUN_EXP="true"

fi

if [ "$1" == "gawk" ]; then

SUBJECT_NAME=$1
SUBJECT_PATH_NAME="gawk-4.0.2"
RUN_EXP="true"

fi

if [ "$1" == "space" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="space"
RUN_EXP="true"
fi

if [ "$1" == "hashmap" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="c_hashmap"
RUN_EXP="true"
fi

if [ "$1" == "fat_memory" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="fat_memory"
RUN_EXP="true"
fi

if [ "$1" == "flex" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="flex"
RUN_EXP="true"
fi

if [ "$1" == "bash" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="bash"
RUN_EXP="true"
fi

if [ "$1" == "sed" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="sed"
RUN_EXP="true"
fi

if [ "$1" == "abc" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="abc"
RUN_EXP="true"
fi

if [ "$1" == "mencoder" ]; then
SUBJECT_NAME=$1
SUBJECT_PATH_NAME="mencoder"
RUN_EXP="true"
fi

if [ "true" == "$RUN_EXP" ]; then

if [ ! -f "$SUBJECT_NAME"_fitness.txt ]; then
       touch "$SUBJECT_NAME"_fitness.txt 
       else
       rm "$SUBJECT_NAME"_fitness.txt 
       touch "$SUBJECT_NAME"_fitness.txt 
 fi


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




