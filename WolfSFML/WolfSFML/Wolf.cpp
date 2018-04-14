#pragma once
#include "stdafx.h"
#include <windows.h>
#include "Wolf.h"


// This the constructor 
Wolf::Wolf(float startX, float startY)
{
	position.x = startX;
	position.y = startY;

	wolfShape.setSize(sf::Vector2f(10, 10));
	wolfShape.setPosition(position);
}

FloatRect Wolf::getPosition()
{
	return wolfShape.getGlobalBounds();
}

float Wolf::getPositionX()
{
	return position.x;
}

float Wolf::getPositionY()
{
	return position.y;
}


RectangleShape Wolf::getShape()
{
	return wolfShape;
}

void Wolf::moveLeft()
{
	position.x -= wolfSpeed;
}

void Wolf::moveRight()
{
	position.x += wolfSpeed;
}

void Wolf::moveUp()
{
	position.y -= wolfSpeed;
}

void Wolf::moveDown()
{
	position.y += wolfSpeed;
}

void Wolf::leap(int i)
{
	if (i == 0)
	{
		wolfSpeed = .5f;
	}
	else
	{
		wolfSpeed = .2f;
	}
}

void Wolf::update()
{
	wolfShape.setPosition(position);
}

void Wolf::reset(int windowWidth, int windowHeight)
{
	
	//position.x = (windowWidth * (((float)rand()) / RAND_MAX));
	//position.y = (windowHeight * (((float)rand()) / RAND_MAX));
	
	
	position.x = windowWidth / 2;
	position.y = windowHeight / 2;//was windowheight - 20

	wolfShape.setPosition(position);
}