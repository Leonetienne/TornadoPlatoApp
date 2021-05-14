#include "LightingEngine.h"

void LightingEngine::BeginBatch(std::size_t reserve_lightSources)
{
	lightSources.clear();
	lightSources.reserve(reserve_lightSources);

	return;
}

void LightingEngine::RegisterLightSource(const RenderLightSource* lightSource)
{
	lightSources.emplace(lightSource);
	return;
}

Color LightingEngine::GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point)
{
	Color totalIntensity(0,0,0);

	for (const RenderLightSource* ls : lightSources)
	{
		Color result = ls->GetColorIntensityFactors(ird, point);
		
		totalIntensity.r += result.r;
		totalIntensity.g += result.g;
		totalIntensity.b += result.b;
	}

	return totalIntensity;
}

std::unordered_set<const RenderLightSource*> LightingEngine::lightSources;
