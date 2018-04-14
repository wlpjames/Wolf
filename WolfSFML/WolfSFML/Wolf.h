#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wolf
{
public:
	Vector2f position;

	// A RectangleShape object
	RectangleShape wolfShape;

	float wolfSpeed = 2.0f;

public:
	Wolf(float startX, float startY);

	FloatRect getPosition();
	
	float getPositionX();
	float getPositionY();


	RectangleShape getShape();

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void leap(int i);

	void update();

	void reset(int windowWidth, int windowHeight);

};
