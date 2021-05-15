#pragma once
#include "WorkerPool.h"
#include "InterRenderTriangle.h"
#include <utility>

/** This engine is responsible for removing all triangles from the rendering queue, that are not facing the camera.
*/
class BackfaceCullingEngine
{
public:
	BackfaceCullingEngine(WorkerPool* workerPool);

	//! Will reset the engines internal state to be ready to compute a new frame.
	void BeginBatch(std::size_t reserve_triangles = 0);

	//! Will register an InterRenderTriangle to be checked if it should be culled.
	void RegisterRenderTriangle(const InterRenderTriangle* triangle);

	//! Will cull the triangles in a multithreaded workload
	void Cull();

	//! Will finish the job and return a vector of pointers to all InterRenderTriangles's that should be rendered.
	//! They point to the elements in the original vector.
	std::vector<const InterRenderTriangle*> Finish();

private:
	WorkerPool* workerPool;

	void Thread__CullTriangle(std::pair<const InterRenderTriangle*, bool>* ird);

	// bool -> keep the triangle?
	std::vector<std::pair<const InterRenderTriangle*, bool>> registeredTriangles;
};

