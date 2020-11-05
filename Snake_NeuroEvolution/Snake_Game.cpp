#include "stdafx.h"
#include "Snake_Game.h"

Snake_Game::Snake_Game() {
	srand(time(NULL));

	window = new sf::RenderWindow(sf::VideoMode(xSize * tileSize, ySize * tileSize), "OUI !");

	snake = new Snake();

	newApple();

	if (!font.loadFromFile("police.ttf"))
		std::cout << "Erreur chargement police" << std::endl;

	affScore.setFont(font);
	affScore.setString(std::to_string(4));
	affScore.setFillColor(sf::Color(127, 127, 127));
	affScore.setCharacterSize(24);
	affScore.setPosition(0, 0);
}

void Snake_Game::show() {
	window->clear(sf::Color::White);

	snake->show(window);

	sf::RectangleShape a(sf::Vector2f(tileSize, tileSize));
	a.setPosition(sf::Vector2f(apple.x * tileSize, apple.y * tileSize));
	a.setFillColor(sf::Color::Red);
	window->draw(a);

	if (!training) {
		affScore.setString(std::to_string(snake->getLength()));
		window->draw(affScore);
	}


	window->display();
}

bool Snake_Game::tick() {
	sf::Event event;
	while (window->pollEvent(event))
	{
		//Fermeture de la fenêtre lorsque l'utilisateur le souhaite
		if (event.type == sf::Event::Closed)
			window->close();
	}

	//float deltaX = float(snake->getHead().x - apple.x) / xSize;
	//float deltaY = float(snake->getHead().y - apple.y) / ySize;
	

	float deltaX = apple.x / xSize;
	float deltaY = apple.y / ySize;

	arma::fvec e = {deltaX, deltaY};
	//snake->think(e);

//	cout << apple.x << ";" << apple.y << endl;


	if (!training && snake->isIA() && sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
		fvec input = snake->inputPreprocessing(apple);
		snake->getBrain()->setLearningRates({ 0.2f, 0.2f });

		while (sf::Keyboard::isKeyPressed(sf::Keyboard::T));

		while (!sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !snake->backPropagation(input));

		while (sf::Keyboard::isKeyPressed(sf::Keyboard::T));

	//	cout << "oui" << endl;
	}

	snake->think(apple);


	if (training) {
		sf::Vector2i deltaApple = apple - snake->getHead();
		float distApple = abs(deltaApple.x) + abs(deltaApple.y);
	
		snake->tick();

		sf::Vector2i ndeltaApple = apple - snake->getHead();
		float ndistApple = abs(ndeltaApple.x) + abs(ndeltaApple.y);

		if (ndistApple < distApple)
			snake->addFitness(1);
		else
			snake->addFitness(-1.5);
	}
	else {

		snake->setHunger(3);
		snake->tick();

		if (snake->isIA()) {
			snake->getBrain()->show();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
			if (!snake->isIA()) {
				if (snake->getBrain()->load("NN_Data")) {
					snake->setIA(true);
					cout << "IA activee" << endl;
					snake->getBrain()->open();
				}
			}
			else {
				snake->setIA(false);
				cout << "IA desactivee" << endl;
				snake->getBrain()->close();
			}

			while (sf::Keyboard::isKeyPressed(sf::Keyboard::I));
		}

	}

	if (snake->getHead() == apple) {
		newApple();
		snake->grow();
		snake->setFitness(0.f);
		/*if (training)
			snake->addFitness(30);*/
	}

	return window->isOpen() && !snake->isDead();
}

float Snake_Game::fullTick(Snake *s, int maxFrames) {
	float frameCount = 0;
	float currFitness = 0;
	
	changeSnake(s);
	reset();

	while (!s->isDead() /*&& frameCount < maxFrames/* && currFitness >= -5*/) {
		//float deltaX = float(snake->getHead().x - apple.x) / xSize;
		//float deltaY = float(snake->getHead().y - apple.y) / ySize;
		

		float deltaX = apple.x / xSize;
		float deltaY = apple.y / ySize;

		arma::fvec in = { deltaX, deltaY };
		
		s->think(apple);
//		s->think(in);

		sf::Vector2i deltaApple = apple - s->getHead();
		float distApple = abs(deltaApple.x) + abs(deltaApple.y);
		
		s->tick();


		sf::Vector2i ndeltaApple = apple - s->getHead();
		float ndistApple = abs(ndeltaApple.x) + abs(ndeltaApple.y);

		if (ndistApple < distApple) {
			currFitness += 1;
			s->addFitness(1);
		}
		else {
			currFitness -= 1;
			s->addFitness(-1.5);
		}

		frameCount++;

		if (s->getHead() == apple) {
			newApple();
			s->grow();
			frameCount = 0;
			s->setFitness(0.f);
			//currFitness += 30;
			//s->addFitness(30);
		}
	}

	//currFitness -= float(abs(apple.x - s->getHead().x) + abs(apple.y - s->getHead().y)) / ((xSize + ySize));
	/*currFitness += frameCount/maxFrames;*/

	return currFitness;
}

void Snake_Game::newApple() {
	if (indexApple >= apples.size() || !training) {
		int x = 0;
		int y = 0;
		do {
			x = rand() % xSize;
			y = rand() % ySize;
		} while (snake->isOccupied(sf::Vector2i(x, y)));

		apple = sf::Vector2i(x, y);
		if (training)
			apples.push_back(apple);
	}
	else {
		apple = apples[indexApple];
	}
	indexApple++;
}

void Snake_Game::reset() {
	snake->reset();
	indexApple = 0;
	newApple();
}