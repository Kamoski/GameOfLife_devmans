#ifndef _PREDATOR_
#define _PREDATOR_
#include "Animal.h"

class Predator : public Animal
{
public:
	Predator(GENDER _gender);

	void setTimeSinceMeal(unsigned int _timeSinceMeal);
	unsigned int getTimeSinceMeal();

	void eat();
	void starve();
	bool getMealStatus();

private:
	unsigned int m_timeSinceMeal;
	bool hadMeal;
};

#endif // !_PREDATOR_


