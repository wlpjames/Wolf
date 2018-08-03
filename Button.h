#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;
class Button
{
public:
	int h;
	int w;
	Vector2f position;
	sf::RectangleShape buttonShape;
	Text text;
	Font *font;
	int value;

	Button(float px, float py, const char *val, float inpts, Font *font);
	bool clicked(sf::RenderWindow &window);
};

class Row
{
public:
	int b;
	Button * buttons[15];
	//int selected = 0;
	Text text; 
	Font font;
	int *value_destination;

	
	Row(const char *title, int *dest, int button,  int curr_row, const char *vals[], float inpts[]);
	~Row();
	void write(const char *title, int row);
	void draw(sf::RenderWindow *window);
	void check(sf::RenderWindow &window);
};