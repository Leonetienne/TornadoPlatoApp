#include "ProjectionEngine.h"
#include "ClippingEngine.h"

ProjectionEngine::ProjectionEngine(WorkerPool* workerPool)
{
	this->workerPool = workerPool;

	return;
}

void ProjectionEngine::BeginBatch(std::size_t reserve_triangles)
{
	registeredTriangles.clear();
	projectedTriangles.clear();

	registeredTriangles.reserve(reserve_triangles);
	projectedTriangles.reserve(reserve_triangles);

	return;
}

void ProjectionEngine::RegisterRenderTriangle(const RenderTriangle3D* triangle)
{
	registeredTriangles.emplace_back(triangle);
	return;
}

void ProjectionEngine::Project(const ProjectionProperties& projectionProperties, const Matrix4x4& worldMatrix)
{
	for (const RenderTriangle3D* tri : registeredTriangles)
	{
		WorkerTask* newTask = new WorkerTask; // Will be freed by the workerPool
		newTask->task = std::bind(&ProjectionEngine::Thread_ProjectTriangle, this,
			tri, projectionProperties, worldMatrix);

		workerPool->QueueTask(newTask);
	}

	workerPool->Execute();

	return;
}

void ProjectionEngine::Thread_ProjectTriangle(const RenderTriangle3D* tri, const ProjectionProperties& projectionProperties, const Matrix4x4 worldMatrix)
{
	// Perform a quick distance check, and cull the triangle if all vertices are farther away than the farclip
	if ((tri->a.pos_worldSpace.SqrMagnitude() > projectionProperties.GetSqrFarclip()) &&
		(tri->b.pos_worldSpace.SqrMagnitude() > projectionProperties.GetSqrFarclip()) &&
		(tri->c.pos_worldSpace.SqrMagnitude() > projectionProperties.GetSqrFarclip()))
		return;

	// Create InterRenderTriangle
	InterRenderTriangle ird;

	// Apply RenderTriangle3D values
	ird.material = tri->material;
	
	ird.a.pos_ws = tri->a.pos_worldSpace;
	ird.b.pos_ws = tri->b.pos_worldSpace;
	ird.c.pos_ws = tri->c.pos_worldSpace;

	ird.a.pos_uv = tri->a.pos_textureSpace;
	ird.b.pos_uv = tri->b.pos_textureSpace;
	ird.c.pos_uv = tri->c.pos_textureSpace;

	ird.a.normal = tri->a.normal;
	ird.b.normal = tri->b.normal;
	ird.c.normal = tri->c.normal;

	// Apply world matrix
	ird.a.pos_wsmx = ird.a.pos_ws * worldMatrix;
	ird.b.pos_wsmx = ird.b.pos_ws * worldMatrix;
	ird.c.pos_wsmx = ird.c.pos_ws * worldMatrix;

	// Project into clipping space
	ird.a.pos_cs = Vector4d(ird.a.pos_wsmx.x, ird.a.pos_wsmx.y, ird.a.pos_wsmx.z, 1.0) * projectionProperties.GetProjectionMatrix();
	ird.b.pos_cs = Vector4d(ird.b.pos_wsmx.x, ird.b.pos_wsmx.y, ird.b.pos_wsmx.z, 1.0) * projectionProperties.GetProjectionMatrix();
	ird.c.pos_cs = Vector4d(ird.c.pos_wsmx.x, ird.c.pos_wsmx.y, ird.c.pos_wsmx.z, 1.0) * projectionProperties.GetProjectionMatrix();

	// Clip triangle
	std::vector<InterRenderTriangle> clippingResults = ClippingEngine::Clip(ird);

	// Have we got any results? If not, abort, since the triangle has been dropped.
	if (clippingResults.size() == 0)
		return;

	// Now continue treating all of our fragments
	for (InterRenderTriangle& cird : clippingResults)
	{
		// Transform into normalized device coordinates (perform perspective division)
		cird.a.pos_ndc = cird.a.pos_cs / cird.a.pos_cs.w;
		cird.b.pos_ndc = cird.b.pos_cs / cird.b.pos_cs.w;
		cird.c.pos_ndc = cird.c.pos_cs / cird.c.pos_cs.w;

		// Transform into screen space coordinates
		const double halfResX = projectionProperties.GetHalfResolution().x;
		const double halfResY = projectionProperties.GetHalfResolution().y;

		cird.a.pos_ss.x = (cird.a.pos_ndc.x * halfResX) + halfResX;
		cird.a.pos_ss.y = (cird.a.pos_ndc.y * halfResY) + halfResY;
		cird.a.pos_ss.z = cird.a.pos_cs.z;

		cird.b.pos_ss.x = (cird.b.pos_ndc.x * halfResX) + halfResX;
		cird.b.pos_ss.y = (cird.b.pos_ndc.y * halfResY) + halfResY;
		cird.b.pos_ss.z = cird.b.pos_cs.z;

		cird.c.pos_ss.x = (cird.c.pos_ndc.x * halfResX) + halfResX;
		cird.c.pos_ss.y = (cird.c.pos_ndc.y * halfResY) + halfResY;
		cird.c.pos_ss.z = cird.c.pos_cs.z;

		// Calculate screen area (in pixels) taken up by triangle
		// Edgefunction(a, b, c);
		cird.ss_area = abs(((cird.c.pos_ss.x - cird.a.pos_ss.x) * (cird.b.pos_ss.y - cird.a.pos_ss.y) - (cird.c.pos_ss.y - cird.a.pos_ss.y) * (cird.b.pos_ss.x - cird.a.pos_ss.x)));
		cird.ss_iarea = 1.0 / cird.ss_area;
	}

	// Now that we have transformed all of our triangles into device space, let's now add our results to the results vector
	std::lock_guard<std::mutex> guard(resultVecMutex);

	for (InterRenderTriangle& cird : clippingResults)
		projectedTriangles.emplace_back(cird);

	return;
}

std::vector<InterRenderTriangle>& ProjectionEngine::Finish()
{
	return projectedTriangles;
}
