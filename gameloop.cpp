
#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>
#include <cstdlib>
#include "GameLoop.h"
#include "Wolf.h"
#include "prey.h"
#include "main.h"
#include <stdio.h>
#include "neural_net.h"
#include <windows.h>
#include <thread> 

using namespace sf;

Loop::Loop()
{
	
	font.loadFromFile("Chapaza.ttf");
	hud.setFont(font);
	stats.setFont(font);
	// Set size
	hud.setCharacterSize(30);
	stats.setCharacterSize(20);
	// Choose a color
	hud.setFillColor(sf::Color::White);
	stats.setFillColor(sf::Color::Black);
}

int Loop::draw(sf::RenderWindow *window, Wolf* wolf[2], Prey prey)
{
	window->setFramerateLimit(fps);
	
	// Clear everything from the last frame
	window->clear(Color(26, 128, 182, 255));
	for (int i = 0; i < wolfpack; i++)
	{
		window->draw(wolf[i]->getShape());
	}
	window->draw(prey.getShape());


	// Draw our score
	window->draw(hud);
	window->draw(stats);


	// Show everything we just drew
	window->display();
	return 1;
}


int Loop::gameLoop(Pool* pool, int mem, int p_mem, int frameCount, Wolf *wolf[2], Prey *prey)
{
	for (int i = 0; i < wolfpack; i++)
	{
		if (frameCount % 10 == 0) {
			pool->runNet(prey, wolf[i], wolf[i == 0 ? 1 : 0], wolf[i]->pool_mem, pool->wolfBrain, wolf[i]->controls);
		}
		wolf[i]->direct();
		if (wolf[i]->getPosition().top < 15)
		{
			wolf[i]->position.y = 15;
		}
		else if (wolf[i]->getPosition().top > windowHeight - 10)
		{
			wolf[i]->position.y = windowHeight - 10;
		}
		if (wolf[i]->getPosition().left < 0)
		{
			wolf[i]->position.x = 0;
		}
		else if (wolf[i]->getPosition().left + 10 > windowWidth)
		{
			wolf[i]->position.x = windowWidth - 10;
		}
	}
	/*if (frameCount % 10 == 0) {
		pool->runNet(prey, wolf[0], wolf[1], p_mem, pool->preyBrain, prey->controls);
	}*/

	//prey bounces
	p_inactive(prey);
	//prey->direct();
	
	
	
	//handle prey hiiting 
	if (prey->getPosition().top < 15)
	{
		prey->position.y = 15;
	}
	else if (prey->getPosition().top > windowHeight - 10)
	{
		prey->position.y = windowHeight - 10;
	}
	if (prey->getPosition().left < 0)
	{
		prey->position.x = 0;
	}
	else if (prey->getPosition().left + 10 > windowWidth)
	{
		prey->position.x = windowWidth - 10;
	}
	
	
	// prey gets caught?
	for (int i = 0; i < wolfpack; i++)
	{
		if (prey->getPosition().intersects(wolf[i]->getPosition()))
		{
			prey->reset(windowWidth, windowHeight);
			score++;
		}
	}
	if ((prey->getPositionX() < (windowWidth / 5) * 4 && prey->getPositionX() > windowWidth / 5) && (prey->getPositionY() < (windowHeight / 5) * 4 && prey->getPositionY() > windowHeight / 5))
	{
		if (score == 0)
		{
			p_score++;
		}
	}
	
	prey->update();
	for (int i = 0; i < wolfpack; i++)
	{
		wolf[i]->update();
	}
	return score;
	
}
	


int Loop::animate(sf::RenderWindow *window, Pool* pool, Pool *cpy, thread_data* ptrs, void(*f) (thread_data* ptrs))
{
	//continualy send one frame to loop
	//add wolf and prey.
	
	Wolf* wolf[2];
	for (int i = 0; i < wolfpack; i++)
	{
		wolf[i] = new Wolf(pool->Width / 2, pool->Height / 2, pool->Width, pool->Height);
		wolf[i]->pool_mem = pool->wolfBrain->survivors[i];
	}

	//Wolf *wolf = new Wolf[2];

	Prey prey(pool->Width / 2, 300);
	int champ = pool->wolfBrain->champion;
	
	int p_champ = pool->preyBrain->champion;
	int frameCount = 0;
	while (champ == pool->wolfBrain->champion && p_champ == pool->preyBrain->champion && window->isOpen() == true)
	{
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}
		
		if (Keyboard::isKeyPressed(sf::Keyboard::Return) && pool->buisy != true)
		{
			std::thread train((*f), ptrs);
			train.detach();
			pool->buisy = true;
		}
		


		gameLoop(pool, champ, p_champ, frameCount, wolf, &prey);
		
		// Update the HUD text
		std::stringstream ss;
		ss << "Wolf";
		hud.setString(ss.str());
		
		if (pool->buisy == true)
		{
			ss << "\n Buisy";
		}
		stats.setString(pstream->str());

		draw(window, wolf, prey);
		
		frameCount++;

	}
	
	//free memory
	for (int i = 0; i < wolfpack; i++)
	{
		free(wolf[i]);
	}
	return 0;
}

void Loop::train(Pool* pool, int sec)
{
	for (int i = 0; i < pool->poolSize; i++)
	{
		Wolf* wolf[2];
		for (int j = 0; j < wolfpack; j++)
		{
			wolf[j] = new Wolf(pool->Width / 2, pool->Height / 2, pool->Width, pool->Height);
			wolf[j]->pool_mem = i;
		}
		Prey prey(pool->Width / 2, 300);
		score = 0;
		p_score = 0;
		for (int framecount = 0; framecount < fps * sec; framecount++)
		{
			gameLoop(pool, i, i, framecount, wolf, &prey);
		}
		pool->wolfBrain->results[i] = score;
		pool->preyBrain->results[i] = p_score;

		for (int j = 0; j < wolfpack; j++)
		{
			free(wolf[j]);
		}
	}
	
	return;
}



void Loop::p_inactive(Prey *prey)
{
	if (prey->getPosition().top < windowHeight / 3 || prey->getPosition().top >(windowHeight / 3) * 2)
	{
		prey->reboundBatOrTop();

	}
	// Handle prey hitting sides
	if (prey->getPosition().left < windowWidth / 3 || prey->getPosition().left + 10 >(windowWidth / 3) * 2)
	{
		prey->reboundSides();
	}
	return;
}