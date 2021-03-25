#pragma once
#include <math.h>

// Compares double values with a given accuracy
inline bool Similar(double a, double b, double epsilon = 0.00001)
{
	return fabs(a - b) <= epsilon;
}
