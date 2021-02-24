# tPNG
# Johnathan Corkery, 2021
#
# Simple makefile for making the test driver and example
# You don't need to `make` if you are just using the project,
# just add the tpng.c and tpng.h file to your project add compile it.

all:
	$(CC) tpng.c -Wall -O2 tests/driver.c -o ./tests/tpng_test
	$(CC) tpng.c -Wall -O2 example/helper.c example/main.c -o ./example/example

debug:
	$(CC) tpng.c -Wall -fsanitize=address -fsanitize=undefined -g tests/driver.c  -o ./tests/tpng_test
	$(CC) tpng.c -Wall -fsanitize=address -fsanitize=undefined -g example/helper.c example/main.c -o ./example/example

clean:
	rm ./tests/tpng_test
