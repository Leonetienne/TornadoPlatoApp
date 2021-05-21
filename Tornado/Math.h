#pragma once
#include <random>

/** Math utility class.
*/
class Math
{
public:
	//! Will return the bigger of two values
	[[nodiscard]] static constexpr double Max(const double a, const double b);
	
	//! Will return the smaller of two values
	[[nodiscard]] static constexpr double Min(const double a, const double b);
	
	//! Will return `v`, but at least `min`, and at most `max`
	[[nodiscard]] static constexpr double Clamp(const double v, const double min, const double max);
	
	//! Will return the linear interpolation between `a` and `b` by `t`
	[[nodiscard]] static constexpr double Lerp(double a, double b, double t);
	
	//! Will return the absolute value of `a`
	[[nodiscard]] static constexpr double Abs(const double a);

	//! Compares two double values with a given accuracy
	[[nodiscard]] static constexpr bool Similar(const double a, const double b, const double epsilon = 0.00001);

	//! Will return a random double between `0` and `1`
	static double Random();
	
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
	
	//! Kind of like \f$sin(counter)\f$, but it oscillates over \f$[a,b]\f$ instead of \f$[-1,1]\f$, by a given speed.  
	//! Given that \f$speed = 1\f$, the result will always be `a` if `counter` is even, and `b` if `counter` is uneven.  
	//! If `counter` is a rational, the result will oscillate between `a` and `b`, like `sin()` does.  
	//! If you increase `speed`, the oscillation frequency will increase. Meaning \f$speed = 2\f$ would result in \f$counter=0.5\f$ returning `b`.
	static double Oscillate(const double a, const double b, const double counter, const double speed);

private:
	//! Will initialize the random number generator
	static void InitRng();

	static std::mt19937 rng;
	static bool isRngInitialized;

	// No instanciation! >:(
	Math();
};



/*     These are just the inline methods. They have to lie in the header file.     */
/*     The more sophisticated methods are in the .cpp					           */

constexpr inline double Math::Max(double a, double b)
{
	return (a > b) ? a : b;
}

constexpr inline double Math::Min(double a, double b)
{
	return (a < b) ? a : b;
}

constexpr inline double Math::Clamp(double v, double min, double max)
{
	return Max(Min(v, max), min);
}

constexpr inline double Math::Lerp(double a, double b, double t)
{
	const double it = 1.0 - t;
	return (a * it) + (b * t);
}

inline constexpr double Math::Abs(const double a)
{
	return (a > 0.0) ? a : -a;
}

inline constexpr bool Math::Math::Similar(const double a, const double b, const double epsilon)
{
	return Abs(a - b) <= epsilon;
}
