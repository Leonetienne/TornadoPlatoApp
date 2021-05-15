#pragma once
#include "Color.h"
#include "Vector3.h"
#include "InterRenderTriangle.h"

/** Abstract implementation of all light sources within Tornado.
* You can't instanciate it on its own, and even if you could, bad things would happen. Don't do it.
*/
class RenderLightSource
{
public:
	//! Will return color multipliers/factors (r,g,b only) for this light source, given an InterRenderTriangle, and a worldspace point.
	virtual Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point) const = 0;
	
	Color color;
	double intensity;
	Vector3d position;
};
