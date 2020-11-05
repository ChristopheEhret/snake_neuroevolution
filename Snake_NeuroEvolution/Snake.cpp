#include "stdafx.h"
#include "Snake.h"


Snake::Snake(sf::Vector2i iPos, int l, sf::Vector2i dir) {
	direction = dir;
	length = l;

	initialize(iPos);

	brain = new VisualisationReseauNeuronal();

	IA = false;
}

Snake::Snake(std::vector<int> struc, sf::Vector2i iPos, int l, sf::Vector2i dir) {
	direction = dir;
	length = l;

	initialize(iPos);

	//brain = new VisualisationReseauNeuronal(ReseauNeuronal(struc));
	brain = new VisualisationReseauNeuronal(struc);
	if (brain)
		IA = true;
	///POTENTIELLEMENT A TESTER MEME SI CA PARAIT PAS UTILE
	//if(brain)
	//	brain->setActivationFuction(ReseauNeuronal::SOFTMAX, struc.size() - 2);
}

Snake::Snake(Snake *parent1, Snake *parent2, sf::Vector2i iPos, int l, sf::Vector2i dir) {
	direction = dir;
	length = l;

	initialize(iPos);

	ReseauNeuronal* temp = ReseauNeuronal::reproduction(parent1->brain, parent2->brain);
	brain = new VisualisationReseauNeuronal(*temp);
	delete temp;

	//brain = ReseauNeuronal::reproduction(parent1->brain, parent2->brain);


	if (brain)
		IA = true;
}

Snake::Snake(const Snake &s) {
	brain = new VisualisationReseauNeuronal(*s.brain);
	IA = s.IA;
}

Snake::~Snake() {
	delete brain;
}

void Snake::initialize(sf::Vector2i iPos) {
	addBodyPart(iPos);
	for (int i = 1; i < length; i++)
		bodyParts.push_back(bodyParts[i - 1] - direction);
}

bool Snake::tick() {
	if (!dead && !IA) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
			go(NORTH);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			go(EAST);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
			go(SOUTH);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			go(WEST);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
			grow();
	}

	if (!dead) {
		timeAlive++;

		sf::Vector2i add = HEAD + direction;
		/*		if (add.x < 0 || add.x >= xSize || add.y < 0 || add.y >= ySize)
					dead = true;

				addBodyPart(add);

				for (int i = 1; i < length; i++)
					if (bodyParts[0] == bodyParts[i])
						dead = true;*/

		if (checkIfDeath(add))
			dead = true;

		if (!growing /*|| growing*/)
			bodyParts.pop_back();
		else {
			length += 1;
			hunger += defaultHunger;
			if (hunger > xSize * ySize / 3)
				hunger = xSize * ySize / 3;
		}

		addBodyPart(add);

		growing = false;

		hunger--;

		if (hunger <= 0) {
			dead = true;
		}
	}

	/*if (fitness > fitnessMax)
		fitnessMax = fitness;*/

	return dead;
}

bool Snake::checkIfDeath(sf::Vector2i pos) {
	if (pos.x < 0 || pos.x >= xSize || pos.y < 0 || pos.y >= ySize) {
		return true;
	}

	for (int i = 1; i < length - 1; i++) {
		if (pos == bodyParts[i]) {
			return true;
		}
	}

	if (growing && pos == bodyParts[length - 1])
		return true;

	return false;
}

void Snake::show(sf::RenderWindow *w) {
	for (int i = 0; i < length; i++) {
		sf::RectangleShape rec(sf::Vector2f(tileSize, tileSize));
		rec.setPosition(sf::Vector2f(bodyParts[i].x * tileSize, bodyParts[i].y * tileSize));
		rec.setFillColor(sf::Color::Green);

		w->draw(rec);
	}
}

void Snake::think(fvec in) {

	if (!IA)
		return;

	const int inputNb = 2 + in.n_elem;
	//const int inputNb = 6;


	//const int inputNb = xSize * ySize;
	arma::fvec inputs(inputNb);
	inputs.zeros();
	for (int i = 0; i < in.n_elem; i++) {
		inputs(i) = in(i);
	}

	/*for (int i = 0; i < in.n_elem; i++) {
		inputs(i + xSize*ySize) = in(i);
	}*/

	//inputs(inputNb - 2) = HEAD.x / xSize;
	//inputs(inputNb - 1) = HEAD.y / ySize;

	/*inputs(HEAD.x + HEAD.y * xSize) = 1;
	inputs(in(0) + in(1) * xSize) = -1;
	for (int i = 1; i < length; i++) {
		inputs(bodyParts[i].x + bodyParts[i].y * xSize) = 0.5;
	}*/

	///PEUT ETRE METTRE LA PROPORTION D'UN TILE EN ENTREE (1/xSize && 1/ySize) OU ENLEVER 1 A CHAQUE DISTANCE => POUVOIR UTILISER L'IA SUR N'IMPORTE QUELLE TAILLE DE TERRAIN
	///PEUT ETRE METTRE LA POSITION DE LA TETE EN ENTREE

	/*float nBodyDistance = HEAD.y;
	float sBodyDistance = ySize - HEAD.y - 1;
	float eBodyDistance = xSize - HEAD.x - 1;
	float wBodyDistance = HEAD.x;

	for (int i = 1; i < length; i++) {
		if (bodyParts[i].x == HEAD.x) {
			if (bodyParts[i].y < HEAD.y) {
				if(HEAD.y - bodyParts[i].y < nBodyDistance)
					nBodyDistance = HEAD.y - bodyParts[i].y;
			}
			else {
				if (bodyParts[i].y == HEAD.y)
					cout << "PAS NORMAl";
				if (bodyParts[i].y - HEAD.y < sBodyDistance)
					sBodyDistance = bodyParts[i].y - HEAD.y;
			}
		}
		else if (bodyParts[i].y == HEAD.y) {
			if (bodyParts[i].x < HEAD.x) {
				if (HEAD.x - bodyParts[i].x < wBodyDistance)
					wBodyDistance = HEAD.x - bodyParts[i].x;
			}
			else {
				if (bodyParts[i].x == HEAD.x)
					cout << "PAS NORMAl";
				if (bodyParts[i].x - HEAD.x < eBodyDistance)
					eBodyDistance = bodyParts[i].x - HEAD.x;
			}
		}
	}
	nBodyDistance /= ySize;
	sBodyDistance /= ySize;
	eBodyDistance /= xSize;
	wBodyDistance /= xSize;

	inputs(in.n_elem) = nBodyDistance;
	inputs(in.n_elem + 1) = sBodyDistance;
	inputs(in.n_elem + 2) = eBodyDistance;
	inputs(in.n_elem + 3) = wBodyDistance;*/
	/*	inputs(in.n_elem + 4) = HEAD.x / xSize;
		inputs(in.n_elem + 5) = HEAD.y / ySize;*/
	inputs(in.n_elem) = float(HEAD.x) / xSize;
	inputs(in.n_elem + 1) = float(HEAD.y) / ySize;

	for (int i = 0; i < in.n_elem; i++) {
		inputs(i) = inputs(i) * 2 - 1;
	}


	arma::fvec output = brain->feedforward(inputs);

	///A TESTER SANS SOFTMAX
	/*if (s(0) > 0.5)
		go(NORTH);
	else if (s(1) > 0.5)
		go(EAST);
	else if (s(2) > 0.5)
		go(SOUTH);
	else if (s(3) > 0.5)
		go(WEST);*/

	int iMax = 0;
	for (int i = 0; i < output.n_elem; i++)
		if (output(i) > output(iMax))
			iMax = i;

	switch (iMax) {
	case 0:
		go(NORTH);
		break;
	case 1:
		go(EAST);
		break;
	case 2:
		go(SOUTH);
		break;
	case 3:
		go(WEST);
		break;
	}
}

void Snake::think(sf::Vector2i apple) {

	if (!IA)
		return;

	fvec input = inputPreprocessing(apple);

	arma::fvec output = brain->feedforward(input);

	int iMax = 0;
	for (int i = 0; i < output.n_elem; i++)
		if (output(i) > output(iMax))
			iMax = i;

	switch (iMax) {
	case 1:
		go(sf::Vector2i(-direction.y, direction.x));
		break;
	case 2:
		go(sf::Vector2i(direction.y, -direction.x));
		break;
	}
}

bool Snake::backPropagation(fvec input) {
	sf::Vector2i inputDir;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		inputDir = NORTH;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		inputDir = SOUTH;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		inputDir = EAST;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		inputDir = WEST;
	}
	else
		return false;

	int front = inputDir.x * direction.x + inputDir.y * direction.y;
	int right = inputDir.y * direction.x - inputDir.x * direction.y;

	fvec output(3);
	output.zeros();

	if (front == 1)
		output(0) = 1;
	else if (right == 1)
		output(1) = 1;
	else if (right == -1)
		output(2) = 1;
	else
		return false;

	//brain->save("NN_Data_Backup");
	brain->backPropagation(input, output);
	//brain->save("NN_Data");

	return true;
}

void Snake::addBodyPart(sf::Vector2i bodyPart) {
	if (bodyPart.x < 0)
		bodyPart.x = xSize - 1;
	else if (bodyPart.x >= xSize)
		bodyPart.x = 0;

	if (bodyPart.y < 0)
		bodyPart.y = ySize - 1;
	else if (bodyPart.y >= ySize)
		bodyPart.y = 0;

	bodyParts.insert(bodyParts.begin(), bodyPart);
}

void Snake::go(sf::Vector2i dir) {
	if ((direction == NORTH || direction == SOUTH) && (dir == EAST || dir == WEST))
		direction = dir;
	else if ((direction == WEST || direction == EAST) && (dir == NORTH || dir == SOUTH))
		direction = dir;
}

bool Snake::loadNN(std::string filePath) {
	if (IA && brain) {
		delete brain;
		brain = nullptr;
		IA = false;
		std::cout << "IA desactivee" << std::endl;
	}
	else {
		IA = brain->load(filePath);

		if (!IA)
			std::cout << "Erreur lors du chargement de l'IA" << std::endl;
		else
			std::cout << "IA activee" << std::endl;
	}

	return IA;
}

void Snake::reset(sf::Vector2i iPos, int l, sf::Vector2i dir) {
	bodyParts.clear();
	length = l;
	direction = dir;

	growing = false;
	dead = false;
	timeAlive = 0;

	hunger = defaultHunger;

	initialize(iPos);
}

bool Snake::isOccupied(sf::Vector2i pos) {
	for (sf::Vector2i bp : bodyParts) {
		if (bp == pos)
			return true;
	}

	return false;
}

void Snake::calculateFitness() {
	///A AJUSTER
	fitness = pow(fitness, 3);
	fitness += pow(length - 4, 3);
	/*if (hunger > 0)
		fitness *= 0.7;*/
	//fitness -= pow(hunger / 40, 3);	
}

void Snake::fitnessCalculationType1() {
	fitness = 0;
	fitness += timeAlive;
	fitness *= pow(3, (length - 4));
}

void Snake::fitnessCalculationType2() {
	fitness = 0;
	//fitness += timeAlive;
	fitness += pow((length - 4), 0.83);
	//fitness -= pow(hunger/100, 1);
	fitness -= hunger / 100;
	/*if (hunger > 0)
		fitness *= 0.7;*/
}

///A ADAPTER
void Snake::fitnessCalculationType3() {
	fitness = pow(fitness, 3);
	fitness += pow(length - 4, 3);
}

fvec Snake::inputPreprocessing(sf::Vector2i apple) {
	arma::fvec input(NBINPUTS);
	input.zeros();

	sf::Vector2i deltaApple = apple - HEAD;

	sf::Vector2i dirRight(-direction.y, direction.x);

	float deltaFront = deltaApple.x * direction.x + deltaApple.y * direction.y;
	float deltaRight = deltaApple.x * dirRight.x + deltaApple.y * dirRight.y;

	///A FAIRE : INVERSER deltaFront ET deltaRight pour qu'il soit plus importants si la pomme est proche
	///ATTENTION : DIVISION PAS CORRECTE SI xSize != ySize

	/*if (deltaFront > 0)
	input(0) = deltaFront / xSize;
	else
	input(1) = -deltaFront / xSize;

	if (deltaRight > 0)
	input(2) = deltaRight / ySize;
	else
	input(3) = -deltaRight / ySize;*/

	sf::Vector2i front = direction;
	sf::Vector2i right = sf::Vector2i(-direction.y, direction.x);
	sf::Vector2i left = sf::Vector2i(direction.y, -direction.x);

	sf::Vector2i checks[3] = { front, right, left };
	float dists[3] = { xSize, ySize, ySize };
	for (int i = 0; i < 3; i++) {
		sf::Vector2i posCheck = HEAD + checks[i];
		float dist = 0;
		/*while (!checkIfDeath(posCheck)) {
		dist += 1;
		posCheck += checks[i];
		}*/

		bool body = false;
		while (posCheck.x >= 0 && posCheck.x < xSize && posCheck.y >= 0 && posCheck.y < ySize && !body) {
			for (int i = 0; i < length; i++) {
				if (posCheck == bodyParts[i])
					body = true;
			}

			if (!body)
				dist++;
			posCheck += checks[i];

		}

		dists[i] = dist;

		if (checks[i].x == 0)
			dist /= xSize;
		else
			dist /= ySize;

		dist = 1 - dist * 2;

		if (body)
			input(4 + i) = dist;
		else
			input(7 + i) = dist;

		if (abs(dist) > 1)
			cout << "pb" << endl;
	}

	/*	if (deltaFront > 0) {
	if (deltaFront < dists[0])
	input(0) = 1;
	else
	input(0) = 0.5;
	}
	else if (deltaFront < 0) {
	input(1) = 0.5;
	//input(1) = 1;
	}


	if (deltaRight > 0) {
	if (deltaRight < dists[1])
	input(2) = 1;
	else
	input(2) = 0.5;
	}
	else if (deltaRight < 0) {
	if (-deltaRight < dists[2])
	input(3) = 1;
	else
	input(3) = 0.5;
	}**/

	if (deltaFront > 0)
		input(0) = 1;
	else if (deltaFront < 0)
		input(1) = 1;


	if (deltaRight > 0)
		input(2) = 1;
	else if (deltaRight < 0)
		input(3) = 1;

	return input;
}