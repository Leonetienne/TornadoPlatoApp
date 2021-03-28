#include "BarycentricInterpolationEngine.h"

double BarycentricInterpolationEngine::PerspectiveCorrected(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c)
{
	// Only calculate area if it has not been done already
	if (tri.ss_iarea == -1)
	{
		const double area = EdgeFunction(tri.a.pos_ss, tri.b.pos_ss, tri.c.pos_ss);
		if (area > 0)
			tri.ss_iarea = 1.0 / area;
	}
	if (tri.ss_iarea == -1)
		return -1;

	double a1 = EdgeFunction(pos, tri.b.pos_ss, tri.c.pos_ss);
	double a2 = EdgeFunction(tri.a.pos_ss, pos, tri.c.pos_ss);
	double a3 = EdgeFunction(tri.a.pos_ss, tri.b.pos_ss, pos);
	
	//// The 1 over z is NOT to convert back to clipping space (because z already is), but because
	//// we are pre-calculating a division by it.
	//// Technically just using tri.a.pos_ss.z should work aswell, but it causes artifacts most likely due to
	//// floating point inaccuracies. Let's just roll with 1.0 / cs.z
	if (tri.a.berp_iw == -1)
		tri.a.berp_iw = 1.0 / tri.a.pos_cs.z;
	
	if (tri.b.berp_iw == -1)
		tri.b.berp_iw = 1.0 / tri.b.pos_cs.z;
	
	if (tri.c.berp_iw == -1)
		tri.c.berp_iw = 1.0 / tri.c.pos_cs.z;
	
	if ((a1 >= 0) && (a2 >= 0) && (a3 >= 0))
	{
		a1 *= tri.ss_iarea;
		a2 *= tri.ss_iarea;
		a3 *= tri.ss_iarea;
		
		const double numerator   = (a1*val_a*tri.a.berp_iw) +  (a2*val_b*tri.b.berp_iw) + (a3*val_c*tri.c.berp_iw);
		const double denominator = (a1*tri.a.berp_iw) + (a2*tri.b.berp_iw) + (a3*tri.c.berp_iw);
		
		return numerator / denominator;
	}

	return -2;
}

double BarycentricInterpolationEngine::EdgeFunction(const Vector2d& a, const Vector2d& b, const Vector2d& c)
{
	return (c.x - a.x)* (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
