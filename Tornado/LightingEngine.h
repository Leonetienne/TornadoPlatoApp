#pragma once
#include "RenderLightSource.h"
#include <unordered_set>

/** This engine is responsible for calculating brightness levels (per r,g,b) for all registered RenderLightSource's, given an InterRenderTriangle and a worldspace point.
* GetColorIntensityFactors() should best be called in a multithreaded fashion.
*/
class LightingEngine
{
public:
	//! Will reset the engines internal state to be ready for a new frame
	static void BeginBatch(std::size_t reserve_lightSources = 0);
	
	//! Will register a light source to be considerated
	static void RegisterLightSource(const RenderLightSource* lightSource);

	//! Will return the factors to multiply the render colors with for a specific location on an InterRenderTriangle. The point must be in world space.
	static Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point);


private:
	//! Will calculate needed values for this InterRenderTriangle.
	static void CalculateLightingRelatedCaches_IRD(const InterRenderTriangle* ird);

	static std::unordered_set<const RenderLightSource*> lightSources;
};

