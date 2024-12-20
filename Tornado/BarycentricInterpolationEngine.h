#pragma once
#include "Vector2.h"
#include "InterRenderTriangle.h"

namespace TorGL
{
	/** Perspective accurately interpolates values between three points.
	* This class is used to interpolate values between three points, within a triangle.
	* The PerspectiveCorrected methods account for perspective distortion (depth).
	* Use the CachedValues variant for extra performance. It is a bit more wordy, but surely faster!
	*/
	class BarycentricInterpolationEngine
	{
	public:
		//! Perspective correct barycentric interpolation
		static double PerspectiveCorrected(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c);

		//! Perspective correct barycentric interpolation, but all per-pixel constant values are cached in reference parameters. Should be faster when interpolating multiple attributes per pixel. Just supply such a pointer to such an array and you're good to go! Make sure the array is zeroed!!
		static double PerspectiveCorrect__CachedValues(const InterRenderTriangle& tri, const Vector2d& pos, double val_a, double val_b, double val_c, std::array<double, 5>* cache);

	private:
		static double EdgeFunction(const Vector2d& a, const Vector2d& b, const Vector2d& c);
	};
}
