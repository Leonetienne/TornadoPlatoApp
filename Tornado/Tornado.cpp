#include "Tornado.h"

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

	return;
}

void Tornado::RegisterRender(const RenderTriangle3D* tri)
{
	registeredTriangles.emplace_back(tri);

	return;
}

void Tornado::Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix)
{
	// Cull backfaces
	backfaceCullingEngine->BeginBatch(registeredTriangles.size());

	for (const RenderTriangle3D* tri : registeredTriangles)
		backfaceCullingEngine->RegisterRenderTriangle(tri);

	backfaceCullingEngine->Cull();
	registeredTriangles = backfaceCullingEngine->Finish();

	// Project triangles
	projectionEngine->BeginBatch(registeredTriangles.size());

	for (const RenderTriangle3D* tri : registeredTriangles)
		projectionEngine->RegisterRenderTriangle(tri);

	projectionEngine->Project(projectionProperties, worldMatrix);
	std::vector<InterRenderTriangle> projectedTriangles = projectionEngine->Finish();

	// Draw triangles
	drawingEngine->BeginBatch(projectedTriangles.size());
	for (const InterRenderTriangle& tri : projectedTriangles)
		drawingEngine->RegisterInterRenderTriangle(&tri);

	drawingEngine->Draw();

	return;
}

const PixelBuffer<3>* Tornado::GetPixelBuffer() const
{
	return pixelBuffer;
}
