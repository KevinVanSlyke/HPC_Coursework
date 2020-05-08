#include <cstdlib> 
#include <stdio.h> 
#include <iostream>
#include <vector>
#include <time.h>
#include "../timer.h"

using namespace std;


/*
double randMToN(double M, double N)
{
	return M + (rand() / (RAND_MAX / (N - M)));
}
*/
timespec before, after;
timespec time_diff;
int mySize = 20000;
int c, d;
vector< vector<double> > myA(mySize, vector<double>(mySize));
vector< vector<double> > myB(mySize, vector<double>(mySize));
vector< vector<double> > myC(mySize, vector<double>(mySize));
vector< vector<double> > myC1(mySize, vector<double>(mySize));

int main()
{
	FILE * pFile;
	pFile = fopen("output.txt", "w");
//	pFile = fopen("C:\\Users\\Kevin\\Documents\\Visual Studio 2015\\Projects\\HW1P2\\HW1P2\\output.txt", "w");

	/* initialize random seed: */
//	srand(time(NULL));
	
	
	for (c = 0; c<mySize; ++c)
	{
		for (d = 0; d < mySize; ++d)
		{
			myA[c][d] = d;
			myB[c][d] = c;
		}

	}
	get_time(&before);
//	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (c = 0; c<mySize; ++c)
	{
		for (d = 0; d<mySize; ++d)
			myC[c][d] = myA[c][d] + myB[c][d];
	}
//	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	get_time(&after);
	diff(&before, &after, &time_diff);		
	double time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	
	//Looping over collumns first instead of rows as above
	get_time(&before);
//	high_resolution_clock::time_point t3 = high_resolution_clock::now();
	for (c = 0; c<mySize; ++c)
	{
		for (d = 0; d<mySize; ++d)
			myC1[d][c] = myA[d][c] + myB[d][c];
	}
//	high_resolution_clock::time_point t4 = high_resolution_clock::now();


//	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//	auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
	get_time(&after);
	diff(&before, &after, &time_diff);		
	double time_s1 = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
//	fprintf(pFile, "Elapsed time for looping over rows then collums is %I64d microseconds. \n", duration);
//	fprintf(pFile, "Elapsed time for looping over collumns then rows is %I64d microseconds. \n", duration1);
	fprintf(pFile, "%f, %If \n", time_s, time_s1);

	fclose(pFile);

	return 0;
}
