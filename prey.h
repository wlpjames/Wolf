#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Prey
{
public:
	Vector2f position;

	// A RectangleShape object called ref
	RectangleShape preyShape;

	float xVelocity = .8f;
	float yVelocity = .8f; //was 5
	bool controls[4];
	float preySpeed = 2.5f;

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

	void direct();

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

};

