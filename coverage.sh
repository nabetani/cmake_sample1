#!/bin/bash

set -eu
HERE=$(cd $(dirname $0); pwd)
TARGET=test

echo $HERE
cd $HERE
rm -f CMakeFiles/${TARGET}.dir/**/*.gcda
${TARGET}/sample.out
mkdir -p gcov

gcov -p -l -f ./src/sum_of_digits.cpp \
  -gcno=./CMakeFiles/test.dir/src/sum_of_digits.cpp.gcno \
  -gcda=./CMakeFiles/test.dir/src/sum_of_digits.cpp.gcda

