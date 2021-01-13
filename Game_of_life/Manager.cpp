#include "Manager.h"

Manager::Manager()
	:
	m_height(0)
	, m_preds(0)
	, m_preys(0)
	, m_randomMapGeneration(0)
	, m_randomStartingAnimals(0)
	, m_width(0)
	, m_isPlaying(true)
{
	m_gameField = nullptr;
}
Manager::~Manager()
{
	if (m_gameField != nullptr)
	{
		for (unsigned int i = 0; i < m_width; ++i)
		{
			if(m_gameField[i] != nullptr)
				delete[]m_gameField[i];
		}
		delete[]m_gameField;
	}

}

void Manager::setPredators(unsigned int _numOfPreds)
{
	m_preds = _numOfPreds;
}
void Manager::setPreys(unsigned int _numOfPreys)
{
	m_preys = _numOfPreys;
}

void Manager::setMapWidth(unsigned int _width)
{
	m_width = _width;
}

void Manager::setMapHeight(unsigned int _height)
{
	m_height = _height;
}


void Manager::setSartingAnimalsFlag(bool _startingAnimals)
{
	m_randomStartingAnimals = _startingAnimals;
}

void Manager::setRandomMapSizeFlag(bool _randomMapFlag)
{
	m_randomMapGeneration = _randomMapFlag;
}

void Manager::initGame()
{
	create_gameField();
	fill_gameField();
}

void Manager::create_gameField()
{
	if (m_randomMapGeneration)
	{
		std::default_random_engine engine;
		engine.seed(std::chrono::system_clock::now().time_since_epoch().count() % 15);
		std::uniform_int_distribution<int> int_dist(50, 50);
		
		m_width = int_dist(engine);
		m_height = int_dist(engine);
	}

	m_gameField = new Field* [m_width];
	for (unsigned int i = 0; i < m_width; ++i)
	{
		m_gameField[i] = new Field[m_height];
	}

}

void Manager::fill_gameField()
{
	std::default_random_engine engine;
	engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

	if (m_randomStartingAnimals)
	{
		std::uniform_int_distribution<int> int_dist(200, 300);

		m_preds = int_dist(engine);
		m_preys = int_dist(engine);
	}
	addAnimalsRandomly(m_preds, m_preys);
}

bool Manager::isPlaying()
{
	return m_isPlaying;
}

void Manager::stopGame()
{
	m_isPlaying = false;
}

void Manager::tick()
{
	displayAnimalsQuantity();

	calculateAnimalsMoves();
	moveAnimals();
	calculateActions();

	displayActionsInfo();
	displayAnimalsQuantity();
	
	clearTurnData();
}

void Manager::clearTurnData()
{
	eaten = diedOfHunger = bornPreds = bornPreys = 0;
}

void Manager::displayAnimalsQuantity()
{
	unsigned int preds{ 0 };
	unsigned int preys{ 0 };
	for (size_t i = 0; i < m_width; ++i)
	{
		for (size_t j = 0; j < m_height; ++j)
		{
			preds += m_gameField[i][j].predators.size();
			preys += m_gameField[i][j].preys.size();
		}
	}
	std::printf("Number of preys:%i\nNumber of predators:%i\n",
		preys, preds);
}

void Manager::displayActionsInfo()
{
	unsigned int preds{ 0 };
	unsigned int preys{ 0 };

	std::printf("Eaten preys: %i\nPredators died of hunger:%i\nBorn predators:%i\nBorn preys: %i\n",
		eaten, diedOfHunger, bornPreds, bornPreys);
}

void Manager::calculateAnimalsMoves()
{
	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			determineValidMoves(i, j, m_gameField[i][j].predators);
			determineValidMoves(i, j, m_gameField[i][j].preys);
		}
	}
}

template <class AnimalType>
void Manager::determineValidMoves(unsigned int i, unsigned int j, std::vector<AnimalType*> & _animalsVector)
{
	std::default_random_engine engine;
	engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> moveDist(0, 7);

	for (size_t index = 0; index < _animalsVector.size(); ++index)
	{
		bool moveIsInvalid = true;
		while (moveIsInvalid)
		{
			switch ((ANIMAL_MOVE)moveDist(engine))
			{
			case ANIMAL_MOVE::TOP:
				if (j <= (m_height - 2))
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::TOP);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::TOP_RIGHT:
				if (i <= (m_width - 2) && j <= (m_height - 2))
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::TOP_RIGHT);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::RIGHT:
				if (i <= (m_width - 2))
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::RIGHT);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::BOTTOM_RIGHT:
				if (i <= (m_width - 2) && j != 0)
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::BOTTOM_RIGHT);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::BOTTOM:
				if (j != 0)
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::BOTTOM);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::BOTTOM_LEFT:
				if (i != 0 && j != 0)
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::BOTTOM_LEFT);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::LEFT:
				if (i != 0)
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::LEFT);
					moveIsInvalid = false;
				}
				break;
			case ANIMAL_MOVE::LEFT_TOP:
				if (i != 0 && j <= (m_height - 2))
				{
					_animalsVector[index]->setAnimalMove(ANIMAL_MOVE::LEFT_TOP);
					moveIsInvalid = false;
				}
				break;
			}
		}
	}
}

void Manager::moveAnimals()
{
	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			moveBasedOnType<Predator>(m_gameField[i][j].predators, TYPE::PREDATOR, i, j);
			moveBasedOnType<Prey>(m_gameField[i][j].preys, TYPE::PREY, i, j);
		}
	}
}

template <class AnimalType>
void Manager::moveBasedOnType(std::vector<AnimalType*>& _animalVector, TYPE _animalType, unsigned int _row, unsigned int _col)
{
	for (size_t index = 0; index < _animalVector.size(); ++index)
	{
		ANIMAL_MOVE moveToMake = _animalVector[index]->getAnimalMove();
		_animalVector[index]->setAnimalMove(ANIMAL_MOVE::NONE);
		switch (moveToMake)
		{
		case ANIMAL_MOVE::TOP:
			m_gameField[_row][_col + 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::TOP_RIGHT:
			m_gameField[_row + 1][_col + 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::RIGHT:
			m_gameField[_row + 1][_col].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::BOTTOM_RIGHT:
			m_gameField[_row + 1][_col - 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::BOTTOM:
			m_gameField[_row][_col - 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::BOTTOM_LEFT:
			m_gameField[_row - 1][_col - 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::LEFT:
			m_gameField[_row - 1][_col].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::LEFT_TOP:
			m_gameField[_row - 1][_col + 1].addAnimal(std::move(_animalVector[index]), _animalType);
			break;
		case ANIMAL_MOVE::NONE:
			break;
		}

		if (moveToMake != ANIMAL_MOVE::NONE)
		{
			_animalVector[index] = nullptr;
			_animalVector.erase(_animalVector.begin() + index);
		}
	}
}

void Manager::calculateActions()
{
	unsigned int preysToAdd{ 0 };
	unsigned int predsToAdd{ 0 };
	
	unsigned int malesLookingMate{ 0 };
	unsigned int femalesLooingMate{ 0 };
	// Manage eating, mating, dying

	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			// First preds eat preys
			if(m_gameField[i][j].predators.size() >= m_gameField[i][j].preys.size())
			{
				// Only few predators eat
				for (size_t x = 0; x < m_gameField[i][j].preys.size(); ++x)
				{
					m_gameField[i][j].predators[x]->eat();
					m_gameField[i][j].preys.erase(m_gameField[i][j].preys.begin() + x);
					eaten++;
				}
			}
			else
			{
				// All of them eat
				for (size_t x = 0; x < m_gameField[i][j].predators.size(); ++x)
				{
					m_gameField[i][j].predators[x]->eat();
					m_gameField[i][j].preys.erase(m_gameField[i][j].preys.begin() + x);
					eaten++;
				}
			}

			// Next we check if any predator died of hunger
			// Also we update their meal status
			for (size_t x = 0; x < m_gameField[i][j].predators.size(); ++x)
			{
				if (m_gameField[i][j].predators[x]->getMealStatus())
				{
					m_gameField[i][j].predators[x]->setTimeSinceMeal(2);
					m_gameField[i][j].predators[x]->starve();
				}
				else
				{
					unsigned int turnsSinceMeal = m_gameField[i][j].predators[x]->getTimeSinceMeal();
					if (turnsSinceMeal == 0)
					{
						m_gameField[i][j].predators.erase(m_gameField[i][j].predators.begin() + x);
						diedOfHunger++;
					}
					else
					{
						m_gameField[i][j].predators[x]->setTimeSinceMeal(turnsSinceMeal - 1);
					}
				}
			}

			// Here we check if any mating occured
			// For predators
			for (size_t x = 0; x < m_gameField[i][j].predators.size(); ++x)
			{
				if (m_gameField[i][j].predators[x] == nullptr)
				{
					continue;
				}
				if (m_gameField[i][j].predators[x]->getGender() == GENDER::MALE)
				{
					malesLookingMate++;
				}
				else
				{
					femalesLooingMate++;
				}
			}

			predsToAdd = (malesLookingMate >= femalesLooingMate) ? femalesLooingMate : malesLookingMate;
			malesLookingMate = femalesLooingMate = 0;

			// And for preys
			for (size_t x = 0; x < m_gameField[i][j].preys.size(); ++x)
			{
				if (m_gameField[i][j].preys[x] == nullptr)
				{
					continue;
				}
				if (m_gameField[i][j].preys[x]->getGender() == GENDER::MALE)
				{
					malesLookingMate++;
				}
				else
				{
					femalesLooingMate++;
				}
			}

			preysToAdd = (malesLookingMate >= femalesLooingMate) ? femalesLooingMate : malesLookingMate;
			malesLookingMate = femalesLooingMate = 0;

			// And now breed animals
			addAnimalsOnCoords(predsToAdd, preysToAdd, i, j);
		}
	}
}

void Manager::addAnimalsRandomly(unsigned int _numOfPreds, unsigned int _numOfPreys)
{
	std::default_random_engine engine;
	engine.seed(std::chrono::system_clock::now().time_since_epoch().count());

	std::uniform_int_distribution<int> posX_dist(0, m_width - 1);
	std::uniform_int_distribution<int> posY_dist(0, m_height - 1);
	std::uniform_int_distribution<int> animalGender(0, 1);
	unsigned int posX{ 0 };
	unsigned int posY{ 0 };

	for (int i = 0; i < _numOfPreys; ++i)
	{
		posX = posX_dist(engine);
		posY = posY_dist(engine);

		m_gameField[posX][posY].addAnimal(new Prey(GENDER(animalGender(engine))), TYPE::PREY);
	}

	for (int i = 0; i < _numOfPreds; ++i)
	{
		posX = posX_dist(engine);
		posY = posY_dist(engine);

		m_gameField[posX][posY].addAnimal(new Predator(GENDER(animalGender(engine))),TYPE::PREDATOR);

	}

}

void Manager::addAnimalsOnCoords(unsigned int _preds, unsigned int _preys, unsigned int _i, unsigned int _j)
{
	std::default_random_engine engine;
	engine.seed(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> genderDist(0, 1);

	for (size_t i = 0; i < _preds; ++i)
	{
		m_gameField[_i][_j].addAnimal(new Predator(GENDER(genderDist(engine))), TYPE::PREDATOR);
		bornPreds++;
	}

	for (size_t i = 0; i < _preys; ++i)
	{
		m_gameField[_i][_j].addAnimal(new Prey(GENDER(genderDist(engine))), TYPE::PREY);
		bornPreys++;
	}

}