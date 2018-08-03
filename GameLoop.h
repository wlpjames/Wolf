#pragma once
#include <SFML/Graphics.hpp>
#include "Wolf.h"
#include "prey.h"
#include "neural_net.h"
#include "main.h"

using namespace sf;

class Loop
{
public:

	//int frameCount;
	int wolfpack = 2;
	int preyPack;
	int score;
	int p_score;
	bool display;
	float windowHeight = 500;
	float windowWidth = 700;
	int fps = 40;
	sf::Text hud;
	sf::Text stats;
	Font font;
	std::stringstream* pstream;

	
	int feedGround = 3;

	Loop();//contructor

	int draw(sf::RenderWindow *window, Wolf* wolf, Prey prey);
	int gameLoop(Pool* pool, int mem, int p_mem, int frameCount, Wolf *wolf, Prey *prey);
	
	int animate(sf::RenderWindow *window, Pool* pool, Pool *cpy, thread_data* ptrs, void(*f) (thread_data* ptrs));
	void train(Pool* pool, int sec);
	void p_inactive(Prey *prey, Pool *pool);

};
