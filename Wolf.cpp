#pragma once
#include "stdafx.h"
#include <windows.h>
#include "Wolf.h"


// This the constructor 
Wolf::Wolf(float startX, float startY, int windowWidth, int windowHeight, int speed)
{
	//position.x = startX;
	//position.y = startY;
	
	position.x = (windowWidth * (((float)rand()) / RAND_MAX));
	position.y = (windowHeight * (((float)rand()) / RAND_MAX));

	wolfSpeed = speeds[speed];

	//wolfShape.setSize(sf::Vector2f(10, 10));
	wolfShape.setRadius(6);
	//wolfShape.setFillColor(sf::Color::Black);
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


CircleShape Wolf::getShape()
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

void Wolf::reset(float windowWidth, float windowHeight)
{
	
	position.x = (windowWidth*0.6 * (((float)rand()) / RAND_MAX) + windowWidth/5);
	position.y = (windowHeight*0.6 * (((float)rand()) / RAND_MAX) + windowHeight/5);
	
	
	//position.x = windowWidth / 2;
	//position.y = windowHeight / 2;//was windowheight - 20

	wolfShape.setPosition(position);
}

void Wolf::direct()
{
	if (Wolf::controls[0] == true) // left
	{
		// move left...
		Wolf::moveLeft();
	}
	if (Wolf::controls[1] == true)//right
	{
		// move right...
		Wolf::moveRight();
	}

	if (Wolf::controls[2] == true)//up
	{
		// move up...
		Wolf::moveUp();
	}
	if (Wolf::controls[3] == true)//down
	{
		// move down...
		Wolf::moveDown();
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
}