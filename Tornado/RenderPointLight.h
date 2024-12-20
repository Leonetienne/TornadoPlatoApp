#pragma once
#include "Vector3.h"
#include "RenderLightSource.h"

namespace TorGL
{
	/** Tornados implementation of a simple point light.
	* This does not cast any shadows.
	*/
	class RenderPointLight : public RenderLightSource
	{
	public:
		//! Will return color multipliers/factors (r,g,b only) for this light source, given an InterRenderTriangle, and a worldspace point.  
		//! Multiply the raw color values with these factors to get the shaded color (for this light)
		Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point, const Vector3d& normal) const override;

	private:
		double GetHardlightFac(const double dot, const double invSqrCoefficient) const;
		double GetSoftlightFac(const double invSqrCoefficient) const;
	};
}
