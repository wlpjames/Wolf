#pragma once
#include <stdio.h>
#include<time.h>
#include "Wolf.h"
#include "prey.h"
//#include <SFML/Graphics.hpp>
//#include "globals.h"




class Pool
{
public:
	Pool(void);

	
	
	int runNet(float wolfX, float wolfY, float preyX, float preyY, int hitwall, int mem, bool* controls);
	
	std::stringstream stat();// also print best and average. and range. 
	
	int eval(void); // pessed array of results
	
	bool mutate(); //and respawn. // pass the best from 
	
	float randVal(float x);//returns random float between .0 and .2
	
	int calcVal(int L, int before, int layr, int mem);

	int runSilent( Wolf wolf, Prey prey, int windowWidth, int windowHeight);
};






