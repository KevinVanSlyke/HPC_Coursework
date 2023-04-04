#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <omp.h>

#include "../timer.h"
using namespace std;

static const long double PI = 4*atan(1);

double serialIntegration( double (*y)(double), double a, double b, int N ) 
{
	//Breaks entire length of integral into discrete sections of dx
	double dx = (b-a)/((double) N);
	double sum = 0;
	//Loops over number of discrete sections
	for (int i=0; i<N; i++) 
	{
		//Sets current x value to the center of the ith rectangle
		double x = a + ((double) i - 0.5)*dx;
		//Adds height of current rectagle to the effective height of
		//a single rectangle of width dx
		sum += (*y)(x);
	}
	return sum * dx;
}

double dynamicParallelIntegration( double (*y)(double), double a, double b, int N ) 
{
	//Breaks entire length of integral into discrete sections of dx
    double dx = (b-a)/((double) N);
	double sum = 0;
	//Loops over number of discrete sections, dividing sections between threads
    #pragma omp parallel for reduction(+:sum) schedule(dynamic) num_threads(3)
    for (int i=0; i<N; i++) 
	{
		//Sets current x value to the center of the ith rectangle
		double x = a + ((double) i - 0.5)*dx;
		//Adds height of current rectagle to the effective height of
		//a single rectangle of width dx
		sum += (*y)(x);
	}
	return sum * dx;
}

double staticParallelIntegration( double (*y)(double), double a, double b, int N ) 
{
	//Breaks entire length of integral into discrete sections of dx
    double dx = (b-a)/((double) N);
	double sum = 0;
	//Loops over number of discrete sections, dividing sections between threads
    #pragma omp parallel for reduction(+:sum) schedule(static) num_threads(3)
    for (int i=0; i<N; i++) 
	{
		//Sets current x value to the center of the ith rectangle
		double x = a + ((double) i - 0.5)*dx;
		//Adds height of current rectagle to the effective height of
		//a single rectangle of width dx
		sum += (*y)(x);
	}
	return sum * dx;
}

//Calculates y given x where y=sqrt(1-x^2)
double y( double x ) 
{
	return sqrt(1.0 - pow(x, 2));
}

int main() 
{
	/* Initialize variable */
	double a = 0.0;
	double b = 1.0;
	int N = 1e9;
	timespec before, after;
	timespec time_diff;
	double time_s, time_p_stat, time_p_dyn;
		
	/* Serial */
	get_time(&before);
	double piSerial = serialIntegration(y, a, b, N) * 4.0;
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Serial Result =" << piSerial << " \n";

	/* Static Parallel */
	get_time(&before);
	double piStatParallel = staticParallelIntegration(y, a, b, N) * 4.0;
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_p_stat = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Static Parallel Result =" << piStatParallel << " \n";

	/* Dynamic Parallel */
	get_time(&before);
	double piDynParallel = dynamicParallelIntegration(y, a, b, N) * 4.0;
	get_time(&after);
	diff(&before, &after, &time_diff);
	time_p_dyn = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	cout << "Dynamic Parallel Result =" << piDynParallel << " \n";

	/* Calculate serial fraction */
	int processors = 3; //omp_get_num_threads();
	
	double speedUpDyn = time_s / time_p_dyn;
	double inverseSDyn = 1.0/speedUpDyn;
	double inversePDyn = 1.0/processors;
	double fDyn = (inverseSDyn - inversePDyn)/(1.0 - inversePDyn);
	
	double speedUpStat = time_s / time_p_stat;
	double inverseSStat = 1.0/speedUpStat;
	double inversePStat = 1.0/processors;
	double fStat = (inverseSStat - inversePStat)/(1.0 - inversePStat);
	
	
	
	
	cout << "\n";
	cout << "Threads = " << processors << "\n";
	cout << "Time for Serial: " << time_s << " s\n";             
	cout << "Time for Parallel with Dynamic Scheduling: " << time_p_dyn << " s\n"; 
	cout << "Time for Parallel with Static Scheduling: " << time_p_stat << " s\n"; 
	cout << "\n";
	cout << "Speed_up from Dynamic Scheduling = " << speedUpDyn << "\n";
	cout << "Speed_up from Static Scheduling = " << speedUpStat << "\n";
	cout << "\n";
	cout << "Serial Fraction (Dynamic): f_d = " << fDyn << "\n";
	cout << "Serial Fraction (Static): f_s = " << fStat << "\n";
return 0;
}
