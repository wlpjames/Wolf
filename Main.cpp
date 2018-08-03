
#include "stdafx.h"
#include "Wolf.h"
#include "prey.h"
#include "main.h"
#include "GameLoop.h"
#include "Button.h"
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
	menu(&window, &pool);
	pool.populate(pool.wolfBrain);
	pool.populate(pool.preyBrain);

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
	delete(pool.wolfBrain);
	delete(pool.preyBrain);
	return 0;
}



void train(thread_data* ptrs)
{
	Pool copy;
	Loop gloop;
	gloop.pstream = ptrs->stream;//sends stats back through a pointer
	
	//why is memcopyingnotworking???
	memcpy(copy.wolfBrain, ptrs->pool->wolfBrain, sizeof(copy.wolfBrain));
	memcpy(copy.preyBrain, ptrs->pool->preyBrain, sizeof(copy.wolfBrain));

	memcpy(copy.wolfBrain->w_matrix, ptrs->pool->wolfBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));
	memcpy(copy.preyBrain->w_matrix, ptrs->pool->preyBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));

	/*memcpy(&copy.wolfBrain->vals, &ptrs->pool->wolfBrain->vals, sizeof(copy.wolfBrain->vals));
	memcpy(&copy.preyBrain->vals, &ptrs->pool->preyBrain->vals, sizeof(copy.wolfBrain->vals));*/


	memcpy(&copy.values, &ptrs->pool->values, sizeof(copy.values));
	
	for (int i = 0; i < copy.values.trainLen; i++)
	{
					//send to functions
		gloop.train(&copy, 7);
		ptrs->pool->stream = copy.stat();
		//for wolf
		if (copy.values.wolfTrain == 1)
		{
			copy.wolfBrain->champion = copy.eval();
			copy.mutate(copy.wolfBrain);//for wolf
			memcpy(ptrs->pool->wolfBrain->w_matrix, copy.wolfBrain->w_matrix, sizeof(copy.wolfBrain->w_matrix));
			ptrs->pool->wolfBrain->champion = copy.wolfBrain->champion;
		}
		if (copy.values.preyTrain == 1)
		{
			//for prey
			copy.preyBrain->champion = copy.pEval();//make an eval for prey. now will learn to find eachother
			copy.mutate(copy.preyBrain);//for prey
			memcpy(ptrs->pool->preyBrain->w_matrix, copy.preyBrain->w_matrix, sizeof(copy.preyBrain->w_matrix));
			ptrs->pool->preyBrain->champion = copy.preyBrain->champion;
		}
						
	}
	delete(copy.wolfBrain);
	delete(copy.preyBrain);
	ptrs->pool->buisy = false;
	return;
			
}
void menu(RenderWindow *window, Pool *pool)
{
	const int r = 1;
	Row* header[r];

	int screen = 0;

	//prepares header
	const char *vals1[3] = { "train", "wolf", "prey" };
	float inpt1[3] = { 0, 1, 2 };
	header[0] = new Row("", &screen, 3, -1, vals1, inpt1);

	//prepares rows for training options
	const int tr = 6;
	Row *tRows[10];
	trainMenu(pool, tRows);

	//prepares rows for wolf options
	const int wr = 7;
	Row *wRows[10];
	wolf_menu(pool, wRows);

	//prepares rows for prey options
	const int pr = 7;
	Row *pRows[10];
	prey_menu(pool, pRows);

	//how do i check prey and wolf menus while running, perhaps wolf_menu returns pointer array of rows.
	while ((!Keyboard::isKeyPressed(Keyboard::Space)) && (window->isOpen() == true))
	{
		window->clear(Color(70, 150, 220, 255));

		//load buttons
		for (int i = 0; i < r; i++)
		{
			header[i]->draw(window);
			header[i]->check(*window);
		}
		if (screen == 0)
		{
			for (int i = 0; i < tr; i++)
			{
				tRows[i]->draw(window);
				tRows[i]->check(*window);
			}
		}
		else if (screen == 1)
		{
			for (int i = 0; i < wr; i++)
			{
				wRows[i]->draw(window);
				wRows[i]->check(*window);
			}
		}
		else if (screen == 2)
		{
			for (int i = 0; i < pr; i++)
			{
				pRows[i]->draw(window);
				pRows[i]->check(*window);
			}
		}
		window->display();
	}while (Keyboard::isKeyPressed(Keyboard::Space)) {}

	for (int i = 0; i < r; i++)
	{
		delete(header[i]);
	}
	for (int i = 0; i < tr; i++)
	{
		delete(tRows[i]);
	}
	for (int i = 0; i < wr; i++)
	{
		delete(wRows[i]);
	}
	for (int i = 0; i < pr; i++)
	{
		delete(pRows[i]);
	}
}

void prey_menu(Pool *pool, Row *rows[10])
{
	

	const char *vals1[4] = {"2000", "1000", "500", "100"};
	float inpt1[4] = {2000, 1000, 500, 100};
	rows[0] = new Row ("poolsize", &pool->preyBrain->poolSize, 4, 0 ,vals1, inpt1);

	const char *vals2[5] = { "500", "250", "125", "100", "50" };
	float inpt2[5] = {500, 250, 125, 100, 50};
	rows[1] = new Row("Survivor size", &pool->preyBrain->pick, 5, 1, vals2, inpt2);
	
	//hidden layers
	const char *vals3[] = { "1", "2", "4", "6", "8", "10" };
	float inpt3[] = { 1, 2, 4, 6, 8, 10 };
	rows[2] = new Row("hidden layers", &pool->preyBrain->layers, 6, 2, vals3, inpt3);
	
	//nodes
	const char *vals4[] = { "1", "2", "4", "6", "8", "10" };
	float inpt4[] = { 2, 3, 5, 7, 9, 11 };
	rows[3] = new Row("nodes", &pool->preyBrain->LYR, 6, 3, vals4, inpt4);

	//prey mutation size
	const char *vals5[] = { "/1", "/2", "/5", "/10", "/20", "/50", "/100" };
	float inpt5[] = { 1, 2, 5, 10, 20, 50, 100 };
	rows[4] = new Row("mutation size \n -0.5 to 0.5", &pool->preyBrain->Mutation, 7, 4, vals5, inpt5);
	
	//chance of mutation
	const char *vals6[] = { "50%", "10%", "5%", "1%", "0.5%", "0.1%", "0.01%" };
	float inpt6[] = { 0, 1, 2, 3, 4, 5, 6 };
	rows[5] = new Row("chance of mutation", &pool->preyBrain->chance, 7, 5, vals6, inpt6);

	//preyspeed
	const char *vals7[] = { "1.0", "1.4", "1.8", "2.2", "2.6", "3.0" };
	float inpt7[] = { 1, 2, 3, 4, 5, 6 };
	rows[6] = new Row("prey Speed", &pool->values.preySpeed, 6, 6, vals7, inpt7);

	return;
}
void wolf_menu(Pool *pool, Row *rows[10])
{
	const char *vals1[4] = { "2000", "1000", "500", "100" };
	float inpt1[4] = { 2000, 1000, 500, 100 };
	rows[0] = new Row("poolsize", &pool->wolfBrain->poolSize, 4, 0, vals1, inpt1);

	const char *vals2[5] = { "500", "250", "125", "100", "50" };
	float inpt2[5] = { 500, 250, 125, 100, 50 };
	rows[1] = new Row("Survivor size", &pool->wolfBrain->pick, 5, 1, vals2, inpt2);

	//hidden layers
	const char *vals3[] = { "1", "2", "4", "6", "8", "10" };
	float inpt3[] = { 1, 2, 4, 6, 8, 10 };
	rows[2] = new Row("hidden layers", &pool->wolfBrain->layers, 6, 2, vals3, inpt3);

	//nodes
	const char *vals4[] = { "1", "2", "4", "6", "8", "10" };
	float inpt4[] = { 2, 3, 5, 7, 9, 11 };
	rows[3] = new Row("nodes", &pool->wolfBrain->LYR, 6, 3, vals4, inpt4);

	//prey mutation size
	const char *vals5[] = { "/1", "/2", "/5", "/10", "/20", "/50", "/100" };
	float inpt5[] = { 1, 2, 5, 10, 20, 50, 100 };
	rows[4] = new Row("mutation size \n -0.5 to 0.5", &pool->wolfBrain->Mutation, 7, 4, vals5, inpt5);

	//chance of mutation
	const char *vals6[] = { "50%", "10%", "5%", "1%", "0.5%", "0.1%", "0.01%" };
	float inpt6[] = { 0, 1, 2, 3, 4, 5, 6 };
	rows[5] = new Row("chance of mutation", &pool->wolfBrain->chance, 7, 5, vals6, inpt6);

	//wolfspeed
	const char *vals7[] = { "1.0", "1.4", "1.8", "2.2", "2.6", "3.0" };
	float inpt7[] = { 1, 2, 3, 4, 5, 6 };
	rows[6] = new Row("Wolf Speed", &pool->values.wolfSpeed, 6, 6, vals7, inpt7);
	
	return;
}

void trainMenu(Pool *pool, Row *rows[10])
{
	const char *vals1[] = { "1 gen", "2 gen", "5 gen", "10 gen", "20 gen" };
	float inpt1[] = { 1, 2, 5, 10, 20 };
	rows[0] = new Row("train for...", &pool->values.trainLen, 5, 0, vals1, inpt1);

	const char *vals2[] = { "off", "on" };
	float inpt2[] = { 0, 1 };
	rows[1] = new Row("train wolf", &pool->values.wolfTrain, 2, 1, vals2, inpt2);

	const char *vals3[] = { "off", "on" };
	float inpt3[] = { 0, 1 };
	rows[2] = new Row("train prey", &pool->values.preyTrain, 2, 2, vals3, inpt3);

	const char *vals4[] = { "off", "on" };
	float inpt4[] = { 0, 1 };
	rows[3] = new Row("mobile prey", &pool->values.prey_mobile, 2, 3, vals4, inpt4);

	const char *vals5[] = { "off", "on" };
	float inpt5[] = { 0, 1 };
	rows[4] = new Row("active prey-mind", &pool->values.prey_active, 2, 4, vals5, inpt5);

	const char *vals6[] = { "1/2", "1/3", "1/5" };
	float inpt6[] = { 2, 3, 5 };
	rows[5] = new Row("inactive prey\n constraint", &pool->values.prey_constraint, 3, 5, vals6, inpt6);

	return;
}