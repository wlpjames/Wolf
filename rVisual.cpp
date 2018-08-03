
#include "stdafx.h"
#include "Wolf.h"
#include "prey.h"
#include <sstream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include "player.h"
#include "neural_net.h"
#include <stdio.h>

using namespace sf;

// This is where our game starts from
int main()
{
	int windowWidth = 700;
	int windowHeight = 500;
	// Make a window that is 1024 by 768 pixels
	// And has the title "Pong"
	RenderWindow window(VideoMode(windowWidth, windowHeight), "Wolf");

	int frameCount = 1000;//used to time jump
	int score = 3;

	// create a bat
	Wolf wolf(windowWidth / 2, windowHeight - 20);

	// create a ball
	Prey prey(windowWidth / 2, 1);

	// Create a "Text" object called "message". Weird but we will learn about objects soon
	Text hud;

	// We need to choose a font
	Font font;
	font.loadFromFile("Chapaza.ttf");

	// Set the font to our message
	hud.setFont(font);

	// Set size
	hud.setCharacterSize(30);

	// Choose a color
	hud.setFillColor(sf::Color::White);


	while (window.isOpen())
	{
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


		int mem = champion;
		//control array...
		bool controls[5]; //left, right, up, down, leap ???is pointer???
		run(wolf.getPositionX(), wolf.getPositionY(), prey.getPositionX(), prey.getPositionY(), frameCount, mem, controls); // what are inputs??? // wolf poss, prey poss, framcount


		if (controls[0] == true) // left
		{
			// move left...
			wolf.moveLeft();
		}
		else if (controls[1] == true)//right
		{
			// move right...
			wolf.moveRight();
		}

		if (controls[3] == true)//up
		{
			// move up...
			wolf.moveUp();
		}
		else if (controls[4] == true)//down
		{
			// move down...
			wolf.moveDown();
		}

		if (controls[5] == true)//leap
		{
			if (frameCount > 1000)
			{
				//leap
				wolf.leap(0);
				//counts ten frames before stopping
				frameCount = 0;
			}
		}

		else if (Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			// quit...
			// Someone closed the window- bye
			window.close();
		}

		/*
		Update the frame
		*********************************************************************
		*********************************************************************
		*********************************************************************
		*/


		// Handle ball hitting top 
		if (prey.getPosition().top < 0 || prey.getPosition().top > windowHeight)
		{
			prey.reboundBatOrTop();
		}


		// Handle ball hitting sides
		if (prey.getPosition().left < 0 || prey.getPosition().left + 10 > windowWidth)
		{
			prey.reboundSides();
		}


		// Has the ball hit the bat?
		if (prey.getPosition().intersects(wolf.getPosition()))
		{
			// Hit detected so reverse the ball and score a point
			prey.reboundBatOrTop();
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

		// Show everything we just drew
		window.display();
	}// This is the end of the "while" loop
	return 0;
}