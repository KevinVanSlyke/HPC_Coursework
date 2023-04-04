//  Adam Sokolow 
//  adam.sokolow@duke.edu 
//  Dust simulation for Dr. Sen 
//  Dated July 29 2005 

/*	Edited by Kevin VanSlyke
kgvansly@buffalo.edu
Dated Jan 2 2016*/

//#include <mpi.h>

#include <cmath> 
#include <iostream> 
#include <fstream> 
#include <cstdlib> 
#include <string> 
#include <iomanip> 
#include <sstream> 
#include <vector>
#include <math.h> 
#include <errno.h>
#include <time.h>

/*	direct.h for windows filesystem manip
	unistd.h and sys/stat.h for unix filesystem manip*/
//#include <direct.h>
#include <unistd.h>
#include <sys/stat.h>
#include "timer.h"

#include "world.h"
#include "parameterReader.h"



using namespace std;



// ******* 
// M A I N 
// ******* 
int main(int argc, char *argv[])
{
/*	MPI_Init (&argc, &argv);
	int npes, iam, ierr;
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &iam);  // get current process id 
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &npes);     // get number of processes 
	
	std::vector<int> timeCounts;
	for(unsigned int i = 0; i < npes; i++)
	{
		timeCounts.push_back(0);
	}
*/
	int iam, timeCounts;
	struct timespec before, after;
	struct timespec time_diff;
	double time_s;
//	if(iam == 0)
//	{
		get_time(&before);
//	}
	
	world * myWorld;
//	std::vector<world*> myUniverse;
	
	/* Parameters so far:

	World Dimensions: X, Y
	World Speeds: x, y

	Total Dust: n
	Size Range of Dust: 1 -> N

	Total timesteps: t
	Enable: merging, splitting, sticking
	Filter Parameters: Length, Width of Slits, Width of Gaps	*/
	int filter, xMax, yMax, xSpeed, ySpeed, totalGrains, MinGrainSize, MaxGrainSize, MaxTime, sticking, splitting, merging, FilterWidth, FilterGap, FilterLength, Filter2Width, Filter2Gap, Filter2Length;
	bool enableSticking, enableSplitting, enableMerging;
	if (argc < 12)
	{
		std::cout << "Not enough parameters passed to main (Need 11, 14 or 17 integers after the executable name), trying to read from parameters.txt... " << std::endl;
		parameterReader *pR = new parameterReader();
		xMax = pR->getxMax();
		yMax = pR->getyMax();
		xSpeed = pR->getxSpeed();
		ySpeed = pR->getySpeed();
		totalGrains = pR->gettotalGrains();
		MaxTime = pR->getMaxTime();
		enableSticking = pR->getSticking();
		enableSplitting = pR->getSplitting();
		enableMerging = pR->getMerging();
		MinGrainSize = pR->getMinGrainSize();
		MaxGrainSize = pR->getMaxGrainSize();
		FilterWidth = pR->getFilterWidth();
		FilterGap = pR->getFilterGap();
		FilterLength = pR->getFilterLength();
		Filter2Width = pR->getFilter2Width();
		Filter2Gap = pR->getFilter2Gap();
		Filter2Length = pR->getFilter2Length();
	}
	else if(argc == 13)
	{
		xMax = atof(argv[1]);
		yMax = atof(argv[2]);
		xSpeed = atof(argv[3]);
		ySpeed = atof(argv[4]);
		totalGrains = atof(argv[5]);
		MinGrainSize = atof(argv[6]);
		MaxGrainSize = atof(argv[7]);
		MaxTime = atof(argv[8]);
		sticking = atof(argv[9]);
		if (sticking == 1)
			enableSticking = true;
		else
			enableSticking = false;
		splitting = atof(argv[10]);
		if (splitting == 1)
			enableSplitting = true;
		else
			enableSplitting = false;
		merging = atof(argv[11]);
		if (merging == 1)
			enableMerging = true;
		else
			enableMerging = false;
		iam = atof(argv[12]);
                FilterWidth = -1;
                FilterGap = -1;
                FilterLength = -1;
                Filter2Width = -1;
                Filter2Gap = -1;
                Filter2Length = -1;
		filter = 0;
	}
	else if(argc == 16)
	{
		xMax = atof(argv[1]);
		yMax = atof(argv[2]);
		xSpeed = atof(argv[3]);
		ySpeed = atof(argv[4]);
		totalGrains = atof(argv[5]);
		MinGrainSize = atof(argv[6]);
		MaxGrainSize = atof(argv[7]);
		MaxTime = atof(argv[8]);
		sticking = atof(argv[9]);
		if (sticking == 1)
			enableSticking = true;
		else
			enableSticking = false;
		splitting = atof(argv[10]);
		if (splitting == 1)
			enableSplitting = true;
		else
			enableSplitting = false;
		merging = atof(argv[11]);
		if (merging == 1)
			enableMerging = true;
		else
			enableMerging = false;
		FilterWidth = atof(argv[12]);
		FilterGap = atof(argv[13]);
		FilterLength = atof(argv[14]);
		iam = atof(argv[15]);
                Filter2Width = -1;
                Filter2Gap = -1;
                Filter2Length = -1;
		filter = 1;
	}
	else if(argc == 19)
	{
		xMax = atof(argv[1]);
		yMax = atof(argv[2]);
		xSpeed = atof(argv[3]);
		ySpeed = atof(argv[4]);
		totalGrains = atof(argv[5]);
		MinGrainSize = atof(argv[6]);
		MaxGrainSize = atof(argv[7]);
		MaxTime = atof(argv[8]);
		sticking = atof(argv[9]);
		if (sticking == 1)
			enableSticking = true;
		else
			enableSticking = false;
		splitting = atof(argv[10]);
		if (splitting == 1)
			enableSplitting = true;
		else
			enableSplitting = false;
		merging = atof(argv[11]);
		if (merging == 1)
			enableMerging = true;
		else
			enableMerging = false;
		FilterWidth = atof(argv[12]);
		FilterGap = atof(argv[13]);
		FilterLength = atof(argv[14]);
		Filter2Width = atof(argv[15]);
		Filter2Gap = atof(argv[16]);
		Filter2Length = atof(argv[17]);
		iam = atof(argv[18]);
		filter = 2;
	}
//	MPI_Barrier(MPI_COMM_WORLD);
//        if(iam == 0)
//            std::cout << "Creating Universe..." << std::endl;

	//Sets the worlds simulation size and maximum velocities as read in by parameterReader.
	//TODO: Get inputs working for running from a CCR batch file.
/*	for(int i = 0; i < npes; i++)
	{
		myUniverse.push_back(new world(xMax, yMax, xSpeed, ySpeed)); //new world(xMax*(iam+1), yMax, xSpeed, ySpeed));
	}
	myWorld = myUniverse[iam];
*/
	myWorld = new world(xMax, yMax, xSpeed, ySpeed);
	/*Sets the simulation size in the dust_list object for use in dust_list routines*/
	myWorld->myList->setMaxXLoc(xMax);
	myWorld->myList->setMaxYLoc(yMax);

	//TODO: Make arbitrary for any number of filter lines appended to end of parameter text file.
	if (FilterGap == (-1) && Filter2Gap == (-1))
	{
		filter = 0;
		myWorld->populateWorld(totalGrains, MinGrainSize, MaxGrainSize);
		std::cout << "No Filter detected." << std::endl;
	}
	else if (FilterGap != (-1) && Filter2Gap == (-1))
	{
		filter = 1;
		myWorld->populateWorld(totalGrains, MinGrainSize, MaxGrainSize, FilterGap, FilterWidth, FilterLength);
		std::cout << "Filter gap... " << FilterGap << std::endl;
	}
	else if (FilterGap != (-1) && Filter2Gap != (-1))
	{
		filter = 2;
		myWorld->populateWorld(totalGrains, MinGrainSize, MaxGrainSize, FilterGap, FilterWidth, FilterLength, Filter2Gap, Filter2Width, Filter2Length);
		std::cout << "Filter 1 gap... " << FilterGap << endl;
		std::cout << "Filter 2 gap... " << Filter2Gap << endl;
	}

	/*  Routine for creating folder, need to make alternate version for Linux file systems*/
//	if(iam == 0)
//	{
		std::cout << "Total No. of Dust Grains =  " << myWorld->myList->getTotal() - filter << endl;
		std::cout << "X Max = " << myWorld->getMaxXSize() << ". Y Max = " << myWorld->getMaxYSize() << ". " << endl;
//	}
//	MPI_Barrier(MPI_COMM_WORLD);
        //chdir("/gpfs/scratch/kgvansly/");
	chdir("/projects/academic/sen/kgvansly/Dust_Data/");

        std::ostringstream oFolder;
//        int trial = 0;
        oFolder << filter << "filters" << MaxTime << "time" << FilterGap << "pore" << FilterWidth << "fiber" << totalGrains << "ptcls" << MinGrainSize << "to" << MaxGrainSize << "dstr" << xMax << "x" << yMax << "area";
        std::string outputFolder = oFolder.str();
//    	MPI_Barrier(MPI_COMM_WORLD);

	if(mkdir(outputFolder.c_str(), S_IRWXU) == -1)
		std::cout << "Folder oFolder.c_str() already exists, entering." << std::endl;
	else
		mkdir(outputFolder.c_str(), S_IRWXU);
	chdir(outputFolder.c_str());

	
/*        if(iam == 0)
        {
		while (mkdir(outputFolder.c_str(), S_IRWXU) == -1)
		{
			if (trial < 10)
			{
				trial++;
				oFolder.seekp(-1, oFolder.cur);
				oFolder << trial;
				outputFolder = oFolder.str();
			}
			else if (trial < 100)
			{
				trial++;
				oFolder.seekp(-2, oFolder.cur);
				oFolder << trial;
				outputFolder = oFolder.str();
			}
			else if (trial < 1000)
			{
				trial++;
				oFolder.seekp(-3, oFolder.cur);
				oFolder << trial;
				outputFolder = oFolder.str();
			}
		}
	}
       MPI_Barrier(MPI_COMM_WORLD);
*/
        
//        if(iam == 0)
//       {
        if (FILE * parameterFile = fopen("parameters.txt", "w"))
	{
            	std::cout << "parameters.txt already exists, not writting" << std::endl;
		fclose(parameterFile);
	}
	else
	{
	   // OUTPUT: dust, size, y-position, y-step, x-localSp, y-localSp
                fprintf(parameterFile, "%d %d \n%d %d \n%d \n%d %d \n%d \n%d %d %d \n%d %d %d \n%d %d %d", xMax, yMax, xSpeed, ySpeed, totalGrains, MinGrainSize, MaxGrainSize, MaxTime, sticking, splitting, merging, FilterWidth, FilterGap, FilterLength, Filter2Width, Filter2Gap, Filter2Length);
                fclose(parameterFile);
         }
        
//        MPI_Barrier(MPI_COMM_WORLD);

        std::ostringstream pFolder;
        pFolder << "Process" << iam;
        std::string processFolder = pFolder.str();
        mkdir(processFolder.c_str(), S_IRWXU);
        chdir(processFolder.c_str());

//        MPI_Barrier(MPI_COMM_WORLD);
	/* End of folder creation routine */
        
	/* Statistics collection calls, might be better to move them to lower level objects. */
	myWorld->myList->dust_dstr();
	myWorld->myList->setFunctionality(enableSplitting, enableSticking, enableMerging);
	//Debug line
	//std::cout << "Dust distribution has been calculated." << std::endl;

	/* Runs program for determined amount of time. */

/*	while (timeCounts[iam] < runTime)
	{
		MPI_Barrier(MPI_COMM_WORLD);
		if (timeCounts[iam] % 1000 == 0 && iam == 0)
			cout << "Time in worlds  = " << timeCounts[iam] << " " << endl;
		timeCounts[iam] = timeCounts[iam] + 1;
		myWorld->takeStep();
		myWorld->updateWorld();
	}
        MPI_Barrier(MPI_COMM_WORLD);
*/
	while (timeCounts < MaxTime)
	{
		if (timeCounts % 1000 == 0)
			cout << "Time in worlds  = " << timeCounts << " " << endl;
		timeCounts = timeCounts + 1;
		myWorld->takeStep();
		myWorld->updateWorld();
	}
//	if(iam == 0)
//	{	
		get_time(&after);
		diff(&before, &after, &time_diff);
		time_s = time_diff.tv_sec + (double)(time_diff.tv_nsec)/1.0e9;
		std::cout << "Total runtime is: " << time_s << " seconds." << std::endl;
	
	ofstream timeOptimization("timeOptimization.txt", ios::app);
	if (timeOptimization.is_open())
	{
		//writing: time of calculation, number of particles and length/width
		timeOptimization << "Total runtime is: " << time_s << " seconds." << std::endl;
		timeOptimization << "For " << MaxTime << "timesteps." << std::endl;
		timeOptimization << "Simulation of " << totalGrains << " particles in an area of:" << std::endl;
		timeOptimization << xMax << " by " << yMax << ". (x by y)" << std::endl;
		timeOptimization.close();
	}
//	}
//	ierr = MPI_Finalize();
	return 0;
}
