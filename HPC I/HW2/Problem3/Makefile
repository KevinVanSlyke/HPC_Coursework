# the compiler: gcc for C program, define as g++ for C++
CC = g++

  # compiler flags:
  #  -g    adds debugging information to the executable file
  #  -Wall turns on most, but not all, compiler warnings
  #  -pg uses gprof, makes output file which is run in gprof and piped to new output
CFLAGS = -g -Wall -std=c++11 -O1 -fopenmp

  # the build target executable:
TARGET = hw2p3

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp ../timer.c

clean:
	$(RM) $(TARGET)
