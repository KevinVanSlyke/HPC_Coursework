# the compiler: gcc for C program, define as g++ for C++
CC = mpiicpc

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  #  -pg uses gprof, makes output file which is run in gprof and piped to new output
CFLAGS = -g -Wall

  # the build target executable:
all : hw3p2 hw3p2a hw3p2weak hw3p2aweak 

hw3p2: hw3p2.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c

hw3p2weak: hw3p2weak.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c

hw3p2a: hw3p2a.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c
	
hw3p2aweak: hw3p2aweak.cpp
	$(CC) $(CFLAGS) -o $@ $@.cpp ../timer.c