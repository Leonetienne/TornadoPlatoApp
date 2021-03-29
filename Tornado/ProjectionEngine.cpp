#include "ProjectionEngine.h"
#include "ClippingEngine.h"

ProjectionEngine::ProjectionEngine(WorkerPool* workerPool)
{
	this->workerPool = workerPool;

	return;
}

void ProjectionEngine::BeginBatch()
{
	registeredTriangles.clear();
	projectedTriangles.clear();

	return;
}

void ProjectionEngine::RegisterRenderTriangle(const RenderTriangle3D* triangle)
{
	registeredTriangles.push_back(triangle);
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
	
	ird.a.vertex_col = tri->a.vertexColor;
	ird.b.vertex_col = tri->b.vertexColor;
	ird.c.vertex_col = tri->c.vertexColor;


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
		const double halfResY = projectionProperties.GetHalfResolution().x;

		cird.a.pos_ss.x = (cird.a.pos_ndc.x * halfResX) + halfResX;
		cird.a.pos_ss.y = (cird.a.pos_ndc.y * halfResY) + halfResY;
		cird.a.pos_ss.z = cird.a.pos_cs.z;

		cird.b.pos_ss.x = (cird.b.pos_ndc.x * halfResX) + halfResX;
		cird.b.pos_ss.y = (cird.b.pos_ndc.y * halfResY) + halfResY;
		cird.b.pos_ss.z = cird.b.pos_cs.z;

		cird.c.pos_ss.x = (cird.c.pos_ndc.x * halfResX) + halfResX;
		cird.c.pos_ss.y = (cird.c.pos_ndc.y * halfResY) + halfResY;
		cird.c.pos_ss.z = cird.c.pos_cs.z;
	}

	// Now that we have projected all of our triangles into device space, let's now add our results to the results vector
	std::lock_guard<std::mutex> guard(resultVecMutex);

	for (InterRenderTriangle& cird : clippingResults)
		projectedTriangles.emplace_back(cird);

	return;
}

std::vector<InterRenderTriangle> ProjectionEngine::Finish()
{
	return projectedTriangles;
}
