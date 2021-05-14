#pragma once
#include "WorkerPool.h"
#include "RenderTriangle3D.h"
#include <utility>

class BackfaceCullingEngine
{
public:
	BackfaceCullingEngine(WorkerPool* workerPool);

	void BeginBatch(std::size_t reserve_triangles = 0);
	void RegisterRenderTriangle(const RenderTriangle3D* triangle);
	void Cull();
	std::vector<const RenderTriangle3D*> Finish();

private:
	WorkerPool* workerPool;

	// bool -> keep the triangle?
	std::vector<std::pair<const RenderTriangle3D*, bool>> registeredTriangles;
};

