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
	registeredTriangles.reserve(reserve_triangles);

	return;
}

void BackfaceCullingEngine::RegisterRenderTriangle(const RenderTriangle3D* triangle)
{
	registeredTriangles.emplace_back(
		std::pair<const RenderTriangle3D*, bool>(
			triangle,
			true // keep by default
			)
	);

	return;
}

void BackfaceCullingEngine::Cull()
{
	// MULTITHREAD THIS!

	for (auto& tri : registeredTriangles)
	{
		// Get average normal INCLUDING position
		const Vector3d avgNormal =
			(
				tri.first->a.pos_worldSpace + tri.first->a.normal +
				tri.first->b.pos_worldSpace + tri.first->b.normal +
				tri.first->c.pos_worldSpace + tri.first->c.normal) / 3.0;

		// Get dot to world origin
		const double dot = avgNormal.DotProduct(Vector3d::backward);

		if (dot <= 0)
			tri.second = false;
	}

	return;
}

std::vector<const RenderTriangle3D*> BackfaceCullingEngine::Finish()
{
	std::vector<const RenderTriangle3D*> toRet;
	toRet.reserve(registeredTriangles.size());

	for (auto& tri : registeredTriangles)
		if (tri.second)
			toRet.emplace_back(tri.first);

	return toRet;
}
