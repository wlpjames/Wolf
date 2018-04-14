
#include "stdafx.h"
#include "Wolf.h"
#include "prey.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>


#include "neural_net.h"
#include <stdio.h>
//#include "globals.h"


using namespace sf;

int main()
{
	int windowWidth = 700;
	int windowHeight = 500;
	bool mutating = false;
	bool evaluating = false;

	sf::Clock Clock;

	// Make a window that is 1024 by 768 pixels
	// And has the title "Pong"
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Wolf");

	int frameCount = 1000;//used to time jump
	float score = 0;

	// create a bat
	Wolf wolf(windowWidth / 2, windowHeight - 2);

	// create a ball
	Prey prey(windowWidth / 2, 300);

	// Create a "Text" object called "message". Weird but we will learn about objects soon
	Text hud;
	Text stats;
	std::stringstream stt;


	//create an evolutionary pool
	Pool pool; 

	// font
	Font font;
	font.loadFromFile("Chapaza.ttf");

	// Set the font to our message
	hud.setFont(font);
	stats.setFont(font);
	// Set size
	hud.setCharacterSize(30);
	stats.setCharacterSize(20);
	// Choose a color
	hud.setFillColor(sf::Color::White);
	stats.setFillColor(sf::Color::Black);
	int champion = 0;

	while (window.isOpen())
	{
		//sets frametime


		window.setFramerateLimit(30);
		/*
		Handle the player input
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				// Someone closed the window- bye
				window.close();
		}


		//int mem = pool.champion;
		//control array...
		bool controls[5]; //left, right, up, down, leap ???is pointer???
		
		if (frameCount % 2 == 0) {
			pool.runNet(wolf.getPositionX(), wolf.getPositionY(), prey.getPositionX(), prey.getPositionY(), frameCount, champion, controls); // what are inputs??? // wolf poss, prey poss, framcount
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

		if (Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			// quit...
			window.close();
		}
		if (Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			
			evaluating = true;
			pool.runSilent( wolf, prey, windowWidth, windowHeight);
			stt = pool.stat();
			champion = pool.eval();
			//print stats;
			wolf.reset(windowWidth, windowHeight);

		}
		if (Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			mutating = true;
			pool.mutate();
		}

		if (Keyboard::isKeyPressed(sf::Keyboard::RShift))
		{
			for (int i = 0; i < 20; i++)
			{
				evaluating = true;
				pool.runSilent(wolf, prey, windowWidth, windowHeight);
				stt = pool.stat();
				champion = pool.eval();
				//print stats;
				wolf.reset(windowWidth, windowHeight);
				mutating = true;
				pool.mutate();
			}
		}
		/*
		Update the frame
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/


		// Handle prey hitting top 
		if (prey.getPosition().top < windowHeight/3 || prey.getPosition().top > (windowHeight/3)*2)
		{
			prey.reboundBatOrTop();

		}
		
		// Handle prey hitting sides
		if (prey.getPosition().left < windowWidth/3 || prey.getPosition().left + 10 > (windowWidth/3)*2)
		{
			prey.reboundSides();
		}
		
		//handle wolf hiiting 
		if (wolf.getPosition().top < 15)
		{			
			wolf.position.y = 15;
		}
		else if (wolf.getPosition().top > windowHeight-10)
		{
			wolf.position.y = windowHeight - 10;
		}

		
		// Handle ball hitting sides
		if (wolf.getPosition().left < 0)
		{
			wolf.position.x = 0;
		}
		else if (wolf.getPosition().left + 10 > windowWidth)
		{
			wolf.position.x = windowWidth - 10;
		}

		// prey gets caught?
		if (prey.getPosition().intersects(wolf.getPosition()))
		{
			// Hit detected so reverse the ball and score a point
			prey.reset(windowWidth, windowHeight);
			//prey.hitTop(windowWidth);
			// Add a point to the players score
			score++;
		}
		


		frameCount++;
		if (frameCount == 50)
		{
			wolf.leap(1);
		}


		prey.update();
		wolf.update();

		// Update the HUD text
		std::stringstream ss;
		ss << "Wolf";
		hud.setString(ss.str());
		stats.setString(stt.str());

		/*
		Draw the frame
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/

		// Clear everything from the last frame
		window.clear(Color(26, 128, 182, 255));

		window.draw(wolf.getShape());

		window.draw(prey.getShape());


		// Draw our score
		window.draw(hud);
		window.draw(stats);

		// Show everything we just drew
		window.display();
	}// This is the end of the "while" loop
	return 0;
}

