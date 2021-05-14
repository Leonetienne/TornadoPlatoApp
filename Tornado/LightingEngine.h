#pragma once
#include "RenderLightSource.h"
#include <unordered_set>

class LightingEngine
{
public:
	static void BeginBatch(std::size_t reserve_lightSources = 0);
	
	static void RegisterLightSource(const RenderLightSource* lightSource);

	static Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point);

private:
	static std::unordered_set<const RenderLightSource*> lightSources;
};

