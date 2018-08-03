
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

int Loop::draw(sf::RenderWindow *window, Wolf* wolf, Prey prey)
{
	window->setFramerateLimit(fps);
	
	// Clear everything from the last frame
	window->clear(Color(26, 128, 182, 255));
	
	window->draw(wolf->getShape());
	window->draw(prey.getShape());

	// Draw our score
	window->draw(hud);
	window->draw(stats);


	// Show everything we just drew
	window->display();
	return 1;
}


int Loop::gameLoop(Pool* pool, int mem, int p_mem, int frameCount, Wolf *wolf, Prey *prey)
{
	//wolf stuff

	if (frameCount % 5 == 0) {
		pool->runNet(prey, wolf, mem, pool->wolfBrain, wolf->controls);
	}
	wolf->direct();

	//handle wolf hitting edge of window
	if (wolf->getPosition().top < 15)
	{
		//wolf->position.y = 15;
		wolf->position.y = windowHeight - 10;
	}
	else if (wolf->getPosition().top > windowHeight - 10)
	{
		wolf->position.y = 15;
		//wolf->position.y = windowHeight - 10;
	}
	if (wolf->getPosition().left < 0)
	{
		//wolf->position.x = 0;
		wolf->position.x = windowWidth - 10;
	}
	else if (wolf->getPosition().left + 10 > windowWidth)
	{
		//wolf->position.x = windowWidth - 10;
		wolf->position.x = 0;
	}
	wolf->update();

	if (pool->values.prey_mobile == 1)
	{
		if (pool->values.prey_active == 0)
		{
			p_inactive(prey, pool);
			prey->direct_inactive();
		}
		else
		{
			if ((frameCount + 5) % 5 == 0)
			{
				pool->runNet(prey, wolf, p_mem, pool->preyBrain, prey->controls);
			}
			prey->direct();
		}
			//handle prey hiiting edge of window.
		if (prey->getPosition().top < 15)
		{
			//prey->position.y = 15;
			prey->position.y = windowHeight - 10;
		}
		else if (prey->getPosition().top > windowHeight - 10)
		{
			//prey->position.y = windowHeight - 10;
			prey->position.y = 15;
		}
		if (prey->getPosition().left < 0)
		{
			//prey->position.x = 0;
			prey->position.x = windowWidth - 10;
		}
		else if (prey->getPosition().left + 10 > windowWidth)
		{
			//prey->position.x = windowWidth - 10;
			prey->position.x = 0;
		}
		
		prey->update();
	}
	
	// prey gets caught?

	if (prey->getPosition().intersects(wolf->getPosition()))
	{
		prey->reset(windowWidth, windowHeight);
		score++;
	}
	//prey doesnt get caught and scores // this system needs some more work.
	if ((prey->getPositionX() < (windowWidth / feedGround) * (feedGround - 1) && prey->getPositionX() > windowWidth / feedGround) 
		&& (prey->getPositionY() < (windowHeight / feedGround) * (feedGround - 1) && prey->getPositionY() > windowHeight / feedGround))
	{
		if (score == 0)
		{
			p_score++;
		}
	}
	
	return score;
}
	


int Loop::animate(sf::RenderWindow *window, Pool* pool, Pool *cpy, thread_data* ptrs, void(*f) (thread_data* ptrs))
{
	//continualy send one frame to loop
	//add wolf and prey.

	Wolf wolf(pool->values.Width / 2, pool->values.Height / 2, pool->values.Width, pool->values.Height, pool->values.wolfSpeed);


	Prey prey(pool->values.Width / 2, 300, pool->values.preySpeed);
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

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			//wait untill space is released
			while (Keyboard::isKeyPressed(Keyboard::Space))
			{

			}
			//pull up menu
			menu(window, pool);
		}
		if (Keyboard::isKeyPressed(Keyboard::BackSpace))
		{
			return 0;
		}
		gameLoop(pool, champ, p_champ, frameCount, &wolf, &prey);
		
		// Update the HUD text
		std::stringstream ss;
		ss << "Wolf";
		hud.setString(ss.str());
		
		if (pool->buisy == true)
		{
			ss << "\n Buisy";
		}
		stats.setString(pstream->str());

		draw(window, &wolf, prey);
		
		frameCount++;

	}
	
	return 0;
}

void Loop::train(Pool* pool, int sec)
{
	for (int i = 0; i < pool->wolfBrain->poolSize; i++)
	{
		Wolf wolf(pool->values.Width / 2, pool->values.Height / 2, pool->values.Width, pool->values.Height, pool->values.wolfSpeed);
		Prey prey(pool->values.Width / 2, 300, pool->values.preySpeed);
		score = 0;
		p_score = 0;
		for (int framecount = 0; framecount < fps * sec; framecount++)
		{
			gameLoop(pool, i, i, framecount, &wolf, &prey);
		}
		pool->wolfBrain->results[i] = score;
		pool->preyBrain->results[i] = p_score;
	}
	
	return;
}



void Loop::p_inactive(Prey *prey, Pool *pool)
{
	if (prey->getPosition().top < windowHeight / pool->values.prey_constraint || prey->getPosition().top >(windowHeight / pool->values.prey_constraint) * (pool->values.prey_constraint - 1))
	{
		prey->reboundBatOrTop();

	}
	// Handle prey hitting sides
	if (prey->getPosition().left < windowWidth / pool->values.prey_constraint || prey->getPosition().left + 10 >(windowWidth / pool->values.prey_constraint) * (pool->values.prey_constraint - 1))
	{
		prey->reboundSides();
	}
	return;
}