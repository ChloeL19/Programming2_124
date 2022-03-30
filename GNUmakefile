# Makefile for randmst assignment
# inspiration from here: v\https://www.softwaretestinghelp.com/cpp-makefile-tutorial/
# Also from : https://stackoverflow.com/questions/2481269/how-to-make-a-simple-c-makefile

CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
test: g++ unit_tests.cpp unit_tests

# test: unit_test unit_tests.o strassen.o
# 	$(CC) $(CFLAGS) -o unit_tests unit_tests.o strassen.o
 
# unit_tests.o: unit_tests.cpp
# 	$(CC) $(CFLAGS) -c unit_tests.cpp strassen.cpp

# strassen.o: strassen.cpp
# 	$(CC) $(CFLAGS) -c strassen.cpp

# # ****************************************
# # Variables to control Makefile operation
# CC = g++
# CFLAGS = -g -Wall -O0
# CFLAGSFAST = -O2 -Wall

# # ****************************************
# all: strassen

# test: strassen unit_tests

# strassen: strassen.cpp
# 	$(CC) $(CFLAGS) -c strassen.cpp -o strassen

# # unit_tests: unit_tests.cpp
# # 	$(CC) $(CFLAGS) -o unit_tests strassen.cpp unit_tests.cpp 

# unit_tests: unit_tests.cpp
# 	$(CC) $(CFLAGS) unit_tests.cpp -o unit_tests