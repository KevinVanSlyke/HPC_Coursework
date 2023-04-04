#include <mpi.h>

#include <cmath> 
#include <iostream> 
#include <fstream> 
#include <cstdlib> 
#include <string> 
#include <vector>
#include <math.h> 
#include <errno.h>
#include <time.h>
#include "timer.h"
 
int main(int argc, char *argv[])
{
	MPI_Init (&argc, &argv);
	srand(time(NULL));
	int npes, iam, ierr;
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &iam);  /* get current process id */
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &npes);     /* get number of processes */
	
	struct timespec before, after;
	struct timespec time_diff;
	double new_time;
	double ping_pong_times[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	double node_pp_times[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int send_data[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int recv_data[8] = { 0, 0, 0, 0, 0, 0, 0, 0 } ;
	for(int m = 0; m < 100; m++)
	{
		for(int i = 1; i < 8; i++)
		{
			if(iam == 0)
				get_time(&before);
			if(iam == 0)
				ierr = MPI_Send(&send_data[0], i, MPI_INT, 1, 100, MPI_COMM_WORLD);
			if(iam == 1)
				ierr = MPI_Recv(&recv_data[0], i, MPI_INT, 0, 100, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Barrier(MPI_COMM_WORLD);
			if(iam == 1)
				ierr = MPI_Send(&send_data[0], i, MPI_INT, 0, 200, MPI_COMM_WORLD);
			if(iam == 0)
				ierr = MPI_Recv(&recv_data[0], i, MPI_INT, 1, 200, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Barrier(MPI_COMM_WORLD);
			if(iam == 0)
			{
				get_time(&after);
				diff(&before, &after, &time_diff);
				new_time = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
				//std::cout << "Latency between node 0 and " << 1 << " is " << new_time << " seconds for n = " << i << "." <<std::endl;
				ping_pong_times[i] = ping_pong_times[i] + new_time;
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		for(int i = 1; i < 8; i++)
		{
			if(iam == 0)
				get_time(&before);
			if(iam == 0)
				ierr = MPI_Send(&send_data[0], i, MPI_INT, 3, 300, MPI_COMM_WORLD);
			if(iam == 3)
				ierr = MPI_Recv(&recv_data[0], i, MPI_INT, 0, 300, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Barrier(MPI_COMM_WORLD);
			if(iam == 3)
				ierr = MPI_Send(&send_data[0], i, MPI_INT, 0, 400, MPI_COMM_WORLD);
			if(iam == 0)
				ierr = MPI_Recv(&recv_data[0], i, MPI_INT, 3, 400, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Barrier(MPI_COMM_WORLD);
			if(iam == 0)
			{
				get_time(&after);
				diff(&before, &after, &time_diff);
				new_time = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
				//std::cout << "Latency between node 0 and " << 3 << " is " << new_time << " seconds for n = " << i << "." << std::endl;
				node_pp_times[i] = node_pp_times[i] + new_time;
			}
			MPI_Barrier(MPI_COMM_WORLD);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 0)
	{	
		for(int i = 0; i < 8; i++)
		{
			node_pp_times[i] = node_pp_times[i]/100;
			ping_pong_times[i] = ping_pong_times[i]/100;
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	double orig_data[4] = {(iam*4)+1, (iam*4)+2, (iam*4)+3, (iam*4)+4};
	double new_data[4] = {0, 0, 0, 0};
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 0)
		std::cout << "Original Row 0: " << orig_data[0] << " " << orig_data[1] << " " << orig_data[2] << " " << orig_data[3] << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 1)
		std::cout << "Original Row 1: " << orig_data[0] << " " << orig_data[1] << " " << orig_data[2] << " " << orig_data[3] << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
		
	double time_total;
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 0)
		get_time(&before);
	/*MPI::Comm::Alltoall(const void* sendbuf, int sendcount, const MPI::Datatype& sendtype, void* recvbuf,
    int recvcount, const MPI::Datatype& recvtype)*/
	ierr = MPI_Alltoall(&orig_data[0], 4, MPI_DOUBLE, &new_data[0], 4, MPI_DOUBLE, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 0)
	{
		get_time(&after);
		diff(&before, &after, &time_diff);
		time_total = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;

		std::cout << "New Row 0: " << new_data[0] << " " << new_data[1] << " " << new_data[2] << " " << new_data[3] << std::endl;
		FILE * pFile= fopen("output.txt","a");
		std::cout << std::endl;
		std::cout << "Total runtime is: " << time_total << " seconds for n = 4 matrix transpose." <<std::endl;
		fprintf(pFile, "Ping pong times are: \n");
		for(int i = 0; i < 8;i++)
		{
			fprintf(pFile, "%f %f \n", ping_pong_times[i], node_pp_times[i]);
		}
		fclose(pFile);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(iam == 1)
		std::cout << "New Row 1: " << new_data[0] << " " << new_data[1] << " " << new_data[2] << " " << new_data[3] << std::endl;
	
	return 0;
}