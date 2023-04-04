#include <iostream>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mpi.h>

#include "../timer.h"
using namespace std;

static const float limit = 2.0;
static const int maxIterations = 10000;


bool containedInMandelbrot(complex< double > C, float bound, int maxIter){
	complex<double> z = 0;
	for (int i = 0; i < maxIter; i++) {
		if ( abs(z) > bound ) {
			return false;
		}
		z = (z*z) + C;
	}
	return true;
}

int main( int argc, char *argv[])
{
	int Nx = 256;
	int Ny = 256;
	MPI_Init (&argc, &argv);
  	int id, p;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);  /* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &p);     /* get number of processes */
	Nx = Nx*p;
	const double hx = 2.0 * limit / (double) Nx;
	const double hy = 2.0 * limit / (double) Ny;
	
    	complex< double > C, D;
    
    	timespec before, after;
    	timespec time_diff;
    	double time_p;

	if(id == 0)
	{
		get_time(&before);
		cout << "Num threads = " << p << endl;
		cout << "Width of area element = " << hx << endl;
		cout << "Height of area element = " << hy << endl;
	}
	//Calculate number of even processors
	
	
   	int area_p = 0;
	int area = 0;
	
    for (int n = 0; n < Nx*Ny; n = n + p) 
	{
		for (int k = 0; k < p; k++)
		{
			if(id == k)
			{
				int i = (k+n)%Nx;
				int j = (k+n)/Nx;
				double x = hx * ((double) i - (((double) Nx - 1.0)/2.0));
				double y = hy * ((double) j - (((double) Ny - 1.0)/2.0));
				D = complex< double >(x, y);
				if( containedInMandelbrot( D, limit, maxIterations) )
					++area_p;
			}
		}
    }	
	cout << "Partial area found by process " << id << " is " << (double)area_p*hx*hy << endl;

	
	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Reduce(&area_p, &area,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	
	MPI_Finalize();

	if(id == 0)
	{
			cout << "Total area found by all processes is " << (double)area*hx*hy << endl;
    		get_time(&after);
    		diff(&before, &after, &time_diff);
    		time_p = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    		cout << "Parallel calculation time: " << time_p << "s" << endl;
   	}
	
	return 0;
}
