#pragma once
#include "stdafx.h"
#include "SFML/Graphics.hpp"
#include "VisualisationReseauNeuronal.h"

const sf::Vector2i NORTH(0, -1);
const sf::Vector2i EAST(1, 0);
const sf::Vector2i SOUTH(0, 1);
const sf::Vector2i WEST(-1, 0);

const int xSize = 30;
const int ySize = 30;
const int tileSize = 10;

const int defaultHunger = 75;

#define NBINPUTS 10

class Snake {
#define HEAD bodyParts[0]
public:
	Snake(sf::Vector2i iPos = sf::Vector2i(15, 15), int l = 4, sf::Vector2i dir = NORTH);
	Snake(std::vector<int> struc, sf::Vector2i iPos = sf::Vector2i(15, 15), int l = 4, sf::Vector2i dir = NORTH);
	Snake(Snake *parent1, Snake *parent2, sf::Vector2i iPos = sf::Vector2i(15, 15), int l = 4, sf::Vector2i dir = NORTH);
	Snake(const Snake &s);

	~Snake();

	void think(fvec e); 
	void think(sf::Vector2i apple);
	fvec inputPreprocessing(sf::Vector2i apple);
	bool backPropagation(fvec input);
	
	bool tick();
	void show(sf::RenderWindow *w);

	void go(sf::Vector2i dir);
	void addBodyPart(sf::Vector2i bodyPart);

	void initialize(sf::Vector2i iPos);
	void reset(sf::Vector2i iPos = sf::Vector2i(15, 15), int l = 4, sf::Vector2i dir = NORTH);

	float getFitness() { return fitness; }
	float getFitnessMax() { return fitnessMax; }
	void mFitness(float mf) { fitness *= mf; }
	void resetFitness() { fitness = 0; fitnessMax = 0; }
	void setFitness(float nf) { fitness = nf; }
	void setFitnessMax(float nfm) { fitnessMax = nfm; }
	void addFitness(float af) { fitness += af; }
	void calculateFitness();
	void fitnessCalculationType1();
	void fitnessCalculationType2();
	void fitnessCalculationType3();

	float getHunger() { return hunger; }
	void setHunger(float nH) { hunger = nH; }
		 
	void grow() { growing = true; }
	bool isGrowing() { return growing; }

	bool loadNN(std::string filePath);

	/*void changeMapSize(int x, int y) { xSize = x; ySize = y; }
	void changeTileSize(int s) { tileSize = s; }*/

	sf::Vector2i getHead() { return HEAD; }
	int getLength() { return length; }
	bool isOccupied(sf::Vector2i pos);
	bool isDead() { return dead; }

	bool isIA() { return IA; }
	void setIA(bool nIA) { IA = nIA; }

	VisualisationReseauNeuronal* getBrain() { return brain; }

	bool checkIfDeath(sf::Vector2i pos);

	//bool isBest = false;

private:
	sf::Vector2i direction = NORTH;
	std::vector<sf::Vector2i> bodyParts;

	bool growing = false;
	bool dead = false;

	int length = 4;

	VisualisationReseauNeuronal* brain;
	bool IA = false;

	float fitness = 0;
	float fitnessMax = 0;
	int timeAlive = 0;

	float hunger = 0;
};