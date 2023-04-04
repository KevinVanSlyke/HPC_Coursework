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
	int Nx = 64;
	int Ny = 64;
	MPI_Init (&argc, &argv);
	MPI_Status status;
	MPI::Intracomm even_comm_id;
	MPI::Group even_group_id;
	int even_id;
	int even_p;
 	int *even_rank;
 	int i;
  	int id;
  	int j;
  	MPI::Intracomm odd_comm_id;
  	MPI::Group odd_group_id;
  	int odd_id;
  	int odd_p;
  	int *odd_rank;
  	int p;
  	MPI::Group world_group_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &id);  /* get current process id */
	MPI_Comm_size(MPI_COMM_WORLD, &p);     /* get number of processes */
	world_group_id = MPI::COMM_WORLD.Get_group ( );
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
	even_p = ( p + 1 ) / 2;
 	even_rank = new int[even_p];
  	j = 0;
 	for ( i = 0; i < p; i = i + 2 )
 	{
    		even_rank[j] = i;
    		j++;
  	}
  	even_group_id = world_group_id.Incl ( even_p, even_rank );
  	even_comm_id = MPI::COMM_WORLD.Create ( even_group_id );

	//Calc # off proc
  	odd_p = p / 2;
  	odd_rank = new int[odd_p];
  	j = 0;
  	for ( i = 1; i < p; i = i + 2 )
  	{
    		odd_rank[j] = i;
    		j++;
  	}
  	odd_group_id = world_group_id.Incl ( odd_p, odd_rank );
  	odd_comm_id = MPI::COMM_WORLD.Create ( odd_group_id );

	if ( id % 2 == 0 )
  	{
    		even_id = even_comm_id.Get_rank ( );
   		odd_id = -1;
  	}
  	else
  	{
    		odd_id = odd_comm_id.Get_rank ( );
    		even_id = -1;
  	}
   	int area_p = 0;
	int area_even = 0;
	int area_odd = 0;
	int area = 0;

	int points = Nx*Ny/p;
	int p_start = id * points;
	int p_end = p_start + points;
    	for (int a = p_start; a < p_end; a++) 
	{
        	int l = a%Nx;
        	int m = a/Nx;
        	double x = hx * ((double) l - (((double) Nx - 1.0)/2.0));
        	double y = hy * ((double) m - (((double) Ny - 1.0)/2.0));
        	D = complex< double >(x, y);
        	if( containedInMandelbrot( D, limit, maxIterations) )
			++area_p;
    	}	
	cout << "Partial area found by process " << id << " is " << (double)area_p*hx*hy << endl;

	if(odd_id == 0)
	{	
		cout << "Odd rank 0 (sender) world id == " << id << endl;
	}
	if(even_id == 0)
	{
		cout << "Even rank 0 (receiver) world id == " << id << endl;
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if(even_id != -1)
		MPI_Reduce(&area_p, &area_even,1,MPI_INT,MPI_SUM,0,even_comm_id);
	if(odd_id != -1)
		MPI_Reduce(&area_p, &area_odd,1,MPI_INT,MPI_SUM,0,odd_comm_id);

	MPI_Barrier(MPI_COMM_WORLD);
	
	if(odd_id == 0)
	{
		cout << "Odd group rank 0 (sender) world id == " << id << endl;
		cout << "Partial area found by odd processes is " << (double)area_odd*hx*hy << endl;
	}
	else if(even_id == 0)
	{
		cout << "Even group rank 0 (receiver) world id == " << id << endl;
		cout << "Partial area found by even processes is " << (double)area_even*hx*hy << endl;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(p>1){
	if(odd_id == 0)
	{
		cout << "Sender world rank is " << id << endl;
         	MPI_Send(&area_odd, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	else if(even_id == 0)
	{	
		cout << "Receiver world rank is " << id << endl;
		MPI_Recv(&area_odd, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
		cout << "Passed partial area found by odd proc is " << (double)area_odd*hx*hy << endl;
		area = area_even + area_odd;
		cout << "Full Area of Mandelbrot set (Parallel): " << (double) area * hx * hy << endl;
	}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

	if(id == 0)
	{
    		get_time(&after);
    		diff(&before, &after, &time_diff);
    		time_p = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    		cout << "Parallel calculation time: " << time_p << "s" << endl;
   	}
	
	return 0;
}
