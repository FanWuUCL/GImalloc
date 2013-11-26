#/bin/bash

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


# Prepare subject cfrac 
cd $curr_path
echo "Prepare subject: Cfrac"
cd cfrac 
make
cp cfrac ../bin

# Prepare subject gawk 
cd $curr_path
echo "Prepare subject: Gawk"
cd gawk-4.0.2
./configure
cd src
make
cp gawk ../bin

# Prepare subject space 
cd $curr_path
echo "Prepare subject: space"
cd space 
make
cp space ../bin

# Prepare subject hashmap 
cd $curr_path
echo "Prepare subject: hashmap"
cd c_hashmap 
make
cp hashmap ../bin



