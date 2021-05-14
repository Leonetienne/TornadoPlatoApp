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
	LightingEngine::BeginBatch(1);

	return;
}

void Tornado::RegisterRender(const RenderTriangle3D* tri)
{
	registeredTriangles.emplace_back(tri);

	return;
}

void Tornado::RegisterRender(const RenderLightSource* lightSource)
{
	LightingEngine::RegisterLightSource(lightSource);

	return;
}

void Tornado::Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix)
{
	// Project triangles
	projectionEngine->BeginBatch(registeredTriangles.size());

	for (const RenderTriangle3D* tri : registeredTriangles)
		projectionEngine->RegisterRenderTriangle(tri);

	projectionEngine->Project(projectionProperties, worldMatrix);
	std::vector<InterRenderTriangle> projectedTriangles = projectionEngine->Finish();



	// Cull backfaces
	backfaceCullingEngine->BeginBatch(registeredTriangles.size());

	for (const InterRenderTriangle& ird : projectedTriangles)
		backfaceCullingEngine->RegisterRenderTriangle(&ird);

	backfaceCullingEngine->Cull();
	std::vector<const InterRenderTriangle*> culledTriangles = backfaceCullingEngine->Finish();


	// Draw triangles
	drawingEngine->BeginBatch(projectedTriangles.size());
	for (const InterRenderTriangle* tri : culledTriangles)
		drawingEngine->RegisterInterRenderTriangle(tri);

	drawingEngine->Draw();

	return;
}

const PixelBuffer<3>* Tornado::GetPixelBuffer() const
{
	return pixelBuffer;
}
