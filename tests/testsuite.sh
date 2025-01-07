#!/bin/bash

echo "Run test..."
./scriptTest.sh
gcc -o unittests unittests.c && ./unittests
echo "Tests complete."

