#include <iostream>
#include <vector>
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../timer.h"
#include "mkl.h"
 
using namespace std;
 
int main() {
     
    int N = 2000;
//    cout << " Enter size of NxN matrix: N = ";
//    cin >> N;
//    cout << endl;
	FILE * pFile1;
//	FILE * pFile2;
//	pFile = fopen("output.txt", "w");
	pFile1 = fopen("p4mkl_plot.txt", "w");
    srand(time(NULL));
    double *A, *B, *C;
    double alpha = 1.0;
    double beta = 0.0;
     
    timespec before, after;
    timespec time_diff;
     
    A = (double *)mkl_malloc( N*N*sizeof( double ), 64 );
    B = (double *)mkl_malloc( N*N*sizeof( double ), 64 );
    C = (double *)mkl_malloc( N*N*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
        cout << "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n";
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return 1;
    }
    // Fill matrices
    for (int i=0; i<N*N; i++) {
        A[i] = rand()%10;
        B[i] = rand()%10;
    }
       
       
    get_time(&before);
     
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, alpha, A, N, B, N, beta, C, N);
     
    get_time(&after);
 
    diff(&before,&after,&time_diff);
    double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	double flops = N * N * N * 2;
	fprintf(pFile1, "%f, %f \n", flops, time_s);
	fclose(pFile1);
	
    return 0;
}