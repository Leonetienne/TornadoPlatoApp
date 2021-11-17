#include "Renderer.h"

using namespace Plato;
using namespace Plato::Components;
using namespace TorGL;

Renderer::Renderer(const Vector2i& renderResolution)
	:
	renderResolution { renderResolution },
	tornado(renderResolution, 24),
	mainCamera { nullptr },
	workerPool(24)
{
	
	worldMatrix[0] = {  1,  0,  0, 0 };
	worldMatrix[1] = {  0,  1,  0, 0 };
	worldMatrix[2] = {  0,  0,  1, 0 };
	worldMatrix[3] = {  0,  0,  0, 1 };

	return;
}


void Renderer::BeginFrame()
{
	mainCamera = Camera::GetMainCamera();
	hasCamera = mainCamera != nullptr;

	if (hasCamera)
		mainCamera->SetRenderResolution(renderResolution);

	meshRenderers.clear();
	lightSourceComponents.clear();

	renderTriangles.clear();
	renderTriangles.reserve(100); // Should be a few

	tornadoLightSources.clear();
	tornadoLightSources.reserve(10);

	return;
}

void Renderer::RegisterLightSource(const LightSource* lr)
{
	lightSourceComponents.emplace_back(lr);
	return;
}

void Renderer::RegisterMeshRenderer(const MeshRenderer* mr)
{
	meshRenderers.emplace_back(mr);
	return;
}

void Renderer::Render()
{
	// Don't render without a camera!
	if (!hasCamera)
		return;

	tornado.BeginFrame();
	
	ResolveRenderTriangles();
	ResolveLightSources();
	
	// Register light sources
	for (const RenderLightSource* lr : tornadoLightSources)
		tornado.RegisterRender(lr);

	// Register render triangles
	for (const RenderTriangle3D& rd : renderTriangles)
		tornado.RegisterRender(&rd);
	
	tornado.Render(mainCamera->GetProjectionProperties(), worldMatrix);

	return;
}

void Renderer::ResolveLightSources()
{
	for (const LightSource* ls : lightSourceComponents)
	{
		// Fetch tornado light source
		RenderLightSource* rls = ls->GetRawTornadoRenderLightSource();
		
		// Transform to camera space
		// These are references!

		tornadoLightSources.emplace_back(rls);
	}
}

void Renderer::ResolveRenderTriangles()
{
	// Create resolving tasks
	// Convert mesh renderers to individual world space triangles
	for (const MeshRenderer* mr : meshRenderers)
	{
		const Mesh* mesh = mr->GetMesh();

		// Number of triangle vertex indices not a multiple of 3
		if (mesh->tris.size() % 3 != 0)
			throw std::runtime_error("Mesh tris.size() is broken!");
	
		for (std::size_t i = 0; i < mesh->tris.size();)
		{
			WorkerTask* task = new WorkerTask(); // Will be freed by the workerPool

			// Compute how many triangles to compute per task (mutex lock overhead)
			const std::size_t optimalNumTrianglesForThisTask = 16;

			// Either optimalNumTrianglesForThisTask, or what's left to not go out_of_range
			const std::size_t numTrianglesForThisTask = 
				((mesh->tris.size()/3 - i/3) >= optimalNumTrianglesForThisTask) ?
					optimalNumTrianglesForThisTask
					:
					mesh->tris.size()/3 - i/3;

			// Create task
			task->task = std::bind(&Renderer::Thread__ResolveMeshRenderer_RenderTriangle, this,
				mr,
				&mesh->tris[i],
				numTrianglesForThisTask
			);

			// Queue task
			workerPool.QueueTask(task);
			
			// Advance loop
			i += numTrianglesForThisTask * 3;
		}
	}

	// Compute tasks
	workerPool.Execute();

	return;
}

void Renderer::Thread__ResolveMeshRenderer_RenderTriangle(
	const MeshRenderer* mr,
	const MeshVertexIndices* idx,
	std::size_t numTris
)
{
	const Mesh* mesh = mr->GetMesh();


	// Cache the results of this iteration, to save on mutex operations
	std::vector<RenderTriangle3D> resultCache;
	resultCache.reserve(numTris);

	for (std::size_t i = 0; i < numTris; i++)
	{
		RenderTriangle3D rd;
		rd.material = mr->GetMaterial();

		// Transform vertices from object space to camera space
		rd.a.pos_worldSpace = 
			mainCamera->WorldSpaceToCameraSpace(
				mr->transform->ObjectSpaceToWorldSpace(mesh->v_vertices[idx[i*3 + 0].v])
			);

		rd.b.pos_worldSpace = 
			mainCamera->WorldSpaceToCameraSpace(
				mr->transform->ObjectSpaceToWorldSpace(mesh->v_vertices[idx[i*3 + 1].v])
			);

		rd.c.pos_worldSpace = 
			mainCamera->WorldSpaceToCameraSpace(
				mr->transform->ObjectSpaceToWorldSpace(mesh->v_vertices[idx[i*3 + 2].v])
			);


		// Texture space can stay as is
		rd.a.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 0].uv];
		rd.b.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 1].uv];
		rd.c.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 2].uv];

		rd.a.normal = mesh->normals[idx[i*3 + 0].vn];
		rd.b.normal = mesh->normals[idx[i*3 + 1].vn];
		rd.c.normal = mesh->normals[idx[i*3 + 2].vn];


		// Apply object- and camera rotation to the vertex normals
		const Matrix4x4 normalTransMat = mr->transform->GetGlobalTransformationMatrix().DropTranslationComponents() * mainCamera->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
		rd.a.normal *= normalTransMat;
		rd.b.normal *= normalTransMat;
		rd.c.normal *= normalTransMat;
		rd.a.normal.NormalizeSelf();
		rd.b.normal.NormalizeSelf();
		rd.c.normal.NormalizeSelf();

		// Add to local result cache
		resultCache.emplace_back(std::move(rd));
	}

	// After having calculated all iterations, add them all at once to the vector
	std::lock_guard<std::mutex> lock(vecMutex);
	renderTriangles.insert(renderTriangles.end(), resultCache.begin(), resultCache.end());

	return;
}

const PixelBuffer<3>* Renderer::GetPixelBuffer() const
{
	return tornado.GetPixelBuffer();
}
