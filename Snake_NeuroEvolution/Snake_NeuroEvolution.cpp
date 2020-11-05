// Snake_NeuroEvolution.cpp�: d�finit le point d'entr�e pour l'application console.
//

#include "stdafx.h"
#include "Snake_Game.h"

int ticketIndividu(int nbIndividus, int i) {
	return  int(nbIndividus / pow(i + 1, 1 / 1.5));
}


using namespace std;
vector<Snake*> listFusion(vector<Snake*> list1, vector<Snake*> list2) {
	vector<Snake*> nList;

	int i1 = 0;
	int i2 = 0;

	int size1 = list1.size();
	int size2 = list2.size();

	while (i1 < size1 && i2 < size2) {
		if (list1[i1]->getFitness() >= list2[i2]->getFitness()) {
			nList.push_back(list1[i1]);
			i1++;
		}
		else {
			nList.push_back(list2[i2]);
			i2++;
		}
	}

	if (i2 == size2) {
		for (i1; i1 < list1.size(); i1++)
			nList.push_back(list1[i1]);
	}
	else {
		for (i2; i2 < list2.size(); i2++)
			nList.push_back(list2[i2]);
	}

	return nList;
}

vector<Snake*> fusionSort(vector<Snake*> list) {
	if (list.size() < 2) {
		return list;
	}
	else {
		int cut = int(list.size() / 2);
		vector<Snake*> list1 = list;
		list1.erase(list1.begin() + cut, list1.end());

		vector<Snake*> list2 = list;
		list2.erase(list2.begin(), list2.begin() + cut);

		return listFusion(fusionSort(list1), fusionSort(list2));
	}
}

const int populationSize = 200;
const float childProp = 0.9; 
const float newProp = 0.15;
const float mutationRatio = 0.2;
const int nbGamesEachSnake = 1;

bool training = false;
int main()
{
	Snake_Game sg = Snake_Game();

	if (!training) {
		while (sg.tick()) {
			sg.show();
			if (sg.getSnake()->isIA())
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
					usleep(300 * 1000);
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
					usleep(10 * 1000);
				else
					usleep(3 * 1000);
			else {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
					usleep(300 * 1000);
				else 
					usleep(80 * 1000);
			}
		}
	}
	else {

		sg.training = true;
		//arma_rng::set_seed_random();

		/*Snake *bestYet = nullptr;
		float bestYetReinsertions = 0;
		*/

		cout << "20x20" << endl;
		cout << "------------------------------" << endl;

		vector<int> structure = {NBINPUTS, 6, 3};

		vector<Snake*> population;
		for (int i = 0; i < populationSize; i++)
			population.push_back(new Snake(structure));

		int generation = 0;

		std::minstd_rand generator = std::minstd_rand(std::chrono::system_clock::now().time_since_epoch().count());

		bool fitnessType2 = false;

		while (sg.isOpen()) {

			//bool fitnessType2 = false;

			for (int j = 0; j < nbGamesEachSnake; j++) {
				sg.resetApples();

				for (int i = 0; i < populationSize; i++) {
					float fitness = 1;

					if ((i == 0 && j == 0) || false) {
						//cout << "---------------------" << endl;
						sg.changeSnake(population[i]);
						sg.reset();
						float frameCount = 0;

						while (sg.tick() /*&& frameCount < xSize * ySize * 2 /*&& population[i]->getFitness() >= -10*/) {

							frameCount++;
							//sg.tick();
							if (population[i]->isGrowing()) {
								frameCount = 0;
							}
							sg.show();
							usleep(3 * 1000);
						}

						//fitness -= float(abs(sg.getApple().x - population[i]->getHead().x) + abs(sg.getApple().y - population[i]->getHead().y)) / ((xSize + ySize));
						/*fitness += frameCount/(xSize * ySize);*/
					}
					else {
						fitness = sg.fullTick(population[i]);
						//population[i]->addFitness(fitness);
					}

					if(i % 10 == 0)
						cout << i << "/" << populationSize << endl;
					//population[i]->addFitness(population[i]->getHunger() / 2);

					population[i]->calculateFitness();



					//population[i]->mFitness(fitness);

					//cout << "Generation : " << generation << ";Individu : " << i << endl;
				}

				int maxHunger = 50;
				for (int i = 0; i < populationSize; i++) {
					if (population[i]->getHunger() > maxHunger)
						maxHunger = population[i]->getHunger();
				}

				for (int i = 0; i < populationSize; i++) {
					float prop = 1 - 0.5*population[i]->getHunger() / maxHunger;
					population[i]->setFitness(population[i]->getFitness() * prop);
				}

				/*for (int i = 0; i < populationSize * childProp /2; i++) {
					if (population[i]->getHunger() > 0)
						fitnessType3 = false;
				}*/

				//fitnessType3 = !(population[0]->getHunger() > 0);

				//fitnessType3 = false;

				/*if (generation != 0) {
					if (fitnessType2) {
						fitnessType2 = false;
						for (int i = 0; i < 3; i++) {
							if (population[i]->getHunger() > 0)
								fitnessType2 = true;
						}
					}
					else {
						fitnessType2 = true;
						for (int i = 0; i < 3; i++) {
							if (population[i]->getHunger() <= 0)
								fitnessType2 = false;
						}
					}
				}

				if (!fitnessType2) {
					for (Snake* s : population)
						s->fitnessCalculationType3();
					cout << "Type 3" << endl;
				}
				else {
					for (Snake* s : population)
						s->fitnessCalculationType2();
					cout << "Type 2" << endl;
				}*/
				/*else {
					for (Snake* s : population)
						s->fitnessCalculationType1();
					cout << "Type 1" << endl;
				}*/

			}	

			//cout << population[0]->getFitness() << endl;

			population = fusionSort(population);

			cout << "Generation : " << generation << "; Max fitness : " << population[0]->getFitness() << "; Individu Max : " << population[0] << endl;
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
			population[0]->getBrain()->save("Data/NN_Data_" + to_string(generation) + "_" + to_string(population[0]->getFitnessMax()));
			//cout << "Saved" << endl;
			//}
  
			//if (!bestYet || population[0]->getFitnessMax() > bestYet->getFitnessMax()) {
			/*if(!bestYet || population[0]->getFitnessMax() > bestYet->getFitnessMax() || bestYetReinsertions >= 5){
				if (bestYet)
					delete bestYet;
				bestYet = new Snake(*population[0]);
				bestYet->setFitnessMax(population[0]->getFitnessMax());
				bestYetReinsertions = 0;
				population[0]->isBest = true;
			}*/

			/*float avg = 0;
			for (int i = 0; i < populationSize; i++)
				avg += population[i]->getFitness();
			avg = avg / populationSize;

			float ecartType = 0;
			for (int i = 0; i < populationSize; i++)
				ecartType += pow(population[i]->getFitness() - avg, 2);
			ecartType = ecartType / populationSize;
			ecartType = pow(ecartType, 0.5);
			cout << "Ecart type : " << ecartType << endl;
			*/


			float ticketsNb = 0;
			for (int i = 0; i < populationSize * (1-childProp); i++) {
			//for (int i = 0; i < populationSize / 4; i++) {
					//population[i]->setFitness(pow(population[i]->getFitness(), 2));
				//ticketsNb += population[i]->getFitness();
				ticketsNb += ticketIndividu(populationSize / 4, i);
				//ticketsNb += ticketIndividu(populationSize * (1 - childProp), i);
			}
			uniform_int_distribution<int> distribution = uniform_int_distribution<int>(0, ticketsNb);

			vector<Snake*> childrenList;

			int temp = 0;
			for (int i = 0; i < populationSize * childProp; i++) {
				int indexParent1 = -1;
				float ticketParent1 = distribution(generator);

				while (ticketParent1 >= 0) {
					indexParent1++;
					if (indexParent1 >= population.size())
						indexParent1 = population.size() - 1;

					//ticketParent1 -= population[indexParent1]->getFitness();
					ticketParent1 -= ticketIndividu(populationSize / 4, indexParent1);
					//ticketParent1 -= ticketIndividu(populationSize * (1 - childProp), indexParent1);
				}

				int indexParent2 = indexParent1;
				while (indexParent2 == indexParent1) {
					indexParent2 = -1;
					float ticketParent2 = distribution(generator);

					while (ticketParent2 >= 0) {
						indexParent2++;
						if (indexParent2 >= population.size())
							indexParent2 = population.size() - 1;

						//ticketParent2 -= population[indexParent2]->getFitness();
						//ticketParent2 -= ticketIndividu(populationSize * (1 - childProp), indexParent2);
						ticketParent2 -= ticketIndividu(populationSize / 4, indexParent2);
					}
				}

				childrenList.push_back(new Snake(population[0], population[1]));

				//temp += indexParent1 + indexParent2;
			}

			//cout << temp / (populationSize * childProp* 2) << endl;

			int indexFirstChild = populationSize - childrenList.size();
			for (int i = 0; i < childrenList.size(); i++) {
				delete population[i + indexFirstChild];
				population[i + indexFirstChild] = childrenList[i];
			}

			int indexFirstNew = populationSize * (1 - newProp);

			for (int i = indexFirstNew; i < populationSize - 1; i++) {
				delete population[i];
				population[i] = new Snake(structure);
			}

			/*population[0]->getBrain()->print();
			population[populationSize - 1]->getBrain()->print();*/

			/*delete population[populationSize - 1];
			population[populationSize - 1] = new Snake(*population[0]);*/


			for (int i = 0; i < populationSize; i++) {
				if(i >= populationSize * (1 - childProp))
					population[i]->getBrain()->mutation();
				population[i]->resetFitness();
			}

			/*bool isBestHere = false;
			for (int i = 0; i < populationSize * (1 - childProp); i++) {
				if (population[i]->isBest) {
					isBestHere = true;
					break;
				}
			}

			if (!isBestHere) {
				population[populationSize - 1] = new Snake(*bestYet);
				population[populationSize - 1]->isBest = true;
				bestYetReinsertions++;
			}*/

			//population[0]->getBrain()->print();
			//population[populationSize - 1]->getBrain()->print();

			generation++;
		}
	}

	system("pause");

    return 0;
}

