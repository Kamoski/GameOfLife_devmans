#include "Animal.h"

Animal::Animal(GENDER _gender)
	:
	m_gender(_gender)
	, m_move(ANIMAL_MOVE::NONE)
{
	
}

void Animal::setAnimalMove(ANIMAL_MOVE _move)
{
	m_move = _move;
}
ANIMAL_MOVE Animal::getAnimalMove()
{
	return m_move;
}

GENDER Animal::getGender()
{
	return m_gender;
}