#pragma once
#include <math.h>

// Compares double values with an accuracy of 0.00001
inline bool Similar(double a, double b)
{
	return fabs(a - b) < 0.00001;
}
