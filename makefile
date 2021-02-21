# tPNG
# Johnathan Corkery, 2021
#
# Simple makefile for making the test driver.
# You don't need to `make` if you are just using the project,
# just add the c and h file to your project add compile it.

all:
	$(CC) tpng.c -Wall -O2 tests/driver.c -o ./tests/tpng_test

clean:
	rm ./tests/tpng_test
