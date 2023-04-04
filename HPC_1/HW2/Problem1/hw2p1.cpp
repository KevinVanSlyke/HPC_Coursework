#include <iostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../timer.h"

using namespace std;

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

int main()
{
    int N = 4096;
//    cout << " Enter size of NxN matrix: N = ";
//    cin >> N;
//    cout << endl;  
    srand(time(NULL));
    
    vector< vector<double> > A( N, vector<double> (N, 0) );
    vector< vector<double> > B( N, vector<double> (N, 0) );
    vector< vector<double> > C( N, vector<double> (N, 0) );
    vector< vector<double> > D( N, vector<double> (N, 0) );

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
    
    get_time(&before);
    C = addMatricesRowFirst(A, B);
    get_time(&after);
    diff(&before,&after,&time_diff);
    double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout << "     Operating time (Row): " << time_s << " seconds.\n";
    
    get_time(&before);
    D = addMatricesColumnFirst(A, B);
    get_time(&after);
    diff(&before,&after,&time_diff);
    time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout << "  Operating time (Column): " << time_s << " seconds.\n";   
	return 0;
}
