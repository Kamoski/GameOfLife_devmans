#include <iostream>
#include <string>
#include <cctype>
#include <conio.h>

#include "Manager.h"
#include "ConstDefines.h"

INPUT_ACTION parseInput(const std::string& input, std::pair<unsigned int, unsigned int>& xyPair);
void prepareLane(std::string& input);

int main()
{
	Manager gameManager;
	bool isReady = false;
	std::pair<unsigned int, unsigned int> xyPair;
	std::string lane;

	while (!isReady) 
	{
		std::cout << "Welcome in game. Commands are accepted with button \"enter\"."							<< std::endl
			<< "To begin without animals, type \"Start\"."														<< std::endl
			<< "To begin with custom starting amout of animals type \"Set <x> <y>\""  
				<<" where x - number of predators and y - number of preys, x and y must be greater than zero"	<< std::endl
			<< "To begin with random amout of each type, type \"Generate\""										<< std::endl;

#ifdef _DEBUG
		lane = "generate";
#else

		std::getline(std::cin, lane);
		prepareLane(lane);
#endif // _DEBUG



		switch (parseInput(lane, xyPair))
		{
			case INPUT_ACTION::START:
				gameManager.setSartingAnimalsFlag(false);
				break;

			case INPUT_ACTION::GENERATE:
				gameManager.setSartingAnimalsFlag(true);
				break;

			case INPUT_ACTION::SET:
				gameManager.setSartingAnimalsFlag(true);
				gameManager.setPredators(xyPair.first);
				gameManager.setPreys(xyPair.second);
				break;

			case INPUT_ACTION::INVALID_SET:
				std::cout << "Invalid values passed to set command." << std::endl;
				system("pause");
				system("cls");
				continue;

			default:
			case INPUT_ACTION::UNKNOWN:
				std::cout << "Couldn't recognize passed command. Please try again. Make sure that no spelling errors occured." << std::endl;
				system("pause");
				system("cls");
				continue;
		}
		lane = "";

		std::cout << "To begin with random map size type \"Generate\"" << std::endl
			<< "To begin with custom map size type \"SET <x> <y>\" where x - number of rows, y - number of columns" << std::endl;

#ifdef _DEBUG
		lane = "generate";
#else

		std::getline(std::cin, lane);
		prepareLane(lane);
#endif // _DEBUG

		switch (parseInput(lane, xyPair))
		{
		case INPUT_ACTION::GENERATE:
			gameManager.setRandomMapSizeFlag(true);
			break;

		case INPUT_ACTION::SET:
			gameManager.setRandomMapSizeFlag(false);
			gameManager.setMapHeight(xyPair.first);
			gameManager.setMapWidth(xyPair.second);
			break;

		case INPUT_ACTION::INVALID_SET:
			std::cout << "Invalid values passed to set command." << std::endl;
			system("pause");
			system("cls");
			continue;

		default:
		case INPUT_ACTION::UNKNOWN:
			std::cout << "Couldn't recognize passed command. Please try again. Make sure that no spelling errors occured." << std::endl;
			system("pause");
			system("cls");
			continue;
		
		}
		isReady = true;
	}

	std::cout << "After each turn, you can pass those commands:"   << std::endl
		<< "stop - Exits game,"																										<< std::endl
		<< "resume - Resumes game with no change at all, (you can also resume just by pressing enter)"																			<< std::endl
		<< "ADD <x> <y> - adds both x - predators, y - preys scattered randomly across game field*, "								<< std::endl
		<< "			*works only with both parameters, both need to greater then 0 "												<< std::endl << std::endl;


#ifdef _DEBUG
#else

	system("pause");
	system("cls");

#endif // _DEBUG


	gameManager.initGame();

	while (gameManager.isPlaying())
	{
		gameManager.tick();
		std::cout << "-------- Enter command, if any --------" << std::endl;
		getline(std::cin, lane);

		if (lane.length() == 0)
		{
			continue;
		}

		prepareLane(lane);
		switch (parseInput(lane, xyPair))
		{
		case INPUT_ACTION::STOP:
			gameManager.stopGame();
			break;

		case INPUT_ACTION::RESUME:
			break;

		case INPUT_ACTION::ADD:
			gameManager.addAnimalsRandomly(xyPair.first, xyPair.second);
			break;

		default:
		case INPUT_ACTION::UNKNOWN:
			std::cout << "Couldn't understand passed command. Resuming game." << std::endl;
			break;
		}
	}
}

void prepareLane(std::string& input)
{
	// To make sure that upper/lower case doen't matter
	std::transform(input.begin(), input.end(), input.begin(),
		[](unsigned char letter) { return std::tolower(letter); });

	int iter = 0;
	while (input[iter] == ' ')
	{
		iter++;
	}

	// And to make sure that spaces in front doen't matter either
	input = input.substr(iter, input.length());

}

INPUT_ACTION parseInput(const std::string& input, std::pair<unsigned int, unsigned int>& xyPair)
{

	auto pairParseLambda = ([](std::string _input, std::pair<unsigned int, unsigned int>& _xyPair ) -> bool {
		auto first = atoi(_input.substr(4, 1).c_str());
		if (first == 0)
		{
			return false;
		}
		else
		{
			unsigned int iFirstValue = 0, iSecondValue = 0;
			unsigned int numStartingIndex = 4;

			while (_input[static_cast<int64_t>(iFirstValue) + numStartingIndex] != ' ')
			{
				iFirstValue++;
			}
			_xyPair.first = atoi(_input.substr(4, iFirstValue).c_str());

			iFirstValue += 1 + 4;
			if (iFirstValue > _input.length())
			{
				return false;
			}

			while (_input[static_cast<int64_t>(iSecondValue) + iFirstValue] != ' ' && _input.length() != static_cast<int64_t>(iSecondValue) + iFirstValue)
			{
				iSecondValue++;
			}
			_xyPair.second = atoi(_input.substr(iFirstValue, iSecondValue).c_str());

		}

		if (_xyPair.first == 0 || _xyPair.second == 0)
		{
			return false;
		}
		return true;
	});

	if (input.substr(0, 5) == COMMANDS_START)
	{
		return INPUT_ACTION::START;
	}
	else if (input.substr(0, 8) == COMMANDS_GENERATE)
	{
		return INPUT_ACTION::GENERATE;
	}
	else if (input.substr(0, 3) == COMMANDS_SET)
	{
		if (!pairParseLambda(input, xyPair))
		{
			return INPUT_ACTION::INVALID_SET;
		}
		return INPUT_ACTION::SET;
	}
	else if (input.substr(0, 4) == COMMANDS_STOP)
	{
		return INPUT_ACTION::STOP;
	}
	else if (input.substr(0, 6) == COMMANDS_RESUME)
	{
		return INPUT_ACTION::RESUME;
	}
	else if (input.substr(0,3) == COMMANDS_ADD)
	{
		if (!pairParseLambda(input, xyPair))
		{
			return INPUT_ACTION::INVALID_ADD;
		}
		return INPUT_ACTION::ADD;
	}
	else
	{
		return INPUT_ACTION::UNKNOWN;
	}
}