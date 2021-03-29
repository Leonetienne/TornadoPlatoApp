#include "Tornado.h"

Tornado::Tornado(const Vector2i& renderTargetSize, std::size_t numRenderthreads)
{
	workerPool = new WorkerPool(numRenderthreads);
	projectionEngine = new ProjectionEngine(workerPool);
	pixelBuffer = new PixelBuffer<3>(renderTargetSize);
	drawingEngine = new DrawingEngine(pixelBuffer, workerPool);

	return;
}

Tornado::~Tornado()
{
	delete workerPool;
	delete projectionEngine;
	delete drawingEngine;
	delete pixelBuffer;
	
	workerPool = nullptr;
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
	registeredTriangles.push_back(tri);

	return;
}

void Tornado::Render(const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix)
{
	// Project triangles
	projectionEngine->BeginBatch();

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

PixelBuffer<3>* Tornado::GetPixelBuffer() const
{
	return pixelBuffer;
}
