#pragma once
#include <math.h>

//! Compares two double values with a given accuracy
inline bool Similar(double a, double b, const double epsilon = 0.00001)
{
	return fabs(a - b) <= epsilon;
}
