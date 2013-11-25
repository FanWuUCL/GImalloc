echo "Run mutation sensitivity analysis experiment"
echo "November 25, 2013"


CURR_PATH=`pwd`

if [ ! -f fitness.txt ]; then
       touch fitness.txt 
       else
       rm fitness.txt
       touch fitness.txt
 fi

cd espresso
./run.sh
cd $CURR_PATH
