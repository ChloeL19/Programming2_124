# Makefile for randmst assignment
# inspiration from here: v\https://www.softwaretestinghelp.com/cpp-makefile-tutorial/
# Also from : https://stackoverflow.com/questions/2481269/how-to-make-a-simple-c-makefile

# ****************************************
# Variables to control Makefile operation
CC = g++
CFLAGS = -g -Wall -O0
CFLAGSFAST = -O2 -Wall

# ****************************************
all: strassen unit_tests

test: unit_tests strassen

strassen: strassen.cpp
	$(CC) $(CFLAGS) -c strassen.cpp -o strassen

unit_tests: unit_tests.cpp
	$(CC) $(CFLAGS) -c unit_tests.cpp -o unit_tests