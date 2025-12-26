#!/bin/bash
cd ./test
echo "$(pwd)"
EXECUTABLE="../build/RelWithDebInfo/mlparser"

$EXECUTABLE "./invalid_9.txt"
