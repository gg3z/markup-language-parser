#!/bin/bash
cd ./test
printf "input file:\n"
INPUT_FILE=$1
cat $INPUT_FILE
printf "\nplain text in BFS:\n"
../build/RelWithDebInfo/mlparser $INPUT_FILE