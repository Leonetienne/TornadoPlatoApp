#pragma once
#include "RenderLightSource.h"
#include "Vector3.h"
#include <vector>

namespace TorGL
{
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

		//! Faster way of registering a lot of RenderLightSource's at once using std::move. This will consume the original vector.
		static void HardsetLightsources(std::vector<const RenderLightSource*>&& lightSources);

		//! Will return the factors to multiply the render colors with for a specific location on an InterRenderTriangle. The point must be in world space.
		//! Multiply the raw color values with these factors to get the shaded color (for this light)
		static Color GetColorIntensityFactors(const InterRenderTriangle* ird, const Vector3d& point, const Vector3d& normal);

	private:
		//! Will calculate needed values for this InterRenderTriangle.
		static void CalculateLightingRelatedCaches_IRD(const InterRenderTriangle* ird);

		static std::vector<const RenderLightSource*> lightSources;
	};
}
