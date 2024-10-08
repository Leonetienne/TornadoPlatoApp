#include "Random.h"
#include <array>
#include <algorithm>

using namespace Eule;

// Checks if the random number generator is initialized. Does nothing if it is, initializes if it isn't.
#define MAKE_SURE_RNG_IS_INITIALIZED if (!isRngInitialized) InitRng();

void Random::InitRng()
{
	// Create truly random source (from hardware events)
	std::random_device randomSource;

	// Generate enough truly random values to populate the entire state of the mersenne twister
	std::array<int, std::mt19937::state_size> seedValues;
	std::generate_n(seedValues.data(), seedValues.size(), std::ref(randomSource));
	std::seed_seq seedSequence(seedValues.begin(), seedValues.end());

	// Seed the mersenne twister with these values
	rng = std::mt19937(seedSequence);

	isRngInitialized = true;

	return;
}

// Will return a random double between 0 and 1
double Random::RandomFloat()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return (rng() % 694206942069ll) / 694206942069.0;
}

// Will return a random unsigned integer.
unsigned int Random::RandomUint()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return rng();
}

// Will return a random integer
unsigned int Random::RandomInt()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	// Since this is supposed to return a random value anyways,
	// we can let the random uint overflow without any problems.
	return (int)rng();
}

// Will return a random double within a range  
// These bounds are INCLUSIVE!
double Random::RandomRange(double min, double max)
{
	return (RandomFloat() * (max - min)) + min;
}

// Will return a random integer within a range. This is faster than '(int)RandomRange(x,y)'
// These bounds are INCLUSIVE!
int Random::RandomIntRange(int min, int max)
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return (rng() % (max + 1 - min)) + min;
}

bool Random::RandomChance(const double chance)
{
	return RandomFloat() <= chance;
}

std::mt19937 Random::rng;
bool Random::isRngInitialized = false;
