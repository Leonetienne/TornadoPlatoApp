#pragma once
#include "PixelBuffer.h"
#include "WorkerPool.h"
#include "InterRenderTriangle.h"

class DrawingEngine
{
public:
	DrawingEngine(PixelBuffer<3>* renderTarget, WorkerPool* workerPool);
	~DrawingEngine();

	// Will initialize the new drawing sequence
	void BeginBatch(std::size_t reservesize_triangles = 0);

	// Will register an InterRenderTriangle to be drawn.
	void RegisterInterRenderTriangle(const InterRenderTriangle* tri);

	// Will create and start the drawing tasks and allocate resources accordingly
	// Will freeze the main (calling) thread, until the drawing has been finished
	void Draw();

private:
	WorkerPool* workerPool;
	PixelBuffer<3>* renderTarget;

	double* zBuffer;
	std::size_t numPixels;
	std::vector<const InterRenderTriangle*> registeredTriangles;
};
