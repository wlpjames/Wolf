#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Wolf
{
public:
	Vector2f position;

	// A RectangleShape object
	sf::CircleShape wolfShape;
	float wolfSpeed = 2.0f;
	int pool_mem = 0;

public:
	Wolf(float startX, float startY,int windowWidth, int windowHieght);

	FloatRect getPosition();
	
	float getPositionX();
	float getPositionY();
	
	bool controls[4];

	CircleShape getShape();

	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	void leap(int i);

	void update();

	void reset(float windowWidth, float windowHeight);

	void direct();
};
