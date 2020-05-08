# the compiler: gcc for C program, define as g++ for C++
CC = g++

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -O2
  # the build target executable:

hw1p4: hw1p4.cpp timer.c
	$(CC) $(CFLAGS) -o $@ hw1p4.cpp timer.c

