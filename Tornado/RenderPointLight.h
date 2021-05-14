#pragma once
#include "RenderLightSource.h"

class RenderPointLight : public RenderLightSource
{
public:
	//! Will return the factors to multiply the render colors with for a specific location on an InterRenderTriangle
	Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point) const override;
};

