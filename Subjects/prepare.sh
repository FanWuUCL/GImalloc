#!/bin/bash

curr_path=`pwd`

if [ ! -d "$DIRECTORY" ]; then
    mkdir bin
fi

# Prepare subject diff
#echo "Prepare subject: Diff 3.2"
#cd diffutils-3.2 
#./configure
#make
#cp ./src/diff ../bin


# Prepare subject fat_malloc 
cd $curr_path
echo "Prepare subject: fat_malloc"
cd fat_memory
make
cp fat_memory ../bin
cp libmalloc.so ../bin

# Prepare subject ed 
cd $curr_path
echo "Prepare subject: ed"
cd ed-1.9 
./configure
make
cp ed ../bin
cp libmalloc.so ../bin


# Prepare subject espresso 
cd $curr_path
echo "Prepare subject: Espresso"
cd espresso-ab-1.0 
./configure
cd src
make
cp espresso ../../bin
#cp libmalloc.so ../bin

