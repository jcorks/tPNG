#!/bin/bash

# a build file for delivering coverage results for the test case.
# Not really useful to anyone using tPNG!

make
cd ./tests/
./tpng_test
cd ..
gcov tpng.c
