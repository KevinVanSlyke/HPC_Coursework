#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

#include <mpi.h>

#include "../timer.h"
using namespace std;

double integration( double (*y)(double), double a, int n, double h ) 
{
	//Breaks entire length of integral into discrete sections of dx
	double sum = 0;
	//Loops over number of discrete sections
	for (int i=0; i<n; i++) 
	{
		//Sets current x value to the center of the ith rectangle
		double x = a + ((double) i - 0.5)*h;
		//Adds height of current rectagle to the effective height of
		//a single rectangle of width h
		sum += (*y)(x);
	}
	return sum * h;
}

//Calculates y given x where y=sqrt(1-x^2)
double y( double x ) 
{
	return sqrt(1.0 - pow(x, 2));
}

int main( int argc, char *argv[])
{
	/* Initialize variable */
	double a = 0.0;
	double b = 1.0;
	int N = 16000;
	
	/* thread id, number of processors/threads, number of intervals for each thread to integrate, mpi error flag*/
	int myid, p, num;
	/* starting point for each threads integration routine, and the range over which it will integrate */
	double a_thread, range_thread, piParallel;

	MPI_Init(&argc,&argv);                 /* starts MPI */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);  /* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &p);     /* get number of processes */

	N = N*p;
	double h = (double)((b-a)/N);

	timespec before, after;
	timespec time_diff;
	double time_s, time_p;


	/* Serial */
if(myid == 0)
{

	get_time(&before);

	double piSerial = integration(y, a, N, h) * 4.0;

	get_time(&after);
	diff(&before, &after, &time_diff);
	time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Serial Result =" << piSerial << endl;
}
	MPI_Barrier(MPI_COMM_WORLD);
	/* Parallel */
	if(myid == 0)
		get_time(&before);

	num = N/p;
	range_thread = (b-a)/p;
	a_thread = a + myid * range_thread;
	
	double my_piParallel = integration(y, a_thread, num, h) * 4.0;
	cout << "Process " << myid << " has the partial result of " << my_piParallel << endl;
	MPI_Reduce(&my_piParallel, &piParallel,1,MPI_DOUBLE_PRECISION,MPI_SUM,0,MPI_COMM_WORLD);
	MPI_Finalize();
	if(myid == 0)
{
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_p = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Final parallel Result =" << piParallel << " \n";

	/* Calculate serial fraction */

	double speedUp = time_s / time_p;
	double inverseS = 1.0/speedUp;
	double inverseP = 1.0/p;
	double f = (inverseS - inverseP)/(1.0 - inverseP);
	
	cout << "\n";
	cout << "Threads = " << p << "\n";
	cout << "Time for Serial: " << time_s << " s" << endl;             
	cout << "Time for Parallel: " << time_p << " s" << endl; 

	cout << "\n";
	cout << "Speed_up = " << speedUp << "\n";
	cout << "\n";
	cout << "Serial Fraction: f = " << f << "\n";
}
return 0;
}
