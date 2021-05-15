#include "BackfaceCullingEngine.h"

BackfaceCullingEngine::BackfaceCullingEngine(WorkerPool* workerPool)
	:
	workerPool { workerPool }
{
	return;
}

void BackfaceCullingEngine::BeginBatch(std::size_t reserve_triangles)
{
	registeredTriangles.clear();
	culledTriangles.clear();

	culledTriangles.reserve(reserve_triangles); // Prepared for the best case. Won't be that much memory.
	registeredTriangles.reserve(reserve_triangles);

	return;
}

void BackfaceCullingEngine::RegisterRenderTriangle(const InterRenderTriangle* triangle)
{
	registeredTriangles.emplace_back(
		std::pair<const InterRenderTriangle*, bool>(
			triangle,
			true // keep by default
		)
	);

	return;
}

void BackfaceCullingEngine::Cull()
{
	// Create and queue worker tasks
	for (auto& tri : registeredTriangles)
	{
		WorkerTask* newTask = new WorkerTask; // Will be freed by the workerPool

		newTask->task = std::bind(&BackfaceCullingEngine::Thread__CullTriangle, this,
			&tri
		);

		workerPool->QueueTask(newTask);
	}

	// Execute worker tasks
	workerPool->Execute();

	return;
}

void BackfaceCullingEngine::Thread__CullTriangle(std::pair<const InterRenderTriangle*, bool>* ird)
{
	// Calculate InterRenderTriangle surface normal (in device space)
	ird->first->surfaceNormalNdc =
		(
			(
				ird->first->b.pos_ndc - ird->first->a.pos_ndc
			).CrossProduct(
				ird->first->c.pos_ndc - ird->first->a.pos_ndc
			)
		);

	// Get dot to world origin
	const double dot = ird->first->surfaceNormalNdc.DotProduct(Vector3d::backward);


	if (dot > 0)
		ird->second = false;

	return;
}

std::vector<const InterRenderTriangle*>& BackfaceCullingEngine::Finish()
{
	culledTriangles.reserve(registeredTriangles.size());

	for (auto& tri : registeredTriangles)
		if (tri.second)
			culledTriangles.emplace_back(tri.first);

	return culledTriangles;
}
