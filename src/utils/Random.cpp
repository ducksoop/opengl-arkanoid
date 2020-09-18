#include "Random.h"

#include <cstdlib>

bool Random::Chance(int chance)
{
	return (rand() % chance) == 0;
}
