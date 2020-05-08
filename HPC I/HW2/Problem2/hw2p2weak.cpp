#include <iostream>
#include <complex>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#include "../timer.h"
using namespace std;

static const float limit = 2.0;
static const int maxIterations = 10000;
static const int Nx = 4096;
static const int Ny = 4096;

bool containedInMandelbrot(complex< double > C, float bound, int maxIter){
	complex<double> z = 0;
	for (int i = 0; i < maxIter; i++) {
		if ( abs(z) > bound ) {
			return false;
		}
		z = (z*z) + C;
	}
	return true;
}

int main() 
{
    const double hx = 2.0 * limit / (double) Nx;
    const double hy = 2.0 * limit / (double) Ny;
    complex< double > C, D;
//    int area = 0;
    timespec before, after;
    timespec time_diff;
    double time_s, time_p;
	time_p = 0;
    for(int p = 1; p < omp_get_max_threads(); p++)
{
	omp_set_num_threads(p);
while(time_p < 100)
{
    /* Serial 
    get_time(&before);
    for (int n=0; n<Nx*Ny; n++)
	{
        int i = n%Nx;
        int j = n/Nx;
        double x = hx * ((double) i - (((double) Nx - 1.0)/2.0));
        double y = hy * ((double) j - (((double) Ny - 1.0)/2.0));
        C = complex< double >(x, y);
		if( containedInMandelbrot( C, limit, maxIterations) )
			++area;
    }
    get_time(&after);
    diff(&before, &after, &time_diff);
    time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
    cout << " \n Area of Mandelbrot set (Serial): " << (double) area * hx * hy << "\n\n";
    cout << " Series calculation time: " << time_s << "s\n";
*/
    /* Parallel */
    int area_p = 0;
    get_time(&before);
    #pragma omp parallel for reduction(+:area_p)
    for (int n=0; n<Nx*Ny; n++) {
        int i = n%Nx;
        int j = n/Nx;
        double x = hx * ((double) i - (((double) Nx - 1.0)/2.0));
        double y = hy * ((double) j - (((double) Ny - 1.0)/2.0));
        D = complex< double >(x, y);
        if( containedInMandelbrot( D, limit, maxIterations) )
			++area_p;
    }
    get_time(&after);
    diff(&before, &after, &time_diff);
    time_p = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
	if( p == 1)
		time_s = time_p;
}
    cout << " \n Area of Mandelbrot set (Parallel): " << (double) area_p * hx * hy << "\n\n";
    cout << " Parallel calculation time: " << time_p << "s\n\n";

    /* Calculate serial fraction */
    int processors = omp_get_num_threads();
    double speedRatio = time_s / time_p;
    double inverseS = 1.0/speedRatio;
    double inverseP = 1.0/processors;
    double f = (inverseS - inverseP)/(1.0 - inverseP);
    
    
    cout << " Num Processors = " << processors << "\n";
    cout << " Speed up = " << speedRatio << " *100 percent \n";
    cout << " Serial Fraction: f = " << f << "\n\n";    
}
return 0;
}
