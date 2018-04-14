#pragma once
#include "stdafx.h"
#include "prey.h"

// This the constructor and it is called when we create an object
Prey::Prey(float startX, float startY)
{
	srand(time(NULL));
	position.x = (700/3 * (((float)rand()) / RAND_MAX)) + 700/3;
	position.y = (500/3 * (((float)rand()) / RAND_MAX)) + 500/3;

	
	//position.x = startX;
	//position.y = startY;

	preyShape.setSize(sf::Vector2f(10, 10));
	preyShape.setPosition(position);
}
void Prey::reset(int windowWidth, int windowHeight)
{
	position.x = (windowWidth / 3 * (((float)rand()) / RAND_MAX)) + windowWidth / 3;
	position.y = (windowHeight / 3 * (((float)rand()) / RAND_MAX)) + windowHeight / 3;

	preyShape.setPosition(position);
}
FloatRect Prey::getPosition()
{
	return preyShape.getGlobalBounds();
}

float Prey::getPositionX()
{
	return position.x;
}

float Prey::getPositionY()
{
	return position.y;
}

RectangleShape Prey::getShape()
{
	return preyShape;
}

float Prey::getXVelocity()
{
	return xVelocity;
}

void Prey::reboundSides()
{
	xVelocity = -xVelocity;
}

void Prey::reboundBatOrTop()
{
	position.y -= (yVelocity * 30);
	yVelocity = -yVelocity;
}

void Prey::hitTop(int windowWidth)
{
	position.x = windowWidth / 2;
	position.y = 300;

	preyShape.setSize(sf::Vector2f(10, 10));
	preyShape.setPosition(position);
}

void Prey::hitBottom()
{
	position.y = 1;
	position.x = 500;
}

void Prey::update()
{
	// Update the ball position variables
	position.y += yVelocity;
	position.x += xVelocity;

	// Move the ball and the bat
	preyShape.setPosition(position);
}

