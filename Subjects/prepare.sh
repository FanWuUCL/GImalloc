#!/bin/bash


mkdir bin
# Prepare subject diff
#echo "Prepare subject: Diff 3.2"
#cd diffutils-3.2 
#./configure
#make
#cp ./src/diff ../bin


# Prepare subject fat_malloc 
echo "Prepare subject: fat_malloc"
cd fat_memory
make
cp fat_memory ../bin
cp libmalloc.so ../bin

