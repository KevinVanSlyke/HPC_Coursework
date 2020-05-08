#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <math.h>
// include "mkl.h"
#include "/lib64/cblas.h"
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../timer.h"
 
 
using namespace std;
     
int main() 
{
    for(int stride=1; stride<=1; stride++)
	{
		timespec before, after;
        timespec time_diff;
        // Open file to write to
        FILE * file;
        ostringstream fileNameStream;
        fileNameStream << "Problem3/blas_output_stride_" << stride << ".txt";
        string fileName = fileNameStream.str(); 
        file = fopen(fileName.c_str(), "w");
        fprintf(file,"# Size \t MFLOP/S \n");
 
        for (int mySize=1; mySize<=pow(2,26); mySize=mySize*2) 
		{

             
            srand(time(NULL));
             
            // Initialize vectors
            double myA[mySize];
            double myB[mySize];
            // Fill vector with random numbers
            for (int i=0; i<mySize; i++) 
			{
                A[i] = (double) (rand()%10);
                B[i] = (double) (rand()%10);
            }
            double sum;
            // Dot product
            get_time(&before);
            sum = ddot(mySize,myA,1, myB,1);
            get_time(&after);
               
            diff(&before,&after,&time_diff);
            double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
              
			double perf = (2.0 * (double) mySize / time_s) / 1.0e6;
             
            // Write to file
            fprintf(file, "%d \t %f \t \n", mySize*8, perf);
        }
                 
        fclose(file);
    }
    return 0;
}