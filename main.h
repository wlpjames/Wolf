#pragma once
#include "neural_net.h"
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




//void deconstruct(void);
void train(thread_data* ptrs);