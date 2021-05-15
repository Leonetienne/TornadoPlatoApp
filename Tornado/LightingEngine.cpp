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
	// Calculate caches
	CalculateLightingRelatedCaches_IRD(ird);

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

void LightingEngine::CalculateLightingRelatedCaches_IRD(const InterRenderTriangle* ird)
{
	// Calculate surface normal for world space
	if (ird->surfaceNormalWs == Vector3d::zero)
		ird->surfaceNormalWs =
			(
				(
					ird->b.pos_ws - ird->a.pos_ws
				).CrossProduct(
					ird->c.pos_ws - ird->a.pos_ws
				)
			).Normalize();

	return;
}

std::unordered_set<const RenderLightSource*> LightingEngine::lightSources;
