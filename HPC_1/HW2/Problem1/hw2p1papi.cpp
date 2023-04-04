#include <iostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../timer.h"

#include <stdexcept>
#include "../papi.h"

using namespace std;

void handle_error (int retval)
{
     printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
     exit(1);
}

vector< vector<double> > addMatricesRowFirst( vector< vector<double> > A, vector< vector<double> > B) 
{
	vector< vector<double> > C( A.size(), vector<double> (A.size(), 0) );
    for( unsigned int i = 0; i < A.size(); i++ ) {
        for( unsigned int j = 0; j < A.size(); j++ ) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
	return C;
}

vector< vector<double> > addMatricesColumnFirst( vector< vector<double> > A, vector< vector<double> > B)
{
		vector< vector<double> > C( A.size(), vector<double> (A.size(), 0) );
    for( unsigned int j = 0; j < A.size(); j++ ) {
        for( unsigned int i = 0; i < A.size(); i++ ) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
	return C;
}

void printMatrix( vector< vector<double> > M)
{
    for( unsigned int i = 0; i < M.size(); i++ ) {
        for( unsigned int j = 0; j < M.size(); j++ ) {
            cout << M[i][j] << "\t";
        }
    cout << endl;
    }
}
#define NUM_EVENTS 1
int main()
{
    int N = 4096;
	int	retval;
	
//	int EventSet = PAPI_NULL;
//Couldn't get the EventSet way working, had to use the line below, changing L1 -> L2 -> L3 for each addition method
	int Events[NUM_EVENTS] = {PAPI_L3_TCM};
	

	long_long values[NUM_EVENTS];

//	float rtime, ptime, mflops;
//	long_long flpins;
    srand(time(NULL));
		
    vector< vector<double> > A( N, vector<double> (N, 0) );
    vector< vector<double> > B( N, vector<double> (N, 0) );
    vector< vector<double> > C( N, vector<double> (N, 0) );

    timespec before, after;
    timespec time_diff;
    
	
	
    // Fill matrices
    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            double randNum = rand() % 10;
            A[i][j] = randNum;
		randNum = rand() % 10;
            B[i][j] = randNum;
        }
    }
	
/*	if (PAPI_create_eventset(&EventSet) != PAPI_OK)
		handle_error(1);
	// Add Total Instructions Executed to our EventSet 
	if (PAPI_add_event(EventSet, PAPI_FP_OPS) != PAPI_OK)
		handle_error(1);
	if (PAPI_add_event(EventSet, PAPI_L1_TCM) != PAPI_OK)
		handle_error(1);
	if (PAPI_add_event(EventSet, PAPI_L2_TCM) != PAPI_OK)
		handle_error(1);
	if (PAPI_add_event(EventSet, PAPI_L3_TCM) != PAPI_OK)
		handle_error(1);
*/	
    if((retval=PAPI_start_counters( Events, NUM_EVENTS))<PAPI_OK)
		handle_error(retval);

	// Setup PAPI library and begin collecting data from the counters 
//	if((retval=PAPI_flops( &rtime, &ptime, &flpins, &mflops))<PAPI_OK)
//		handle_error(retval);

    get_time(&before);
    C = addMatricesColumnFirst(A, B);
    get_time(&after);
    diff(&before,&after,&time_diff);
    double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout << "     Operating time (Row): " << time_s << " seconds.\n";
	
	if((retval=PAPI_stop_counters(values, NUM_EVENTS))<PAPI_OK)
		handle_error(retval);
	
	/* Collect the data into the variables passed in */
//	if((retval=PAPI_flops(  &rtime, &ptime, &flpins, &mflops))<PAPI_OK)
//		handle_error(retval);
//	cout << mflops;


//	if ((retval = PAPI_read(EventSet, values)) != PAPI_OK)
//		handle_error(retval);
	
	cout << values[0];
	
	return 0;
}
