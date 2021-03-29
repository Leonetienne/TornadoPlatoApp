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
	std::fill_n(zBuffer, numPixels, 100000000);
	memset(renderTarget->GetRawData(), 0, renderTarget->GetSizeofBuffer());

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
	// Code for testing
	// No multithreading yet

	for (const InterRenderTriangle* ird : registeredTriangles)
		for (std::size_t x = 0; x < renderTarget->GetDimensions().x; x++)
			for (std::size_t y = 0; y < renderTarget->GetDimensions().y; y++)
				if (ird->DoesScreenspaceContainPoint(Vector2d((double)x, (double)y)))
				{
					uint8_t* basePixel = renderTarget->GetRawData() + renderTarget->GetChannelWidth() * (y * renderTarget->GetDimensions().x + x);

					uint8_t& r = basePixel[0];
					uint8_t& g = basePixel[1];
					uint8_t& b = basePixel[2];
					uint8_t& a = basePixel[3];
					double& zBuf = zBuffer[x + y * renderTarget->GetDimensions().x];

					std::array<double, 5> berp_cache {0};
					const double z = BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
						*ird,
						Vector2d((double)x, (double)y),
						ird->a.pos_cs.z,
						ird->b.pos_cs.z,
						ird->c.pos_cs.z,
						&berp_cache
					);

					const Color vertexColor(
						BarycentricInterpolationEngine::PerspectiveCorrected(
							*ird,
							Vector2d((double)x, (double)y),
							ird->a.vertex_col.r,
							ird->b.vertex_col.r,
							ird->c.vertex_col.r
						),
						BarycentricInterpolationEngine::PerspectiveCorrected(
							*ird,
							Vector2d((double)x, (double)y),
							ird->a.vertex_col.g,
							ird->b.vertex_col.g,
							ird->c.vertex_col.g
						),
						BarycentricInterpolationEngine::PerspectiveCorrected(
							*ird,
							Vector2d((double)x, (double)y),
							ird->a.vertex_col.b,
							ird->b.vertex_col.b,
							ird->c.vertex_col.b
						),
						255
					);


					if (z < zBuf)
					{
						zBuf = z;

						r = vertexColor.r;
						g = vertexColor.g;
						b = vertexColor.b;
					}
				}

	return;
}
