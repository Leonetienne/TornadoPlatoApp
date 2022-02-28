#include "DrawingEngine.h"
#include "BarycentricInterpolationEngine.h"
#include <Eule/Eule/Math.h>
#include <cstddef>

using namespace TorGL;
using namespace Eule;

DrawingEngine::DrawingEngine(PixelBuffer<3>* renderTarget, WorkerPool* workerPool)
	:
	workerPool {workerPool},
	renderTarget {renderTarget}

{
	numPixels = (std::size_t)renderTarget->GetDimensions().x * (std::size_t)renderTarget->GetDimensions().y;

	zBuffer = new double[numPixels];

	return;
}

DrawingEngine::~DrawingEngine()
{
	delete[] zBuffer;

	zBuffer = nullptr;

	return;
}

void DrawingEngine::BeginBatch(std::size_t reservesize_triangles)
{
	// Clear buffers
	std::fill_n(zBuffer, numPixels, 1000000); // Clear z-buffer
	renderTarget->Clear({0}); // Clear pixel buffer black

	// Clear triangle registry
	registeredTriangles.clear();

	// Pre-allocate memory for triangles, if a size got supplied
	if (reservesize_triangles > 0)
		registeredTriangles.reserve(reservesize_triangles);

	return;
}

void DrawingEngine::RegisterInterRenderTriangle(const InterRenderTriangle* tri)
{
	// Calculate cache values
	CalculateRenderingRelatedCaches_IRD(tri);

	// Queue
	registeredTriangles.emplace_back(tri);
	return;
}

void DrawingEngine::HardsetInterRenderTriangles(std::vector<const InterRenderTriangle*>&& triangles)
{
	// Replace our vector
	this->registeredTriangles = std::move(triangles);

	// Now update the cached values
	for (const InterRenderTriangle* ird : registeredTriangles)
		CalculateRenderingRelatedCaches_IRD(ird);

	return;
}

void DrawingEngine::Draw()
{
	CreateTasks();
	ComputeTasks();
	
	return;
}

void DrawingEngine::CalculateRenderingRelatedCaches_IRD(const InterRenderTriangle* ird)
{
	// These normals are the meshes vertices normals!
	// These should be rotated accordingly to the meshes rotation
	ird->meanVertexNormal =
		(ird->a.normal + ird->b.normal + ird->c.normal) / 3.0;

	return;
}

void DrawingEngine::CreateTasks()
{
	// Calculate maximum screen area
	const std::size_t totalScreenArea = (std::size_t)renderTarget->GetDimensions().x * (std::size_t)renderTarget->GetDimensions().y;

	for (const InterRenderTriangle* ird : registeredTriangles)
	{
		// Decide how many threads to allocate to drawing this triangle
		const double normalizedScreenAreaPercentage = abs(ird->ss_area) / (double)totalScreenArea;
		const double lerpedThreads = normalizedScreenAreaPercentage * (workerPool->GetNumWorkers() - 1); // f.e. 0-15
		const std::size_t numThreadsForTriangle = std::min<std::size_t>((std::size_t)lerpedThreads + 1, 16); // would now be 1-16
		
		// Calculate triangle bounds, minmaxed within render area bounds
		Rect bounds;
		bounds.pos.x = Math::Min(ird->a.pos_ss.x, Math::Min(ird->b.pos_ss.x, ird->c.pos_ss.x));
		bounds.pos.y = Math::Min(ird->a.pos_ss.y, Math::Min(ird->b.pos_ss.y, ird->c.pos_ss.y));

		bounds.size.x = Math::Max(ird->a.pos_ss.x,Math::Max(ird->b.pos_ss.x, ird->c.pos_ss.x)) - bounds.pos.x;
		bounds.size.y = Math::Max(ird->a.pos_ss.y,Math::Max(ird->b.pos_ss.y, ird->c.pos_ss.y)) - bounds.pos.y;

		// Now split this triangle into worker tasks
		for (std::size_t i = 0; i < numThreadsForTriangle; i++)
		{
			// Calculate task working areas
			const double taskSegment = bounds.size.x / numThreadsForTriangle;
			Rect workerBounds;
			workerBounds.pos.x = bounds.pos.x + (taskSegment * i);
			workerBounds.pos.y = bounds.pos.y;
			workerBounds.size.x = taskSegment;
			workerBounds.size.y = bounds.size.y;

			// Add one pixel of padding to the right (if it wouldn't cause out-of-range access)
			// To correct floating-point in accuraccies (which would cause unrendered lines)
			if (workerBounds.pos.x + workerBounds.size.x < renderTarget->GetDimensions().x - std::size_t(2))
				workerBounds.size.x += 2;
			if (workerBounds.pos.y + workerBounds.size.y < renderTarget->GetDimensions().y - std::size_t(2))
				workerBounds.size.y += 2;

			// Create new task 
			WorkerTask* newTask = new WorkerTask; // Will be freed by the workerPool
			newTask->task = std::bind(&DrawingEngine::Thread_Draw, this,
				ird, workerBounds);
			workerPool->QueueTask(newTask);
		}
	}

	return;
}

void DrawingEngine::ComputeTasks()
{
	// Launch tasks
	workerPool->Execute();
	return;
}

void DrawingEngine::Thread_Draw(const InterRenderTriangle* ird, const Rect& bounds)
{
	std::array<double, 5> berp_cache{ 0 };
	const std::size_t maxx = (std::size_t)bounds.pos.x + (std::size_t)bounds.size.x;
	const std::size_t maxy = (std::size_t)bounds.pos.y + (std::size_t)bounds.size.y;

	for (std::size_t y = (std::size_t)bounds.pos.y; y < maxy; y++)
	{
		const std::size_t row = y * renderTarget->GetDimensions().x;
		for (std::size_t x = (std::size_t)bounds.pos.x; x < maxx; x++)
		{
			if ((x > 0) && (y > 0) && (x < renderTarget->GetDimensions().x) && (y < renderTarget->GetDimensions().y))
			{
				Vector2d pixelPosition((double)x, (double)y);
				if (ird->DoesScreenspaceContainPoint(pixelPosition))
				{
					std::size_t pixelIndex = (row + x) * renderTarget->GetChannelWidth();
					uint8_t* basePixel = renderTarget->GetRawData() + pixelIndex;

					berp_cache[0] = 0;
					const double z = BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
						*ird,
						pixelPosition,
						ird->a.pos_ss.z,
						ird->b.pos_ss.z,
						ird->c.pos_ss.z,
						&berp_cache
					);

					double& zBuf = zBuffer[x + y * renderTarget->GetDimensions().x];
					if (z < zBuf)
					{
						zBuf = z;
						Thread_PixelShader(ird, basePixel, pixelPosition, &berp_cache, z);
					}
				}
			}
		}
	}

	return;
}

void DrawingEngine::Thread_PixelShader(const InterRenderTriangle* ird, uint8_t* pixelBase, const Vector2d& pixelPosition, std::array<double, 5>* berp_cache, double z)
{
	Vector2d uv_coords(
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.pos_uv.x,
			ird->b.pos_uv.x,
			ird->c.pos_uv.x,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.pos_uv.y,
			ird->b.pos_uv.y,
			ird->c.pos_uv.y,
			berp_cache
		)
	);

	Vector3d ws_coords(
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.pos_ws.x,
			ird->b.pos_ws.x,
			ird->c.pos_ws.x,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.pos_ws.y,
			ird->b.pos_ws.y,
			ird->c.pos_ws.y,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.pos_ws.z,
			ird->b.pos_ws.z,
			ird->c.pos_ws.z,
			berp_cache
		)
	);

	Vector3d smooth_normal(
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.normal.x,
			ird->b.normal.x,
			ird->c.normal.x,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.normal.y,
			ird->b.normal.y,
			ird->c.normal.y,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.normal.z,
			ird->b.normal.z,
			ird->c.normal.z,
			berp_cache
		)
	);

	uint8_t& r = pixelBase[0];
	uint8_t& g = pixelBase[1];
	uint8_t& b = pixelBase[2];


	// Do we have a material?
	if (ird->material != nullptr)
	{
		Vector2d text_size(
			ird->material->texture->GetPixelBuffer().GetDimensions().x,
			ird->material->texture->GetPixelBuffer().GetDimensions().y
		);

		// Clamp to 0 <= n <= 1 to compensate for floating point inaccuracies
		uv_coords.x = Math::Clamp(uv_coords.x, 0.0, 1.0);
		uv_coords.y = Math::Clamp(uv_coords.y, 0.0, 1.0);

		uint8_t* text_pixel = ird->material->texture->GetPixelBuffer().GetPixel(Vector2i(
			(int)((uv_coords.x) * (text_size.x - 1)),
			(int)((1.0 - uv_coords.y) * (text_size.y - 1)) // Uv-coordinates are top-left == (0,1)
		));
		
		// Set global illumination (minimum brightness)
		constexpr double globalIllu = 1;

		// Calculate brightness (if we should shade)
		Color brightness = Color(1,1,1);
		if (!ird->material->noShading)
		{
			// Apply lighting
			const Color lightingIntensity =
				LightingEngine::GetColorIntensityFactors(ird, ws_coords, smooth_normal);

			brightness.r = lightingIntensity.r / 255.0;
			brightness.g = lightingIntensity.g / 255.0;
			brightness.b = lightingIntensity.b / 255.0;
		
			// Apply global illumination
			// This could overflow the int!!! FIX THIS
			brightness.r += globalIllu;
			brightness.g += globalIllu;
			brightness.b += globalIllu;
		}

		r = uint8_t(Math::Clamp((double)text_pixel[0] * brightness.r, 0, 255));
		g = uint8_t(Math::Clamp((double)text_pixel[1] * brightness.g, 0, 255));
		b = uint8_t(Math::Clamp((double)text_pixel[2] * brightness.b, 0, 255));

		// Render camera-space normals
		//r = uint8_t(255.0 * (smooth_normal.x + 1.0) * 0.5);
		//g = uint8_t(255.0 * (smooth_normal.y + 1.0) * 0.5);
		//b = uint8_t(255.0 * (smooth_normal.z + 1.0) * 0.5);
	}
	// If we have no material, paint the normal map
	else
	{
		r = 255 * uint8_t((ird->meanVertexNormal.x + 1) * 0.5);
		g = 255 * uint8_t((ird->meanVertexNormal.y + 1) * 0.5);
		b = 255 * uint8_t((ird->meanVertexNormal.z + 1) * 0.5);
	}

	return;
}
