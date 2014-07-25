#/bin/bash

curr_path=`pwd`

if [ ! -d "$DIRECTORY" ]; then
    mkdir bin
fi

gcc -shared -fPIC malloc.c -o libmalloc.so

# Prepare subject diff
#echo "Prepare subject: Diff 3.2"
#cd diffutils-3.2 
#./configure
#make
#cp ./src/diff ../bin


# Prepare subject fat_malloc 
#cd $curr_path
#echo "Prepare subject: fat_malloc"
#cd fat_memory
#make
#cp fat_memory ../bin
#cp libmalloc.so ../bin

# Prepare subject ed 
#cd $curr_path
#echo "Prepare subject: ed"
#cd ed-1.9 
#./configure
#make
#cp ed ../bin
#cp libmalloc.so ../bin


# Prepare subject espresso 
cd $curr_path
echo "Prepare subject: Espresso"
cd espresso-ab-1.0 
./configure
cd src
make
cp espresso ../../bin
cp libmalloc.so ../bin
cp $curr_path/espresso-ab-1.0/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/espresso-ab-1.0/

# Prepare subject cfrac 
cd $curr_path
echo "Prepare subject: Cfrac"
cd cfrac 
make
cp cfrac ../bin
cp $curr_path/cfrac/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/cfrac/

# Prepare subject gawk 
cd $curr_path
echo "Prepare subject: Gawk"
cd gawk-4.0.2
./configure
cd src
make
cp gawk ../bin
cp $curr_path/gawk-4.0.2/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/gawk-4.0.2/

# Prepare subject space 
cd $curr_path
echo "Prepare subject: space"
cd space 
make
cp space ../bin
cp $curr_path/space/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/space/

# Prepare subject flex 
cd $curr_path
echo "Prepare subject: flex"
cd flex/src 
make
cp flex ../../bin
cp $curr_path/flex/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/flex/

# Prepare subject bash
cd $curr_path
echo "Prepare subject: bash"
cd bash/src 
./configure
make
cp bash ../../bin
cp $curr_path/bash/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/bash/

# Prepare subject sed
cd $curr_path
echo "Prepare subject: sed"
cd sed/src 
make
cp sed ../../bin
cp $curr_path/sed/subjectSetting.h $curr_path/../memory/src/
cd $curr_path/../memory/src/
make
cp ../bin/memory $curr_path/sed/

# Prepare subject hashmap 
#cd $curr_path
#echo "Prepare subject: hashmap"
#cd c_hashmap 
#make
#cp hashmap ../bin



