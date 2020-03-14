#!/bin/bash

set -eu
HERE=$(cd $(dirname $0); pwd)
cd $HERE

TARGET=test

function makegcov(){
  local SRC=$1
  gcov -p -l -f ./${SRC} \
    -gcno=./CMakeFiles/test.dir/${SRC}.gcno \
    -gcda=./CMakeFiles/test.dir/${SRC}.gcda | c++filt -n
}

for x in "$@"
do
  makegcov "$x"
done
