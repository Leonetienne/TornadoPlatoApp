#include "DrawingEngine.h"
#include "BarycentricInterpolationEngine.h"
#include <cstddef>

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
	std::fill_n(zBuffer, numPixels, 100000000); // Clear z-buffer
	memset(renderTarget->GetRawData(), 0, renderTarget->GetSizeofBuffer()); // Clear render target

	// Clear triangle registry
	registeredTriangles.clear();

	// Pre-allocate memory for triangles, if a size got supplied
	if (reservesize_triangles > 0)
		registeredTriangles.reserve(reservesize_triangles);

	return;
}

void DrawingEngine::RegisterInterRenderTriangle(const InterRenderTriangle* tri)
{
	registeredTriangles.push_back(tri);
	return;
}

void DrawingEngine::Draw()
{
	CreateTasks();
	ComputeTasks();
	
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
		bounds.pos.x = std::min(ird->a.pos_ss.x, std::min(ird->b.pos_ss.x, ird->c.pos_ss.x));
		bounds.pos.y = std::min(ird->a.pos_ss.y, std::min(ird->b.pos_ss.y, ird->c.pos_ss.y));

		bounds.size.x = std::max(ird->a.pos_ss.x, std::max(ird->b.pos_ss.x, ird->c.pos_ss.x)) - bounds.pos.x;
		bounds.size.y = std::max(ird->a.pos_ss.y, std::max(ird->b.pos_ss.y, ird->c.pos_ss.y)) - bounds.pos.y;

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
						ird->a.pos_cs.z,
						ird->b.pos_cs.z,
						ird->c.pos_cs.z,
						&berp_cache
					);

					double& zBuf = zBuffer[x + y * renderTarget->GetDimensions().x];
					if (z < zBuf)
					{
						zBuf = z;
						Thread_PixelShader(ird, basePixel, pixelPosition, &berp_cache);
					}
				}
			}
		}
	}

	return;
}

void DrawingEngine::Thread_PixelShader(const InterRenderTriangle* ird, uint8_t* pixelBase, const Vector2d& pixelPosition, std::array<double, 5>* berp_cache)
{
	// Interpolate vertex color
	const Color vertexColor(
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.vertex_col.r,
			ird->b.vertex_col.r,
			ird->c.vertex_col.r,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.vertex_col.g,
			ird->b.vertex_col.g,
			ird->c.vertex_col.g,
			berp_cache
		),
		BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
			*ird,
			pixelPosition,
			ird->a.vertex_col.b,
			ird->b.vertex_col.b,
			ird->c.vertex_col.b,
			berp_cache
		)
	);

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
		uv_coords.x = std::max<double>(std::min<double>(uv_coords.x, 1.0), 0.0);
		uv_coords.y = std::max<double>(std::min<double>(uv_coords.y, 1.0), 0.0);

		uint8_t* text_pixel = ird->material->texture->GetPixelBuffer().GetPixel(Vector2i(
			(int)((1.0 - uv_coords.x) * (text_size.x - 1)),
			(int)((1.0 - uv_coords.y) * (text_size.y - 1)) // Uv-coordinates are top-left == (0,1)
		));

		r = text_pixel[0];
		g = text_pixel[1];
		b = text_pixel[2];
	}
	// If we have no material, paint it pink
	else
	{
		r = 255;
		g = 0;
		b = 255;
	}

	return;
}
