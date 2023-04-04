#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
#include "mkl.h"
//#include "cblas.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../timer.h"
 
 
using namespace std;
     
int main() {
    for(int stride=1; stride<=1; stride++) {
        // Open file to write to
        FILE * file;
       file = fopen("p3mkl_plot.txt", "w");
        fprintf(file,"# Size\tFlops/s\tError\n");
 
        for (int N=1; N<=pow(2,26); N=N*2) {
             
            // Initialize stat variables //
            double total_time = 0.0;
            double time_squared = 0.0;
            const int runs = 10;
            timespec before, after;
            timespec time_diff;
             
            srand(time(NULL));
             
            // Initialize vectors
            double *A;
            double *B;
            A = (double *)mkl_malloc( N*sizeof( double ), 64 );
            B = (double *)mkl_malloc( N*sizeof( double ), 64 );            
            const int inc = 1;
             
            if (A == NULL || B == NULL) {
                cout << "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n";
                mkl_free(A);
                mkl_free(B);
                return 1;
            }
             
            // Repeat multiple times to get average
            for (int j = 0; j<runs; j++) {
                // Fill vector with random numbers
                for (int i=0; i<N; i++) {
                    A[i] = (double) (rand()%10);
                    B[i] = (double) (rand()%10);
                }
                double sum;
                // Dot product
                get_time(&before);
                sum = cblas_ddot(N,A,inc, B,inc);
                get_time(&after);
                 
                diff(&before,&after,&time_diff);
                double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
                 
                //cout << " Operating time: " << time_s << " seconds.\n";
                cout << sum/(double) N << endl;
                total_time += time_s;
                time_squared += pow(time_s,2);
            }
             
            // Stats
            total_time = total_time / (double) runs;
            time_squared = time_squared / (double) runs;
            double time_error = sqrt(time_squared - pow(total_time, 2));
            double perf = (2.0 * (double) N / total_time) / 1.0e6;
            double perf_err = (perf * time_error / total_time) / 1.0e6;
             
            // Write to file
            fprintf(file, "%d\t%f\t%f\n", N*8, perf, perf_err);
            //file << N*8 << "\t" << perf << "\t" << perf_err << endl;
        }
                 
        fclose(file);
    }
    return 0;
}