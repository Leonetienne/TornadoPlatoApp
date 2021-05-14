#pragma once
#include "Color.h"
#include "Vector3.h"
#include "InterRenderTriangle.h"

class RenderLightSource
{
public:
	virtual Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point) const;
	
	Color color;
	double intensity;
	Vector3d position;

};

