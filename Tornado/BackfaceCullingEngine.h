#pragma once
#include "WorkerPool.h"
#include "InterRenderTriangle.h"
#include <utility>

class BackfaceCullingEngine
{
public:
	BackfaceCullingEngine(WorkerPool* workerPool);

	void BeginBatch(std::size_t reserve_triangles = 0);
	void RegisterRenderTriangle(const InterRenderTriangle* triangle);
	void Cull();
	std::vector<const InterRenderTriangle*> Finish();

private:
	WorkerPool* workerPool;

	void Thread__CullTriangle(std::pair<const InterRenderTriangle*, bool>* ird);

	// bool -> keep the triangle?
	std::vector<std::pair<const InterRenderTriangle*, bool>> registeredTriangles;
};

