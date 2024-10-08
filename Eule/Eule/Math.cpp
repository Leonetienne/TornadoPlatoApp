#include "Math.h"
#include "Constants.h"
#include <array>

using namespace Eule;

int Math::Mod(const int numerator, const int denominator)
{
	if (denominator == 0)
		throw std::logic_error("Division by zero");

	// Quick optimizations:

	// -> 0/n is always 0
	if (numerator == 0)
		return 0;

	// -> operator% works for a > 0 && b > 0
	if (denominator > 0 && numerator > 0)
		return numerator % denominator;

	// Else: generalized formula
	return (denominator + (numerator % denominator)) % denominator;
}

double Math::Oscillate(const double a, const double b, const double counter, const double speed)
{
	return (sin(counter * speed * PI - HALF_PI) * 0.5 + 0.5) * (b - a) + a;
}
