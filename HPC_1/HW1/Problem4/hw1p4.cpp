#include <cstdlib> 
#include <stdio.h> 
#include <iostream>
#include <vector>
#include "timer.h"
#include <time.h>
using namespace std;


double randMToN(double M, double N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}

int i, j, k, l, m, n, c, d, stride, mySize, dataSize;
double flops;

void transposeMatrix(vector < vector <double> > &myMatrix)
{
	int N = myMatrix[0].size();
	for (i = 0; i<N; i++) {
		for (j = i; j<N; j++) {
			double ij = myMatrix[i][j];
			myMatrix[i][j] = myMatrix[j][i];
			myMatrix[j][i] = ij;
		}
	}
}

int main()
{
//	FILE * pFile;
	FILE * pFile1;
//	FILE * pFile2;
//	pFile = fopen("output.txt", "w");
	pFile1 = fopen("hw1p4_plot.txt", "w");
//	pFile2 = fopen("/home/kevin/Documents/Phy 515/HW1/Problem4/excel_trans_output.txt", "w");

/*
	pFile = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P4\\HW1P4\\output.txt", "w");
	pFile1 = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P4\\HW1P4\\excel_output.txt", "w");
	pFile2 = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P4\\HW1P4\\excel_trans_output.txt", "w");
*/
	timespec before, after;
	timespec time_diff;
	for (mySize = 2; mySize < 3000; mySize = mySize * 2)
	{
		/* initialize random seed: */
		srand(time(NULL));
		dataSize = mySize * mySize * 8;

		vector< vector<double> > myA(mySize, vector<double>(mySize));
		vector< vector<double> > myB(mySize, vector<double>(mySize));
		vector< vector<double> > myC(mySize, vector<double>(mySize));
		
		for (c = 0; c < mySize; ++c)
		{
			for (d = 0; d < mySize; ++d)
			{
				myA[c][d] = randMToN(-10,10);
				myB[c][d] = randMToN(-10,10);
			}

		}
/*
		fprintf(pFile, "Timing for Matrix Multiplication of size %d by %d : \n", mySize, mySize);

		for (stride = 1; stride < 9; stride++)
		{
			high_resolution_clock::time_point t1 = high_resolution_clock::now();
			for (i = 0; i < stride; i++)
			{
				for (l = i; l < mySize; l += stride)
				{
					for (j = 0; j < stride; j++)
					{
						for(m = j; m < mySize; m += stride)
						{
							myC[l][m] = 0.0;
							for (k = 0; k < stride; ++k)
							{
								for (n = k; n < mySize; n += stride)
								{
									myC[l][m] += myA[l][n] * myB[n][m];
								}	
							}
						}
					}
				}
			}
		high_resolution_clock::time_point t2 = high_resolution_clock::now();

		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		if (duration < 1)
			duration = 1;
		flops = 2 * mySize * mySize * mySize / duration;
		fprintf(pFile1, "%d, %d, %f \n", stride, dataSize, flops);

		fprintf(pFile, "Elapsed time for matrix mult with 'single' stride %d is %I64d microseconds \n", stride, duration);

		}
*/

		transposeMatrix(myB);

//		fprintf(pFile, "Timing for Transposed Matrix Multiplication of size %d by %d : \n", mySize, mySize);
		for (stride = 1; stride < 9; stride++)
		{
			get_time(&before);
//			high_resolution_clock::time_point t3 = high_resolution_clock::now();
			for (i = 0; i < mySize; i++)
			{
				for (j = 0; j < mySize; j++)
				{
					myC[i][j] = 0.0;
					for (k = 0; k < stride; ++k)
					{
						for (n = k; n < mySize; n += stride)
						{
							myC[i][j] += myA[i][n] * myB[n][j];
						}
					}
				}
			}
			get_time(&after);
			diff(&before, &after, &time_diff);
			double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
/*			high_resolution_clock::time_point t4 = high_resolution_clock::now();

			auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
			if (duration1 < 1)
				duration1 =1;
*/
			flops = 2 * mySize * mySize * mySize / time_s;
			fprintf(pFile1, "%d, %d, %f, %f \n", stride, dataSize, flops, time_s);

//			fprintf(pFile, "Elapsed time for transpose matrix mult with 'single' stride %d is %f microseconds \n", stride, time_s);
		}

	}
//	fclose(pFile);
	fclose(pFile1);
//	fclose(pFile2);

	return 0;
}
