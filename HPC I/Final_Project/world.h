//  Adam Sokolow 
//  adam.sokolow@duke.edu 
//  Dust simulation for Dr. Sen 
//  Dated July 29 2005 

/*	Edited by Kevin VanSlyke
kgvansly@buffalo.edu
Dated Jan 2 2016*/

#ifndef WORLD_H 

#include<iostream> 
#include<cstdlib> 
#include<sstream>
#include "dustlist.h" 
#include "dustgrain.h" 

//#include "randomgen.h" 
// ******************************************************************* 
// Create a World Class that keeps track of the visual location of the dust 

// ******************************************************************* 

class world
{
public:

	// constructors/destructor 
	world();                   // default constructor (size==500) 
	world(int x, int y, int xSpeed, int ySpeed);       // initial size of world 
	world(const world & w);         // copy constructor 
	~world();                       // destructor 

  // assignment 
	const world & operator = (const world & gen);

	// accessors 
	int getMaxXSize();
	int getMaxYSize();
	int getCurDust();

	int grainNumAt(int x, int y);

	int cartX_to_MatX(int x, int y);
	int cartY_to_MatY(int x, int y);

	int MatX_to_CartX(int x, int y);
	int MatY_to_CartY(int x, int y);

  // modifiers 
	void updateWorld();  // Update the matrix of the world according to the locations stored in the dust list
	void setWorld(int x, int y, int id);

	void populateWorld(int numDust, int low, int high);
	void populateWorld(int numDust, int low, int high, int filterGap, int filterWidth, int filterLength);
	void populateWorld(int numDust, int low, int high, int filterGap, int filterWidth, int filterLength, int filter2Gap, int filter2Width, int filter2Length);

//	void shakefilter(int filterGap, int filterWidth, int filterLength, float timeF);
	void takeStep();

	void writingDust(); //tracking dust particles
	void overlapingDust();

	dust_list * myList; // from below 

private:
	//random_gen * myGenerator; 
	int myxSpeed, myySpeed;
	int  myXMax, myYMax;                            //World Size 
	int ** myWorld;
};
#define world_H 
#endif 
