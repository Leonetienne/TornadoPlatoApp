#include "BarycentricInterpolationEngine.h"

double BarycentricInterpolationEngine::PerspectiveCorrected(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c)
{
	std::array<double, 5> cache { 0 };
	return PerspectiveCorrect__CachedValues(tri, pos, val_a, val_b, val_c, &cache);
}

double BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c, std::array<double, 5>* cache)
{
	double& shouldInitialize = cache->at(0);
	double& aw1 = cache->at(1);
	double& aw2 = cache->at(2);
	double& aw3 = cache->at(3);
	double& iaws = cache->at(4);

	// This scope gets only executed once per pixel, even when interpolating multiple attributes per pixel!
	// Array is zeroed by default. If this value is zero, we should initialize the values
	if (shouldInitialize == 0)
	{
		shouldInitialize = 1;

		double a1 = EdgeFunction(pos, tri.b.pos_ss, tri.c.pos_ss);
		double a2 = EdgeFunction(tri.a.pos_ss, pos, tri.c.pos_ss);
		double a3 = EdgeFunction(tri.a.pos_ss, tri.b.pos_ss, pos);

		if ((a1 >= 0) && (a2 >= 0) && (a3 >= 0))
		{
			// The 1 over z is NOT to convert back to clipping space (because z already is), but because
			// we are pre-calculating a division by it.
			// Technically just using tri.a.pos_ss.z should work aswell, but it causes artifacts most likely due to
			// floating point inaccuracies. Let's just roll with 1.0 / cs.z
			if (tri.a.berp_iw == -1)
				tri.a.berp_iw = 1.0 / tri.a.pos_cs.z;

			if (tri.b.berp_iw == -1)
				tri.b.berp_iw = 1.0 / tri.b.pos_cs.z;

			if (tri.c.berp_iw == -1)
				tri.c.berp_iw = 1.0 / tri.c.pos_cs.z;

			aw1 = a1 * tri.ss_iarea * tri.a.berp_iw;
			aw2 = a2 * tri.ss_iarea * tri.b.berp_iw;
			aw3 = a3 * tri.ss_iarea * tri.c.berp_iw;
			iaws = 1.0 / (aw1 + aw2 + aw3); // Pre-calculate division
			aw1 *= iaws; // Pre-calculate division by iaws
			aw2 *= iaws;
			aw3 *= iaws;
		}
	}

	// Now calculate the interpolated value using cached values
	// Only this code gets called per attribute per pixel (when using cache)
	return (aw1 * val_a) + (aw2 * val_b) + (aw3 * val_c);
}



// Known good function, in case any issues occur
//double BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c, std::array<double, 5>* cache)
//{
//	const Vector4d pos4d(pos.x, pos.y, 0, 0);
//
//	const double area = EdgeFunction(tri.a.pos_ss, tri.b.pos_ss, tri.c.pos_ss);
//	double a1 = EdgeFunction(pos, tri.b.pos_ss, tri.c.pos_ss);
//	double a2 = EdgeFunction(tri.a.pos_ss, pos, tri.c.pos_ss);
//	double a3 = EdgeFunction(tri.a.pos_ss, tri.b.pos_ss, pos);
//
//	const double w1 = 1.0 / tri.a.pos_cs.z;
//	const double w2 = 1.0 / tri.b.pos_cs.z;
//	const double w3 = 1.0 / tri.c.pos_cs.z;
//
//	//if (GetAsyncKeyState('T'))
//	//	std::cout << -triangle.ndc_b.z << std::endl;
//
//	// If pos is inside the triangle
//	if ((a1 >= 0) && (a2 >= 0) && (a3 >= 0))
//	{
//		a1 /= area;
//		a2 /= area;
//		a3 /= area;
//
//
//		const double zaehler = a1 * val_a * w1 + a2 * val_b * w2 + a3 * val_c * w3;
//		const double nenner = (a1 * w1) + (a2 * w2) + (a3 * w3);
//
//		return zaehler / nenner;
//	}
//
//	return 0.0;
//}


double BarycentricInterpolationEngine::EdgeFunction(const Vector2d& a, const Vector2d& b, const Vector2d& c)
{
	return (c.x - a.x)* (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}
