#!/bin/bash


# Prepare subject diff
echo "Prepare subject: Diff 3.2"
mkdir bin
cd diffutils-3.2 
./configure
make
cp ./src/diff ../bin

