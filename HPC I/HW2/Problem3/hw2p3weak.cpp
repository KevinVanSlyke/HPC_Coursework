#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#include "../timer.h"
using namespace std;

bool prime(int N)
{
	for (int i = 2; i <= sqrt(N); i++)
		{
			if (N % i == 0) {
				return false;
		}
	}
	return true;
}

bool checkGoldbach(int N)
{
	for (int i = N/2; i > 1; i--)
	{
		if (prime(i))
		{
			if (prime(N-i))
			{
				return true;
			}
		}
	}
	return false;
}

int main()
{
    	bool verified = true;
	long long int N = 1e7;
	timespec before, after, time_diff;
	double time_p_out_bool;
	int largest = 0;
	/* Parallel outer loop boolean (threads get different numbers N) */
	time_p_out_bool = 0.0;
	get_time(&before);
while(time_p_out_bool < 60.0){
    	#pragma omp parallel for schedule(dynamic) shared(verified) shared(time_p_out_bool) num_threads(3)
	for (int j=4; j<N; j+=2) 
	{
		if (j%2 != 0 || !verified) 
		{
			continue;
		}
 		if (!checkGoldbach(j)) 
		{
 			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
 			verified = false;
 		}
		get_time(&after);
		diff(&before, &after, &time_diff);
		time_p_out_bool = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	largest = j;	
	}
	}
    
	cout << " Time for Parallel Bool (Outer): " << time_p_out_bool << " s\n";	
	cout << " Largest num checked: " << largest << "\n";		
 
return 0;	
}
