#pragma once
#include <stdio.h>
#include<time.h>
#include "vals.h"
#include "Wolf.h"
#include "prey.h"
#include <sstream>




class Pool
{
public:

	struct Brain
	{
		float v_matrix[10][10];
		float w_matrix[2000][10][10][10];
		float mutants[2000][10][10][10];
		int survivors[500];
		int champion;
		float results[2000];
		
		
			int poolSize = 500;
			int pick = 125;
			int layers = 10;//including inp and outp
			int LYR = 5 + 1;
			int Mutation = 1;
			int chance = 3; //refenences chances array
			float chances[7] = { 0.5f, 0.1f, 0.05f, 0.01f, 0.005f, 0.001f, 0.0001f };
		
	};

	Brain *wolfBrain;
	Brain *preyBrain;

	struct vals
	{
		int Width = 700;
		bool train = false;
		int Height = 500;
		int trainLen = 1;
		int wolfTrain = 1; //1 == on, 0 == off
		int preyTrain = 1; //1 == on, 0 == off

		int wolfSpeed = 3;
		int preySpeed = 3;
		int prey_active = 1; //1 == on, 0 == off = 1;
		int prey_mobile = 1;
		int prey_constraint = 5;
	};
	
	vals values;
	
	
	

	bool buisy;
	Pool* copy;
	std::stringstream stream;

	

	
	Pool();
	//~Pool();
	void populate(Brain *brain);
	//void update();
	int runNet(Prey* prey, Wolf *wolf1, int mem, Brain* brain, bool* controls);
	
	int eval(void); // pessed array of results
	int pEval(void);
	
	bool mutate(Brain* brain); //and respawn. // pass the best from 
	
	std::stringstream stat();// also print best and average. and range.

	float randVal(float x);//returns random float between .0 and .2
	
	int calcVal(int L, int before, int layr, int mem, Brain *brain);

	float sigmoid(float input);

};






