#pragma once
#include <stdio.h>
#include<time.h>
#include "Wolf.h"
#include "prey.h"
#include <sstream>




class Pool
{
public:

	struct Brain
	{
		float v_matrix[10][10];
		float w_matrix[1000][9][10][10];
		float mutants[1000][9][10][10];
		int survivors[500];
		int champion;
		float results[2000];
	};
	Brain *wolfBrain;

	Brain *preyBrain;

	
	
	int poolSize = 500;
	int Width = 700;
	bool train = false;
	int Height = 500;
	bool buisy;
	Pool* copy;
	std::stringstream stream;

	

	
	Pool();
	//~Pool();

	//void update();
	int runNet(Prey* prey, Wolf *wolf1, Wolf *wolf2, int mem, Brain* brain, bool* controls);
	
	int eval(void); // pessed array of results
	int pEval(void);
	
	bool mutate(Brain* brain, int x); //and respawn. // pass the best from 
	
	std::stringstream stat();// also print best and average. and range.

	float randVal(float x);//returns random float between .0 and .2
	
	int calcVal(int L, int before, int layr, int mem, Brain *brain);

	float sigmoid(float input);

};






