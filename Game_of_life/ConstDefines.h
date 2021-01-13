#ifndef _DEFINES_
#define _DEFINES_

// Seperate file with various includes, so code is readable better

constexpr const char* COMMANDS_START{ "start" };
constexpr const char* COMMANDS_GENERATE{ "generate" };
constexpr const char* COMMANDS_SET{ "set" };

constexpr const char* COMMANDS_STOP{ "stop" };
constexpr const char* COMMANDS_ADD{ "add" };
constexpr const char* COMMANDS_RESUME{ "resume" };

enum class INPUT_ACTION
{
	START, GENERATE, SET, INVALID_SET, STOP,
		RESUME, ADD, INVALID_ADD, 

	UNKNOWN
};

enum class ANIMAL_MOVE
{
	TOP, TOP_RIGHT, RIGHT, BOTTOM_RIGHT,
	BOTTOM, BOTTOM_LEFT, LEFT, LEFT_TOP,
	NONE
};

enum class GENDER
{
	MALE, FEMALE
};

enum class TYPE
{
	PREDATOR, PREY
};

#endif // !_DEFINES_
