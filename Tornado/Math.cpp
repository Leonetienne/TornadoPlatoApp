#include "Math.h"
#include "Constants.h"
#include <array>

// Checks if the random number generator is initialized. Does nothing if it is, initializes if it isn't.
#define MAKE_SURE_RNG_IS_INITIALIZED if (!isRngInitialized) InitRng();

void Math::InitRng()
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
double Math::Random()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return (rng() % 6942069) / 6942069.0;
}

// Will return a random unsigned integer.
unsigned int Math::RandomUint()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return rng();
}

// Will return a random integer
unsigned int Math::RandomInt()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	// Since this is supposed to return a random value anyways,
	// we can let the random uint overflow without any problems.
	return (int)rng();
}

// Will return a random double within a range  
// These bounds are INCLUSIVE!
double Math::RandomRange(double min, double max)
{
	return (Random() * (max - min)) + min;
}

// Will return a random integer within a range. This is faster than '(int)RandomRange(x,y)'
// These bounds are INCLUSIVE!
int Math::RandomIntRange(int min, int max)
{
	return (rng() % (max + 1 - min)) + min;
}

double Math::Oscillate(const double a, const double b, const double counter, const double speed)
{
	return (sin(counter * speed * PI - HALF_PI) * 0.5 + 0.5) * (b-a) + a;
}

std::mt19937 Math::rng;
bool Math::isRngInitialized = true;
