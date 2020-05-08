#include <cstdlib> 
#include <stdio.h> 
#include <iostream>
#include <vector>
#include "timer.h"
#include <time.h>
using namespace std;
//using namespace std::chrono;


double randMToN(double M, double N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}

int mySize, dataSize, stride;
double myC, flops;
int d, c, i, j;

int main()
{
//	FILE * pFile;
	FILE * pFile1;
//	pFile = fopen("/home/kevin/Documents/Phy 515/HW1/Problem3/output.txt", "w");
	pFile1 = fopen("excel_output.txt", "w");
/*
	pFile = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P3\\HW1P3\\output.txt", "w");
	pFile1 = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P3\\HW1P3\\excel_output.txt", "w");
*/
	timespec before, after;
	timespec time_diff;
	for ( mySize = 2; mySize < 30000000; mySize = mySize * 2)
	{
		/* initialize random seed: */
		srand(time(NULL));
	 
		vector<double> myA(mySize);
		vector<double> myB(mySize);
		
//		fprintf(pFile, "Vector length %d \n", mySize);

		for (d = 0; d < mySize; ++d)
		{
			myA[d] = randMToN(-10, 10);
			myB[d] = randMToN(-10, 10);
		}

		for (stride = 1; stride < 9; stride++)
		{
			myC = 0;
			get_time(&before);
//			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			for (j = 0; j < stride; ++j)
				for (i = j; i < mySize; i += stride)
					myC += myA[i] * myB[i];

/*			for (j = 0; j < mySize; j += stride)
				for (i = j; i < (j + stride); i = ++i)
					myC += myA[i] * myB[i];
*/
			get_time(&after);
			diff(&before, &after, &time_diff);
			double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
//			high_resolution_clock::time_point t2 = high_resolution_clock::now();
//			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//			fprintf(pFile, "Elapsed time for vector dot product with stride %d is %I64d microseconds \n", stride, duration);
			dataSize = double(mySize * 8);
			flops = 2 * mySize / time_s;
			fprintf(pFile1,"%d, %d, %f, %f \n", stride, dataSize, flops, time_s);
		}
	}
//	fclose(pFile);
	fclose(pFile1);

	return 0;
}
