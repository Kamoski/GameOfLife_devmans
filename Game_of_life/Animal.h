#ifndef _ANIMAL_
#define _ANIMAL_
#include "ConstDefines.h"

class Animal
{
public:
	Animal(GENDER _gender);

	void setAnimalMove(ANIMAL_MOVE _move);
	ANIMAL_MOVE getAnimalMove();

	GENDER getGender();

protected:
	GENDER m_gender;
	ANIMAL_MOVE m_move;
};


#endif // !_ANIMAL_


