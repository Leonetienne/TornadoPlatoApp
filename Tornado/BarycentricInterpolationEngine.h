#pragma once
#include "InterRenderTriangle.h"

class BarycentricInterpolationEngine
{
public:
	static double PerspectiveCorrected(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c);

private:
	static double EdgeFunction(const Vector2d& a, const Vector2d& b, const Vector2d& c);
};

