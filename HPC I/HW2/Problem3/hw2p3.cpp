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

int countGoldbach(int N)
{
	int pairs = 0;
	for (int i = N/2; i > 1; i--)
	{
		if (prime(i))
		{
			if (prime(N-i))
			{
				pairs++;
			}
		}
	}
//	cout << N << " " << pairs << " \n";
	return pairs;
}

bool innerParallelCheckGoldbach(int N)
{
	bool verified = false;
	#pragma omp parallel for shared(verified) schedule(static)
	for (int i = N/2; i > 1; i--)
	{
		if (verified)
		{
			continue;
		}
		if (prime(i))
		{
			if (prime(N-i))
			{
				verified = true;
			}
		}
	}
	return verified;
}

int innerParallelCountGoldbach(int N)
{
	int pairs = 0;
	#pragma omp parallel for schedule(static) reduction(+:pairs)
	for (int i = N/2; i > 1; i--)
	{
		if (prime(i))
		{
			if (prime(N-i))
			{
				pairs++;
			}
		}
	}
//	cout << N << " " << pairs << " \n";
	return pairs;
}

int main()
{
    	bool verified = true;
	long long int N = 1e5;
	timespec before, after, time_diff;
	double time_s_bool, time_s_int, time_p_out_bool, time_p_out_int, time_p_in_bool, time_p_in_int;
	
/*	FILE * statFile;
	statFile = fopen("/home/kevin/Documents/Phy 515/HW2/Problem3/static_out.txt", "w");
        FILE * parallelFile;
	parallelFile = fopen("/home/kevin/Documents/Phy 515/HW2/Problem3/parallel_out.txt", "w");
 	FILE * innerFile;
	innerFile = fopen("/home/kevin/Documents/Phy 515/HW2/Problem3/inner_out.txt", "w");
*/
        int pairs;
	/* Serial boolean check*/
	get_time(&before);
	for (int j=4; j<N; j+=2) 
	{
		if (j%2 != 0) 
		{
			continue;
		}
		if (!checkGoldbach(j)) 
		{
			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
			verified = false;
			break;
		} 
	}
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_s_bool = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Time for serial boolean calculation = " << time_s_bool << " \n";

	/* Serial count check */
//	cout << "Serial Count check \n";
//	cout << "Even number" << " " << "prime pairs found" << " \n";
	get_time(&before);
	for (int j=4; j<N; j+=2) 
	{
		pairs = countGoldbach(j);
//		if ( j%1000 == 0)
//{
//			cout << j << " " << countGoldbach(j) << " \n";
//			fprintf(statFile, "%i %i \n", j, pairs);
//}
		if (j%2 != 0) 
		{
			continue;
		}
		if (pairs == 0) 
		{
			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
			verified = false;
			break;
		}
	}
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_s_int = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Time for serial integer calculation = " << time_s_int << " \n";

	/* Parallel outer loop boolean (threads get different numbers N) */
	get_time(&before);
    	#pragma omp parallel for schedule(dynamic) shared(verified)
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
	}
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_p_out_bool = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;	
	cout << "Time for parallel outer boolean calculation = " << time_p_out_bool << " \n";	

	/* Parallel outer loop int (threads get different numbers N) */
//	cout << "Outer Parallel Count check \n";
//	cout << "Even number" << " " << "prime pairs found" << " \n";
	get_time(&before);
   	#pragma omp parallel for schedule(dynamic) shared(verified)
	for (int j=4; j<N; j+=2) 
	{
		pairs = countGoldbach(j);
//		if (j%1000 == 0)
//{
//			cout << j << " " << countGoldbach(j) << " \n";
//			fprintf(parallelFile, "%i %i \n", j, pairs);
//}
		if (j%2 != 0 || !verified) 
		{
			continue;
		}
 		if (pairs == 0) 
		{
 			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
 			verified = false;
 		}
	}
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_p_out_int = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;	
	cout << "Time for parallel outer integer calculation = " << time_p_out_int << " \n";	

 	/* Parallel inner loop boolean (threads work on finding prime pairs for same N) */
 	get_time(&before);
 	for (int j=4; j<N; j+=2) 
	{
 		if (j%2 != 0) 
		{
 			continue;
 		}
 		if (!innerParallelCheckGoldbach(j)) 
		{
 			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
 			verified = false;
 			break;
 		} 
 	}
 	get_time(&after);
 	diff(&before, &after, &time_diff);
 	time_p_in_bool = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;		
	cout << "Time for parallel inner boolean calculation = " << time_p_in_bool << " \n";	

	/* Parallel inner loop int (threads work on finding prime pairs for same N) */
 	get_time(&before);
 	for (int j=4; j<N; j+=2) 
	{

		pairs = innerParallelCountGoldbach(j);
		
//		if ( j%1000 == 0)
//{
//			cout << j << " " << countGoldbach(j) << " \n";
//			fprintf(innerFile, "%i %i \n", j, pairs);
//}
 		if (j%2 != 0) 
		{
 			continue;
 		}
 		if (pairs == 0) 
		{
 			cout << "Goldbach conjecture is a lie!!\nBroken at N = " << j << "\n";
 			verified = false;
 			break;
 		} 
 	}
 	get_time(&after);
 	diff(&before, &after, &time_diff);
 	time_p_in_int = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;		
	cout << "Time for parallel inner integer calculation = " << time_p_in_int << " \n";	

	if (verified) 
	{
		cout << "Goldbach conjecture verified up to N = " << (int) N << "\n";
	}
	
	int processors = omp_get_max_threads();
	
    double speedRatio_outer_bool = time_s_bool / time_p_out_bool;
    double inverseS_outer_bool = 1.0/speedRatio_outer_bool;
    double inverseP_outer_bool = 1.0/processors;
    double f_out_bool = (inverseS_outer_bool - inverseP_outer_bool)/(1.0 - inverseP_outer_bool);
	
	double speedRatio_inner_bool = time_s_bool / time_p_in_bool;
    double inverseS_inner_bool = 1.0/speedRatio_inner_bool;
    double inverseP_inner_bool = 1.0/processors;
    double f_in_bool = (inverseS_inner_bool - inverseP_inner_bool)/(1.0 - inverseP_inner_bool);
	
	double speedRatio_outer_int = time_s_int / time_p_out_int;
    double inverseS_outer_int = 1.0/speedRatio_outer_int;
    double inverseP_outer_int = 1.0/processors;
    double f_out_int = (inverseS_outer_int - inverseP_outer_int)/(1.0 - inverseP_outer_int);
	
	double speedRatio_inner_int = time_s_int / time_p_in_int;
    double inverseS_inner_int = 1.0/speedRatio_inner_int;
    double inverseP_inner_int = 1.0/processors;
    double f_in_int = (inverseS_inner_int - inverseP_inner_int)/(1.0 - inverseP_inner_int);

	cout << " Time for Serial Bool: " << time_s_bool << " s\n";             
	cout << " Time for Parallel Bool (Outer): " << time_p_out_bool << " s\n";	
	cout << " Serial Fraction for Bool (Outer): " << f_out_bool << "\n";	
 	cout << " Time for Parallel Bool (Inner): " << time_p_out_bool << " s\n";
	cout << " Serial Fraction for Bool (Inner): " << f_in_bool << "\n";	
	
	cout << " Time for Serial Int: " << time_s_int << " s\n";             
	cout << " Time for Parallel Int (Outer): " << time_p_out_int << " s\n";	
	cout << " Serial Fraction for Int (Outer): " << f_out_int << "\n";	
 	cout << " Time for Parallel Int (Inner): " << time_p_in_int << " s\n";	
	cout << " Serial Fraction for Int (Inner): " << f_in_int << "\n";
return 0;	
}
