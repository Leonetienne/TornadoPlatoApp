#pragma once
#include "Matrix4x4.h"
#include "WorkerPool.h"
#include "RenderTriangle3D.h"
#include "InterRenderTriangle.h"
#include "ProjectionProperties.h"
#include <vector>
#include <mutex>

namespace TorGL
{
	/** The engine performing the actual perspective projection.
	* And that it does in a multithreaded fashion.
	*/
	class ProjectionEngine
	{
	public:
		ProjectionEngine(WorkerPool* workerPool);

		//! Will initialize this batch (frame) of processing by clearing buffers, lists, etc...
		//! Call each frame before starting rendering
		void BeginBatch(std::size_t reserve_triangles = 0);

		//! Will register a RenderTriangle3D to be projected.
		void RegisterRenderTriangle(const RenderTriangle3D* triangle);

		//! Faster way of registering a lot of RenderTriangle3D's at once by using std::move. This will consume the original vector.
		void HardsetRenderTriangles(std::vector<const RenderTriangle3D*>&& triangles);

		//! Will chew through projecting (and clipping) all the registered triangles using multiple threads.
		void Project(const ProjectionProperties& projectionProperties, const Matrix4x4& worldMatrix);

		//! Will clean up any remaining mess and return the projected triangles as InterRenderTriangles.
		//! At this point, pos_ss will have been set.
		std::vector<InterRenderTriangle>& Finish();

	private:
		//! This is the task given to the WorkerPool. One task (call) per registered triangle.
		void Thread_ProjectTriangle(const RenderTriangle3D* tri, const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix);

		WorkerPool* workerPool;
		std::vector<const RenderTriangle3D*> registeredTriangles;
		std::vector<InterRenderTriangle> projectedTriangles;
		std::mutex resultVecMutex;
	};
}
