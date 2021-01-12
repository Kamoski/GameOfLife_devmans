#include "Predator.h"

Predator::Predator(GENDER _gender)
	:
	Animal(_gender)
	, m_timeSinceMeal(2)
	, hadMeal(false)
{

}

void Predator::setTimeSinceMeal(unsigned int _timeSinceMeal)
{
	m_timeSinceMeal = _timeSinceMeal;
}
unsigned int Predator::getTimeSinceMeal()
{
	return m_timeSinceMeal;
}

void Predator::eat()
{
	hadMeal = true;
}

void Predator::starve()
{
	hadMeal = false;
}
bool Predator::getMealStatus()
{
	return hadMeal;
}