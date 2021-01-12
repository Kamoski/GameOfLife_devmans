#ifndef _MANAGER_
#define _MANAGER_

#include <iostream>
#include <thread>
#include <random>
#include <ctime>
#include <vector>

#include "Predator.h"
#include "Prey.h"

struct Field
{
	
	void addPredator(Predator* predator)
	{
		predators.push_back(predator);
	}

	void addPrey(Prey* prey)
	{
		preys.push_back(prey);
	}

	bool hasPredators() 
	{
		if (predators.size() > 0)
			return true;
		return false;
	}

	bool hasPreys()
	{
		if (preys.size() > 0)
			return true;
		return false;
	}

	void clearDeadAnimals()
	{
		predators.erase(std::remove(predators.begin(), predators.end(), nullptr),
			predators.end());

		preys.erase(std::remove(preys.begin(), preys.end(), nullptr),
			preys.end());
	}

	std::vector<Predator*> predators;
	std::vector<Prey*> preys;

};

class Manager
{
public:
	 Manager();
	 ~Manager();

	 void setPredators(unsigned int _numOfPreds);
	 void setPreys(unsigned int _numOfPreys);

	 void setMapWidth(unsigned int _width);
	 void setMapHeight(unsigned int _height);

	 void setSartingAnimalsFlag(bool _startingAnimals);
	 void setRandomMapSizeFlag(bool _randomMapFlag);

	 bool isPlaying();
	 void stopGame();

	 void addAnimalsRandomly(unsigned int _numOfPreds, unsigned int _numOfPreys);

	 void initGame();
	 void tick();
	 void calculateActions();

private:

	 void clearTurnData();
	 void displayInfo();

	 void calculateAnimalsMoves();
	 bool moveAnimals();

	 template <class AnimalType>
	 bool determineIfMoveIsValid(unsigned int i, unsigned int j, ANIMAL_MOVE _move, AnimalType* _animal);

	 void create_gameField();
	 void fill_gameField();

	 void addAnimalsOnCoords(unsigned int _preds, unsigned int _preys, unsigned int _i, unsigned int _j);

	 unsigned int m_preds;
	 unsigned int m_preys;

	 unsigned int m_width;
	 unsigned int m_height;

	 bool m_randomStartingAnimals;
	 bool m_randomMapGeneration;

	 bool m_isPlaying;

	 Field** m_gameField;
	 bool debug = false;

	 unsigned int eaten{0};
	 unsigned int diedOfHunger{0};

	 unsigned int bornPreys{0};
	 unsigned int bornPreds{0};


};

#endif // !_MANAGER_


