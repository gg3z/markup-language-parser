#!/bin/bash
cd ./test
echo "Running test cases in $(pwd)"
EXECUTABLE="../build/RelWithDebInfo/mlparser"

rm ./result/*.out

$EXECUTABLE "./valid_1.txt"
$EXECUTABLE "./valid_2.txt"
$EXECUTABLE "./valid_3.txt"
$EXECUTABLE "./valid_4.txt"
$EXECUTABLE "./valid_5.txt"
$EXECUTABLE "./valid_6.txt"
$EXECUTABLE "./valid_7.txt"
$EXECUTABLE "./valid_8.txt"
$EXECUTABLE "./valid_9.txt"
$EXECUTABLE "./invalid_1.txt"
$EXECUTABLE "./invalid_2.txt"
$EXECUTABLE "./invalid_3.txt"
$EXECUTABLE "./invalid_4.txt"
$EXECUTABLE "./invalid_5.txt"
$EXECUTABLE "./invalid_6.txt"
$EXECUTABLE "./invalid_7.txt"
$EXECUTABLE "./invalid_8.txt"
$EXECUTABLE "./invalid_9.txt"
mv *.out ./result