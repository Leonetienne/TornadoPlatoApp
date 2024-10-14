#include "Tornado.h"

using namespace TorGL;

#ifdef _BENCHMARK_CONTEXT
// If we are in a benchmarking context, create a timer used to capture runtimes of inidividual methods
#include <chrono>
namespace {
    std::chrono::high_resolution_clock clock;
    std::chrono::high_resolution_clock::time_point begin;
    double clock_getElapsedTimeMs() {
        return (double)(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - begin).count());
    }
    void clock_reset() {
	    begin = std::chrono::high_resolution_clock::now();
    }
}
#endif

Tornado::Tornado(const Vector2i& renderTargetSize, std::size_t numRenderthreads, double globalIllumination)
{
	workerPool = new WorkerPool(numRenderthreads);
	backfaceCullingEngine = new BackfaceCullingEngine(workerPool);
	projectionEngine = new ProjectionEngine(workerPool);
	pixelBuffer = new PixelBuffer<3>(renderTargetSize);
	drawingEngine = new DrawingEngine(pixelBuffer, workerPool, globalIllumination);

	return;
}

Tornado::~Tornado()
{
	delete workerPool;
	delete backfaceCullingEngine;
	delete projectionEngine;
	delete drawingEngine;
	delete pixelBuffer;
	
	workerPool = nullptr;
	backfaceCullingEngine = nullptr;
	projectionEngine = nullptr;
	drawingEngine = nullptr;
	pixelBuffer = nullptr;

	return;
}

void Tornado::BeginFrame()
{
	registeredTriangles.clear();
	registeredLightsources.clear();

	registeredTriangles.reserve(200);
	registeredLightsources.reserve(4);

	return;
}

void Tornado::RegisterRender(const RenderTriangle3D* tri)
{
	registeredTriangles.emplace_back(tri);

	return;
}

void Tornado::RegisterRender(const RenderLightSource* lightSource)
{
	registeredLightsources.emplace_back(lightSource);

	return;
}

void Tornado::Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix)
{
    /// PERSPECTIVE PROJECTION
    #ifdef _BENCHMARK_CONTEXT
        clock_reset();
    #endif

	// Project triangles
	projectionEngine->BeginBatch(registeredTriangles.size());

	// Pass triangle vector
	projectionEngine->HardsetRenderTriangles(std::move(registeredTriangles));

	// Project and receive results
	projectionEngine->Project(projectionProperties, worldMatrix);
	std::vector<InterRenderTriangle>& projectedTriangles = projectionEngine->Finish();

    #ifdef _BENCHMARK_CONTEXT
        _benchmark_perspectiveProjectionTime = clock_getElapsedTimeMs();
    #endif


    /// BACKFACE CULLING
    #ifdef _BENCHMARK_CONTEXT
        clock_reset();
    #endif

	backfaceCullingEngine->BeginBatch(registeredTriangles.size());

	for (const InterRenderTriangle& ird : projectedTriangles)
		backfaceCullingEngine->RegisterInterRenderTriangle(&ird);

	backfaceCullingEngine->Cull();
	std::vector<const InterRenderTriangle*>& culledTriangles = backfaceCullingEngine->Finish();

    #ifdef _BENCHMARK_CONTEXT
        _benchmark_cullBackfacesTime = clock_getElapsedTimeMs();
    #endif


	// Init LightingEngine
	LightingEngine::BeginBatch(registeredLightsources.size());
	
	// Register components in LightingEngine
	LightingEngine::HardsetLightsources(std::move(registeredLightsources));


	// Draw triangles
    #ifdef _BENCHMARK_CONTEXT
        clock_reset();
    #endif
	drawingEngine->BeginBatch(projectedTriangles.size());
	drawingEngine->HardsetInterRenderTriangles(std::move(culledTriangles));
	drawingEngine->Draw();
    #ifdef _BENCHMARK_CONTEXT
        _benchmark_drawTrianglesTime = clock_getElapsedTimeMs();
    #endif

	return;
}

const PixelBuffer<3>* Tornado::GetPixelBuffer() const
{
	return pixelBuffer;
}
