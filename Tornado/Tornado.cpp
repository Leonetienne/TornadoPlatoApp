#include "Tornado.h"
#include "RenderPointLight.h"

Tornado::Tornado(const Vector2i& renderTargetSize, std::size_t numRenderthreads)
{
	workerPool = new WorkerPool(numRenderthreads);
	backfaceCullingEngine = new BackfaceCullingEngine(workerPool);
	projectionEngine = new ProjectionEngine(workerPool);
	pixelBuffer = new PixelBuffer<3>(renderTargetSize);
	drawingEngine = new DrawingEngine(pixelBuffer, workerPool);

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
	// Project triangles
	projectionEngine->BeginBatch(registeredTriangles.size());

	// Pass triangle vector
	projectionEngine->HardsetRenderTriangles(std::move(registeredTriangles));

	// Project and receive results
	projectionEngine->Project(projectionProperties, worldMatrix);
	std::vector<InterRenderTriangle>& projectedTriangles = projectionEngine->Finish();



	// Cull backfaces
	backfaceCullingEngine->BeginBatch(registeredTriangles.size());

	for (const InterRenderTriangle& ird : projectedTriangles)
		backfaceCullingEngine->RegisterInterRenderTriangle(&ird);

	backfaceCullingEngine->Cull();
	std::vector<const InterRenderTriangle*>& culledTriangles = backfaceCullingEngine->Finish();



	// Init LightingEngine
	LightingEngine::BeginBatch(registeredLightsources.size());
	
	// Register components in LightingEngine
	LightingEngine::HardsetLightsources(std::move(registeredLightsources));



	// Draw triangles
	drawingEngine->BeginBatch(projectedTriangles.size());
	drawingEngine->HardsetInterRenderTriangles(std::move(culledTriangles));

	drawingEngine->Draw();

	return;
}

const PixelBuffer<3>* Tornado::GetPixelBuffer() const
{
	return pixelBuffer;
}
