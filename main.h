#pragma once
#include "neural_net.h"
#include "Button.h"


struct thread_data {
	Pool* pool;
	sf::RenderWindow* window;
	Pool* cpy;
	std::stringstream* stream;
	bool reset = false;
	bool closed = false;
	bool buisy = false;



	int windowWidth;
	int windowHeight;
};


void train(thread_data* ptrs);
void menu(sf::RenderWindow *window, Pool* pool);
void wolf_menu(Pool *pool, Row *rows[10]);
void prey_menu(Pool *pool, Row *rows[10]);
void trainMenu(Pool *pool, Row *rows[10]);
