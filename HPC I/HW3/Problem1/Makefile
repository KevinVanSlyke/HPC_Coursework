# the compiler: gcc for C program, define as g++ for C++
CC = mpiicpc

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  #  -pg uses gprof, makes output file which is run in gprof and piped to new output
CFLAGS = -g -Wall

hw3p1: hw3p1.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c

hw3p1weak: hw3p1weak.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c