#pragma once
#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"
#include "../Tornado/Vector4.h"
#include <math.h>

// Compares double values with a given accuracy
inline bool Similar(double a, double b, const double epsilon = 0.00001)
{
	return fabs(a - b) <= epsilon;
}

// Compares vector values with a given accuracy
inline bool Similar(const Vector2d& a, const Vector2d& b, const double epsilon = 0.00001)
{
	return 
		(Similar(a.x, b.x, epsilon)) &&
		(Similar(a.y, b.y, epsilon))
	;
}

// Compares vector values with a given accuracy
inline bool Similar(const Vector3d& a, const Vector3d& b, const double epsilon = 0.00001)
{
	return
		(Similar(a.x, b.x, epsilon)) &&
		(Similar(a.y, b.y, epsilon)) &&
		(Similar(a.z, b.z, epsilon))
	;
}

// Compares vector values with a given accuracy
inline bool Similar(const Vector4d& a, const Vector4d& b, const double epsilon = 0.00001)
{
	return
		(Similar(a.x, b.x, epsilon)) &&
		(Similar(a.y, b.y, epsilon)) &&
		(Similar(a.z, b.z, epsilon)) &&
		(Similar(a.w, b.w, epsilon))
	;
}
