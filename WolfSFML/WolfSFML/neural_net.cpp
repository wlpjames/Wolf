#pragma once
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include "neural_net.h"
#include <stdio.h>
#include "vals.h"
#include <sstream>
//#include <SFML/Graphics.hpp>
//#include "globals.h"


//constructor
Pool::Pool() //sets tme seed and builds matrix
{
	

	srand(time(NULL));//sets seed for rand	perhaps set seed at reand function
	for (int b = 0; b < poolSize; b++) //initiating random values in matrix
	{
		for (int k = 0; k < layers-1; k++) // for 2 layers + output replace three with kth letter.
		{
			for (int i = 0; i < LYR; i++) // lyr == 10
			{
				for (int j = 0; j < LYR; j++) // lyr == 10
				{
					w_matrix[b][k][i][j] = randVal(1) - .5; //random number between 0.5 & -0.5
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////

int Pool::runNet(float wolfX, float wolfY, float preyX, float preyY, int framecount, int mem, bool* controls)
{// input pointer to controls array, results array, and member of pool.

	//set values for inputs.
	v_matrix[0][0] = wolfX; //distance to top
	v_matrix[0][1] = wolfY;
	v_matrix[0][2] = preyX;
	v_matrix[0][3] = preyY;
	//v_matrix[0][4] = wolfX - preyX;
	//v_matrix[0][5] = wolfY - preyY;
	//v_matrix[0][4] = Width/2 - wolfY;//distance from center x
	//v_matrix[0][5] = Height/2 - wolfX;//distance from center x
	v_matrix[0][4] = (preyX - wolfX) + (preyY- wolfY);//distance to ball
	
	//calculates vaues and outputs
	calcVal(LYR, INPT, 1, mem);
	calcVal(LYR2, LYR, 2, mem);
	calcVal(OUT, LYR2, 3, mem);
	// send to functions here.
	
	//bool control[5];	
		if (v_matrix[3][0] > v_matrix[3][1]) { //left, right
			controls[0] = true; // result must be array
			controls[1] = false;
		}
		else {
			controls[0] = false;
			controls[1] = true;
		}
		
		if (v_matrix[3][2] > v_matrix[3][3]) 
		{ //up, down
			controls[2] = true; // result must be array
			controls[3] = false;
		}
		else 
		{
			controls[2] = false;
			controls[3] = true;
		}
	
	
	return 0;  //where recieves controls?? //set return value
	
}

int Pool::calcVal(int L, int before, int layr, int mem)
{	
	for (int j = 0; j < L; j++)
	{
		v_matrix[layr][j] = 0; // eek wipes values from last test.
		for (int k = 0; k < before; k++)
		{
			v_matrix[layr][j] += (v_matrix[layr-1][k] * w_matrix[mem][layr][j][k]);
		}
		//v_matrix[layr][j] = v_matrix[layr][j] / L; // averages out values.
	}
	return 0;
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
		if (results[i] < low)
		{
			low = results[i];
		}
		if (results[i] > high)
		{
			high = results[i];
		}
		mean += results[i];
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

static int __cdecl pair_score_comparator(void const *left, void const *right)
{
	struct pair const *l = (struct pair *)left, *r = (struct pair *)right;

	return r->score < l->score ? -1 : (r->score == l->score ? 0 : 1);
}

int Pool::eval() // will need to find somthing better than just score
{
	
	////finds champion
	float buffer = 0;
	for (int i = 0; i < poolSize; i++) 
	{
		if (results[i] > buffer)
		{
			buffer = results[i];//givest first lowest. 
			champion = i;
		}
	}

	// finds x amount of survivors.
	
	struct pair *pairs = (struct pair *)malloc(sizeof(struct pair) * poolSize);
	for (int i = 0; i < poolSize; i++)
	{
		pairs[i].pos = i;
		pairs[i].score = results[i];
	}

	qsort(pairs, poolSize, sizeof(struct pair), pair_score_comparator);
	for (int i = 0; i < pick; i++)
	{
		survivors[i] = pairs[i].pos;
	}

	free(pairs);

	/*

	for (int i = 0; i < pick; i++)
	{
		buffer = 0;
		for (int j = 0; j < poolSize; j++) 
		{
			if (results[j] >= buffer)
			{
				bool done = false;
				for (int k = 0; k < pick; k++)
				{
					
					if (j == survivors[k])// survivers should be set to 0.
					{
						done = true;
					}
				}
				if (done == false)
				{
					buffer = results[j];
					survivors[i] = j;
				}
				
			}
		}
	}
	/*for (int j = 0; j < pick; j++)
	{
		buffer = 0;
		for (int i = 0; i < poolSize; i++) //finds 10 highest
		{
			if (results[i] > buffer)
			{
				buffer = results[i];//givest first lowest.
				results[i] = 0; //btw this is bad as is makes results unusable
				survivors[j] = i;
			}
		}
	}*/
	return champion;
}

///////////////////////////////////////////////////////////////////////////////////////

bool Pool::mutate()//takes W_matrix, survivors, destroys others, mutats
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
					mutants[b*(poolSize / pick)][k][i][j] = w_matrix[survivors[b]][k][i][j]; 
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
						if (coin < .075)
						{
							mutants[x + (b * (poolSize / pick))][k][i][j] = mutants[b * (poolSize / pick)][k][i][j] + (((randVal(1)-0.5)/*/100*/)); //should give val of 0.0 to 0.1
			
						}
						else
						{
							mutants[x + (b * (poolSize / pick))][k][i][j] = mutants[b * (poolSize / pick)][k][i][j];
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
					w_matrix[b][k][i][j] = mutants[b][k][i][j];
				}
			}
		}
	}
#else
	memmove(w_matrix, mutants, sizeof( w_matrix ));
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

///////////////////////////////////////////////////////////////////////////////////////

int Pool::runSilent(Wolf wolf, Prey prey, int windowWidth, int windowHeight)
{
	
	for (int mem = 0; mem < poolSize; mem++)
	{
		//reset wolf
		wolf.reset(windowWidth, windowHeight);
		
		//reset prey
		prey.reset(windowWidth, windowHeight);

		//reset score
		float score = 0;

		for (int frameCount = 0; frameCount < 200; frameCount++)// (frameCount != 100) // while frame count == meh
		{
			/*
			Handle the player input
			*********************************************************************
			*********************************************************************
			*********************************************************************
			*/
			

			//int mem = pool.champion;
			//control array...
			bool controls[5]; //left, right, up, down, leap ???is pointer???
			
			if (frameCount % 2 == 0) {
				runNet(wolf.getPositionX(), wolf.getPositionY(), prey.getPositionX(), prey.getPositionY(), frameCount, mem, controls); // what are inputs??? // wolf poss, prey poss, framcount
			}

			if (controls[0] == true) // left
			{
				// move left...
				wolf.moveLeft();
			}
			if (controls[1] == true)//right
			{
				// move right...
				wolf.moveRight();
			}

			if (controls[2] == true)//up
			{
				// move up...
				wolf.moveUp();
			}
			if (controls[3] == true)//down
			{
				// move down...
				wolf.moveDown();
			}

			/*if (controls[5] == true)//leap
			{
				if (frameCount > 1000)
				{
					//leap
					wolf.leap(0);
					//counts ten frames before stopping
					frameCount = 0;
				}
			}*/

			/*
			Update the frame
			*********************************************************************
			*********************************************************************
			*********************************************************************
			*/


			// Handle prey hitting top 
			if (prey.getPosition().top < windowHeight / 3 || prey.getPosition().top >(windowHeight / 3) * 2)
			{
				prey.reboundBatOrTop();

			}

			// Handle prey hitting sides
			if (prey.getPosition().left < windowWidth / 3 || prey.getPosition().left + 10 >(windowWidth / 3) * 2)
			{
				prey.reboundSides();
			}

			//handle wolf hiiting 
			if (wolf.getPosition().top < 15)
			{
				wolf.position.y = 15;
				//score -= 0.001;
			}
			else if (wolf.getPosition().top > windowHeight-10)
			{
				wolf.position.y = windowHeight-10;
				//score -= 0.001;
			}


			// Handle ball hitting sides
			if (wolf.getPosition().left < 0)
			{
				wolf.position.x = 0;
				//score -= 0.001;
			}
			else if (wolf.getPosition().left + 10 > windowWidth)
			{
				wolf.position.x = windowWidth - 10;
				//score -= 0.001;
			}

			// Has the ball hit the bat?
			if (prey.getPosition().intersects(wolf.getPosition()))
			{
				// Hit detected so reverse the ball and score a point
				//prey.reboundBatOrTop();
				prey.reset(windowWidth, windowHeight);
				//prey.hitTop(windowWidth);
				// Add a point to the players scor
				score++;
			}


			//frameCount++;
			/*if (frameCount == 50)
			{
				wolf.leap(1);
			}*/


			prey.update();
			wolf.update();

			/*
			Draw the frame
			*********************************************************************
			*********************************************************************
			*********************************************************************
			*/
		}// This is the end of the "while" loop
		results[mem] = score;
	}
	return 0;
}
