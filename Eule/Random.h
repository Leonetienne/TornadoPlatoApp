#pragma once
#include <random>

namespace Eule
{
	/** Extensive random number generator
	*/
	class Random
	{
	public:
		//! Will return a random double between `0` and `1`
		static double RandomFloat();

		//! Will return a random unsigned integer.
		static unsigned int RandomUint();

		//! Will return a random integer
		static unsigned int RandomInt();

		//! Will return a random double within a range  
		//! These bounds are INCLUSIVE!
		static double RandomRange(const double min, const double max);

		//! Will return a random integer within a range. This is faster than `(int)RandomRange(x,y)`  
		//! These bounds are INCLUSIVE!
		static int RandomIntRange(const int max, const int min);

		//! Will 'roll' a dice, returning `true` \f$100 * chance\f$ percent of the time.
		static bool RandomChance(const double chance);

	private:
		//! Will initialize the random number generator
		static void InitRng();

		static std::mt19937 rng;
		static bool isRngInitialized;

		// No instanciation! >:(
		Random();
	};
}
