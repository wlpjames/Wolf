
#include "stdafx.h"
#include "Wolf.h"
#include "prey.h"
#include "main.h"
#include "GameLoop.h"
#include <sstream>
#include <SFML/Graphics.hpp>
#include "neural_net.h"
#include <process.h>
 



//void gameLoop(sf::RenderWindow *window, Loop gloop, Wolf wolf, Prey prey, Pool* pool);




using namespace sf;




int main()
{
	int windowWidth = 700;
	int windowHeight = 500;


	sf::Clock Clock;
	RenderWindow window(VideoMode(windowWidth, windowHeight), "WolfCatch");

	Pool pool;
	Loop loop;
	//trainLoop.frameCount = 0;//used to time jump
	loop.score = 0;
	loop.preyPack = 1;
	loop.display = true;
	

	thread_data ptrs;
	ptrs.pool = &pool;
	ptrs.windowWidth = windowWidth;
	ptrs.windowHeight = windowHeight;
	ptrs.stream = &pool.stream;
	ptrs.reset = true;
	
	//start second thread
	
	//loop frames till window shuts
	loop.pstream = ptrs.stream;
	loop.score = 0;
	loop.display = true;

	while (window.isOpen() == true)
	{
		loop.animate(&window, &pool, ptrs.cpy, &ptrs, train);
				
	}
	ptrs.closed = true;
	free(pool.wolfBrain);
	free(pool.preyBrain);
	return 0;
}



void train(thread_data* ptrs)
{
	Pool copy;
	Loop gloop;
	gloop.pstream = ptrs->stream;
	memcpy(copy.wolfBrain->w_matrix, ptrs->pool->wolfBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));
	memcpy(copy.preyBrain->w_matrix, ptrs->pool->preyBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));
	bool wolf_t = true;


	for (int i = 0; i < 1000; i++)
	{
					//send to functions
		gloop.train(&copy, 20);
		ptrs->pool->stream = copy.stat();
		//for wolf
		if (wolf_t == false)
		{
			copy.wolfBrain->champion = copy.eval();
			copy.mutate(copy.wolfBrain, 10);//for wolf
			memcpy(ptrs->pool->wolfBrain->w_matrix, copy.wolfBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));
			ptrs->pool->wolfBrain->champion = copy.wolfBrain->champion;
		}
		if (wolf_t == true)
		{
			//for prey
			copy.preyBrain->champion = copy.pEval();//make an eval for prey. now will learn to find eachother
			copy.mutate(copy.preyBrain, 10);//for prey
			memcpy(ptrs->pool->preyBrain->w_matrix, copy.preyBrain->w_matrix, sizeof(copy.preyBrain->w_matrix));
			ptrs->pool->preyBrain->champion = copy.preyBrain->champion;
		}
						
	}
	free(copy.wolfBrain);
	free(copy.preyBrain);
	ptrs->pool->buisy = false;
	return;
			
}
