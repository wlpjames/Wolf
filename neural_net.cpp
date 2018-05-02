#pragma once
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include "neural_net.h"
#include <stdio.h>
#include "vals.h"
#include <sstream>
#include "Wolf.h"
//#include <SFML/Graphics.hpp>
//#include "globals.h"


//constructor
Pool::Pool() //sets tme seed and builds matrix
{
	srand(time(NULL));//sets seed for rand	perhaps set seed at reand function
	
	buisy = false;
	copy = false;
	wolfBrain = new Brain;
	preyBrain = new Brain;
	wolfBrain->champion = 0;
	preyBrain->champion = 0;

	for (int i = 0; i < pick; i++)
	{
		wolfBrain->survivors[i] = 0;
	}

	//insert random wolfbrain + preybrain
	for (int b = 0; b < poolSize; b++) //initiating random values in matrix
	{
		for (int k = 0; k < layers-1; k++) // for 2 layers + output replace three with kth letter.
		{
			for (int i = 0; i < LYR; i++) // lyr == 10
			{
				for (int j = 0; j < LYR; j++) // lyr == 10
				{
					wolfBrain->w_matrix[b][k][i][j] = randVal(1) - .5; //was - 0.5
					preyBrain->w_matrix[b][k][i][j] = randVal(1) - .5;//random number between 0.5 & -0.5
				}
			}
		}
	}

	
}

///////////////////////////////////////////////////////////////////////////////////

int Pool::runNet( Prey* prey, Wolf *wolf1, Wolf *wolf2, int mem, Brain* brain, bool* controls)
{// input pointer to controls array, results array, and member of pool.

	//set values for inputs.
	brain->v_matrix[0][0] = wolf1->getPositionX(); //wolf 1
	brain->v_matrix[0][1] = wolf1->getPositionY();
	brain->v_matrix[0][2] = wolf2->getPositionX(); //wolf 2
	brain->v_matrix[0][3] = wolf2->getPositionY();
	brain->v_matrix[0][4] = prey->getPositionX();
	brain->v_matrix[0][5] = prey->getPositionY();
	brain->v_matrix[0][6] = sqrt(pow(prey->getPositionX() - wolf1->getPositionX(), 2) + pow(prey->getPositionX() - wolf1->getPositionY(), 2));//distance to ball
	brain->v_matrix[0][7] = sqrt(pow(prey->getPositionX() - wolf2->getPositionX(), 2) + pow(prey->getPositionX() - wolf2->getPositionY(), 2));
	//brain->v_matrix[0][4] = (preyX - wolfX) + (preyY - wolfY);
	
	//calculates vaues and outputs //not counting from 0
	calcVal(LYR, INPT, 1, mem, brain);
	calcVal(LYR2, LYR, 2, mem, brain);
	calcVal(LYR2, LYR2, 3, mem, brain);
	calcVal(LYR2, LYR2, 4, mem, brain);
	calcVal(LYR2, LYR2, 4, mem, brain);
	calcVal(OUT, LYR2, 6, mem, brain);

	
	// send to functions here.
	
	//bool control[5];	
		if (brain->v_matrix[layers-1][0] > brain->v_matrix[layers-1][1]) { //left, right
			controls[0] = true;
			controls[1] = false;
		}
		else {
			controls[0] = false;
			controls[1] = true;
		}
		
		if (brain->v_matrix[layers-1][2] > brain->v_matrix[layers-1][3]) 
		{ //up, down
			controls[2] = true; // result must be array
			controls[3] = false;
		}
		else 
		{
			controls[2] = false;
			controls[3] = true;
		}
	
	
	return 0; 
	
}

///////////////////////////////////////////////////////////////////////////////////////

int Pool::calcVal(int L, int before, int layr, int mem, Brain* brain)
{	
	for (int j = 0; j < L; j++)
	{
		brain->v_matrix[layr][j] = 0; // eek wipes values from last test.
		for (int k = 0; k < before; k++)
		{
			brain->v_matrix[layr][j] += (brain->v_matrix[layr-1][k] * brain->w_matrix[mem][layr][j][k]);
		}
		//brain->v_matrix[layr][j] = sigmoid(brain->v_matrix[layr][j]);
		//sigmoid it here
	}
	return 0;
}

float Pool::sigmoid(float input)
{
	//float E = 2.7182818284590;
	//return 1 / (1 + pow(E, -input));
	return tanh(input);
}

///////////////////////////////////////////////////////////////////////////////////

std::stringstream Pool::stat(void)//prints perforance of last group, picks 10 survivors. 
{//wants to return a string to be printed. whilst updating champion.
	//his is now couning score!!!!!!!
	//finds range + mean
	float low = 1000;
	float high = 0;
	float mean = 0;
	for (int i = 0; i < poolSize; i++)
	{
		if (wolfBrain->results[i] < low)
		{
			low = wolfBrain->results[i];
		}
		if (wolfBrain->results[i] > high)
		{
			high = wolfBrain->results[i];
		}
		mean += wolfBrain->results[i];
		//std::cout << results[i] << " ,";
	}
	mean = mean / poolSize;
	float range = high - low;
	//could here evaluate results depending on where are in range? would probably bias.


	std::stringstream stats;
	stats << "\n mean: " << mean << "\n range_high: " << high
		<< "\n range_low: " << low<< "\n span =: " << range<< "\n";
	return stats;
}

///////////////////////////////////////////////////////////////////////////////////////

struct pair
{
	int pos, score;
};

////////////////////////////////////////////////////////////////////////////////////////

static int __cdecl pair_score_comparator(void const *left, void const *right)
{
	struct pair const *l = (struct pair *)left, *r = (struct pair *)right;

	return r->score < l->score ? -1 : (r->score == l->score ? 0 : 1);
}

/////////////////////////////////////////////////////////////////////////////////////

int Pool::eval() // will need to find somthing better than just score
{
	int champion = 0;
	////finds champion
	float buffer = 0;
	for (int i = 0; i < poolSize; i++) 
	{
		if (wolfBrain->results[i] > buffer)
		{
			buffer = wolfBrain->results[i];//givest first lowest. 
			champion = i;
		}
	}

	// finds x amount of survivors.
	
	struct pair *pairs = (struct pair *)malloc(sizeof(struct pair) * poolSize);
	for (int i = 0; i < poolSize; i++)
	{
		pairs[i].pos = i;
		pairs[i].score = wolfBrain->results[i];
	}

	qsort(pairs, poolSize, sizeof(struct pair), pair_score_comparator);
 	for (int i = 0; i < pick; i++)
	{
		wolfBrain->survivors[i] = pairs[i].pos;
		preyBrain->survivors[i] = pairs[poolSize-i-1].pos;
	}

	free(pairs);

	return champion;
}

int Pool::pEval(void)
{
	int champion = 0;
	////finds champion
	float buffer = 0;
	for (int i = 0; i < poolSize; i++)
	{
		if (wolfBrain->results[i] > buffer)
		{
			buffer = preyBrain->results[i];//givest first lowest. 
			champion = i;
		}
	}

	// finds x amount of survivors.

	struct pair *pairs = (struct pair *)malloc(sizeof(struct pair) * poolSize);
	for (int i = 0; i < poolSize; i++)
	{
		pairs[i].pos = i;
		pairs[i].score = preyBrain->results[i];
	}

	qsort(pairs, poolSize, sizeof(struct pair), pair_score_comparator);
	for (int i = 0; i < pick; i++)
	{
		preyBrain->survivors[i] = pairs[i].pos;
	}

	free(pairs);

	return champion;
}

///////////////////////////////////////////////////////////////////////////////////////

bool Pool::mutate(Brain* brain, int x)//takes W_matrix, survivors, destroys others, mutats
{
	//ith survivor = first of ten new
	for (int b = 0; b < pick; b++) 
	{
		for (int k = 0; k < layers-1; k++) // for 2 layers + output (not input)
		{
			for (int i = 0; i < LYR; i++) // lyr == 10
			{
				for (int j = 0; j < LYR; j++) // lyr == 10 wieghts
				{
					if (brain->survivors[b] == 100000000000000)
					{
						return true;
					}
					brain->mutants[b*(poolSize / pick)][k][i][j] = brain->w_matrix[brain->survivors[b]][k][i][j]; 
				}
			}
		}
		//mutates ith survaivor
		for (int x = 1; x < (poolSize / pick); x++) // x = 1 to account for b = 10 (what does that mean!!!)
		{
			for (int k = 0; k < layers-1; k++) // for 2 layers + output
			{
				for (int i = 0; i < LYR; i++) // nodes
				{
					for (int j = 0; j < LYR; j++) // weights
					{
						float coin = randVal(1);
						if (coin < .05)
						{
							brain->mutants[x + (b * (poolSize / pick))][k][i][j] =
								brain->mutants[b * (poolSize / pick)][k][i][j] + (randVal(1)/x); 
						}
						else
						{
							brain->mutants[x + (b * (poolSize / pick))][k][i][j] = brain->mutants[b * (poolSize / pick)][k][i][j];
						}
					}
				}
			}
		}
	}
#if 0
	//copys them into ith*10 aray of wMatrix
	for (int b = 0; b < poolSize; b++) //iterating random values in matrix
	{
		for (int k = 0; k < layers-1; k++) // for 2 layers + output
		{
			for (int i = 0; i < LYR; i++) // lyr == 10
			{
				for (int j = 0; j < LYR; j++) // lyr == 10
				{
					brain->w_matrix[b][k][i][j] = brain->mutants[b][k][i][j];
				}
			}
		}
	}
#else
	memmove(brain->w_matrix, brain->mutants, sizeof( brain->w_matrix ));
#endif
	/*if (w_matrix == mutants)
	{
		return true;
	}*/
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////

float Pool::randVal(float x)
{
	return x * (((float)rand()) / RAND_MAX);
}

/*Pool::~Pool(void)
{
	free(wolfBrain);
	free(preyBrain);
}*/