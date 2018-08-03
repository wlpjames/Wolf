#pragma once
#include "stdafx.h"
#include "button.h"
#include <SFML/Graphics.hpp>

using namespace sf;


Button::Button(float px, float py, const char* val, float inpts, Font *font)
{
	position.x = px;
	position.y = py;

	value = inpts;

	buttonShape.setSize(sf::Vector2f(35, 20));
	buttonShape.setPosition(position);

	//text
	std::stringstream name;
	name << val;
	text.setFont(*font);
	text.setCharacterSize(10);
	text.setFillColor(sf::Color::Black);
	text.setPosition(px + 5, py + 5);

	text.setString(name.str());

	return;
}

bool Button::clicked(sf::RenderWindow &window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	FloatRect bounds = buttonShape.getGlobalBounds();
	
	
	if (sf::Mouse::isButtonPressed(Mouse::Button::Left) && 
		mousePos.x > bounds.left && mousePos.x < bounds.left + bounds.width && 
		mousePos.y > bounds.top && mousePos.y < bounds.top + bounds.height)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Row::Row(const char *title, int *dest, int button, int curr_row, const char *vals[], float inpts[])
{
	b = button;

	value_destination = dest;
	
	for (int j = 0; j < b; j++)
	{
		buttons[j] = new Button(75 * (j + 2), 50 * (curr_row + 2), vals[j], inpts[j], &font);
	}
	write(title, curr_row);
}

void Row::write(const char *title, int row)
{
	std::stringstream name;
	name << title;

	font.loadFromFile("Chapaza.ttf");
	text.setFont(font);
	text.setCharacterSize(10);
	text.setFillColor(sf::Color::Black);
	text.setPosition(50 , (100 + 5) + (row * 50));

	text.setString(name.str());
}

void Row::draw(sf::RenderWindow *window)
{
	
	window->draw(text);
	for (int i = 0; i < b; i++)
	{
		//sets colors
		if (*value_destination == buttons[i]->value)
		{
			buttons[i]->buttonShape.setFillColor(Color::Cyan);
		}
		else
		{
			buttons[i]->buttonShape.setFillColor(Color::Green);
		}
		//draws
		window->draw(buttons[i]->buttonShape);
		window->draw(buttons[i]->text);
	}
}

void Row::check(sf::RenderWindow &window)
{
	for (int i = 0; i < b; i++)
	{
		if (buttons[i]->clicked(window) == true)
		{
			*value_destination = buttons[i]->value;
			//selected = i + 1;
		}
	}
}

Row::~Row()
{
	for (int i = 0; i < b; i++)
	{
		free(buttons[i]);
	}
}