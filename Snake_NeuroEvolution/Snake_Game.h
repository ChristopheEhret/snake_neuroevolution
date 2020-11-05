#pragma once
#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "Snake.h"

/*const int xSize = 10;
const int ySize = 20;
const int tileSize = 10;*/

class Snake_Game {
public:
	Snake_Game();

	void show();
	void showOne();
	bool tick();
	float fullTick(Snake *s, int maxFrames = xSize * ySize * 2);

	void reset();
	void resetApples() { apples = {}; }
	//void setSnake(Snake *s) { delete snake; snake = s; }
	//Careful : It doesn't delete former snake
	void changeSnake(Snake *s) { snake = s; }
	Snake *getSnake() { return snake; }

	void newApple();

	bool isOpen() { return window->isOpen(); }
	bool training = false;

	sf::Vector2i getApple() { return apple; }

private:
	sf::RenderWindow *window;

	sf::Font font;
	sf::Text affScore;

	Snake *snake;

	sf::Vector2i apple;
	vector<sf::Vector2i> apples;
	int indexApple = 0;

};