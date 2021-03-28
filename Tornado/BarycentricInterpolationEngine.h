#pragma once
#include "InterRenderTriangle.h"

class BarycentricInterpolationEngine
{
public:
	// Perspective correct barycentric interpolation
	static double PerspectiveCorrected(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c);
	
	// Perspective correct barycentric interpolation, but all per-pixel constant values are cached in reference parameters. Should be faster when interpolating multiple attributes per pixel. Just supply such a pointer to such an array and you're good to go! Make sure the array is zeroed!!
	static double PerspectiveCorrect__CachedValues(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c, std::array<double, 5>* cache);

private:
	static double EdgeFunction(const Vector2d& a, const Vector2d& b, const Vector2d& c);
};

