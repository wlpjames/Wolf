#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Prey
{
private:
	Vector2f position;

	// A RectangleShape object called ref
	RectangleShape preyShape;

	float xVelocity = .5f;
	float yVelocity = .5f;

public:
	Prey(float startX, float startY);

	void reset(int windowWidth, int windowHeight);

	FloatRect getPosition();

	float getPositionX();
	float getPositionY();

	RectangleShape getShape();

	float getXVelocity();

	void reboundSides();

	void reboundBatOrTop();

	void hitTop(int windowWidth);

	void hitBottom();

	void update();

};

