CC = g++
CFLAGS = -g -Wall

hw1p2: ./Problem2/hw1p2.cpp
	$(CC) $(CFLAGS) -o $@_O0 ./Problem2/hw1p2.cpp timer.c
	$(CC) $(CFLAGS) -O1 -o $@_O1 ./Problem2/hw1p2.cpp timer.c
	$(CC) $(CFLAGS) -O2 -o $@_O2 ./Problem2/hw1p2.cpp timer.c
	$(CC) $(CFLAGS) -O3 -o $@_O3 ./Problem2/hw1p2.cpp timer.c
	
hw1p2intel: ./Problem2/hw1p2intel.cpp
	icpc -o $@_O0 ./Problem2/hw1p2intel.cpp timer.c
	icpc -O1 -o $@_O1 ./Problem2/hw1p2intel.cpp timer.c
	icpc -O2 -o $@_O2 ./Problem2/hw1p2intel.cpp timer.c
	icpc -O3 -o $@_O3 ./Problem2/hw1p2intel.cpp timer.c

hw1p3: ./Problem3/hw1p3.cpp
	$(CC) $(CFLAGS) -o $@ ./Problem3/hw1p3.cpp timer.c
	
hw1p3mkl: ./Problem3/hw1p3mkl.cpp
	icpc -I${MKLROOT}/include -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -O3 -o $@ ./Problem3/hw1p3mkl.cpp timer.c
	
hw1p4: ./Problem4/hw1p4.cpp
	$(CC) $(CFLAGS) -o $@ ./Problem4/hw1p4.cpp timer.c
	
hw1p4mkl: ./Problem4/hw1p4mkl.cpp
	icpc -I${MKLROOT}/include -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_sequential -lpthread -lm -O3 -o $@ ./Problem4/hw1p4mkl.cpp timer.c