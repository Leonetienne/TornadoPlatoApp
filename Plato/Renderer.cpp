#include "Renderer.h"

Renderer::Renderer(const Vector2i& renderResolution)
	:
	renderResolution { renderResolution },
	tornado(renderResolution, 24),
	mainCamera { nullptr },
	workerPool(24)
{
	
	worldMatrix[0] = { 1, 0, 0, 0 };
	worldMatrix[1] = { 0, 1, 0, 0 };
	worldMatrix[2] = { 0, 0, 1, 0 };
	worldMatrix[3] = { 0, 0, 0, 1 };

	return;
}

void Renderer::SetMainCamera(Camera* camera)
{
	camera->SetRenderResolution(renderResolution);
	this->mainCamera = camera;
	return;
}

const Camera* Renderer::GetMainCamera() const
{
	return mainCamera;
}

void Renderer::BeginFrame()
{
	meshRenderers.clear();
	renderTriangles.clear();
	renderTriangles.reserve(100); // Should be a few

	return;
}

void Renderer::RegisterMeshRenderer(const MeshRenderer* mr)
{
	meshRenderers.push_back(mr);
	return;
}

void Renderer::Render()
{
	tornado.BeginFrame();
	
	ResolveRenderTriangles();
	
	for (const RenderTriangle3D& rd : renderTriangles)
		tornado.RegisterRender(&rd);
	
	tornado.Render(mainCamera->GetProjectionProperties(), worldMatrix);

	return;
}

void Renderer::ResolveRenderTriangles()
{
	const Vector3d inverseCameraPosition = mainCamera->transform->GetGlobalPosition() * -1;
	const Matrix4x4 inverseCameraRotation = mainCamera->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
	

	// Create resolving tasks
	// Convert mesh renderers to individual world space triangles
	for (const MeshRenderer* mr : meshRenderers)
	{
		const Mesh* mesh = mr->GetMesh();

		// Number of triangle vertex indices not a multiple of 3
		if (mesh->tris.size() % 3 != 0)
			throw std::exception("Mesh tris.size() is broken!");
	
		for (std::size_t i = 0; i < mesh->tris.size();)
		{
			WorkerTask* task = new WorkerTask(); // Will be freed by the workerPool

			// Compute how many triangles to compute per task (mutex lock overhead)
			const std::size_t optimalNumTrianglesForThisTask = 125;

			// Either optimalNumTrianglesForThisTask, or what's left to not go out_of_range
			const std::size_t numTrianglesForThisTask = 
				((mesh->tris.size()/3 - i/3) >= optimalNumTrianglesForThisTask) ?
					optimalNumTrianglesForThisTask
					:
					mesh->tris.size()/3 - i/3;

			// Create task
			task->task = std::bind(&Renderer::Thread__ResolveMeshRenderer_RenderTriangle, this,
				inverseCameraPosition,
				inverseCameraRotation,
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
	const Vector3d inverseCameraPosition,
	const Matrix4x4 inverseCameraRotation,
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


		rd.a.pos_worldSpace = mesh->v_vertices[idx[i*3 + 0].v];
		rd.b.pos_worldSpace = mesh->v_vertices[idx[i*3 + 1].v];
		rd.c.pos_worldSpace = mesh->v_vertices[idx[i*3 + 2].v];

		rd.a.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 0].uv];
		rd.b.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 1].uv];
		rd.c.pos_textureSpace = mesh->uv_vertices[idx[i*3 + 2].uv];

		rd.a.normal = mesh->normals[idx[i*3 + 0].vn];
		rd.b.normal = mesh->normals[idx[i*3 + 1].vn];
		rd.c.normal = mesh->normals[idx[i*3 + 2].vn];

		// Apply world space transformation
		const Matrix4x4& transformationMatrix = mr->transform->GetGlobalTransformationMatrix(); // <-- This caches its return vales
		rd.a.pos_worldSpace *= transformationMatrix;
		rd.b.pos_worldSpace *= transformationMatrix;
		rd.c.pos_worldSpace *= transformationMatrix;

		// Apply camera space transformation
		rd.a.pos_worldSpace += inverseCameraPosition;
		rd.b.pos_worldSpace += inverseCameraPosition;
		rd.c.pos_worldSpace += inverseCameraPosition;
		rd.a.pos_worldSpace *= inverseCameraRotation;
		rd.b.pos_worldSpace *= inverseCameraRotation;
		rd.c.pos_worldSpace *= inverseCameraRotation;

		// Apply object rotation to the vertex normals
		const Matrix4x4 objectRotation = mr->transform->GetGlobalRotation().ToRotationMatrix();
		rd.a.normal *= objectRotation;
		rd.b.normal *= objectRotation;
		rd.c.normal *= objectRotation;

		// Testing
		rd.a.vertexColor = Color::red;
		rd.b.vertexColor = Color::green;
		rd.c.vertexColor = Color::blue;

		// Add to local result cache
		resultCache.emplace_back(std::move(rd));
	}

	// After having calculated all iterations, add them all at once to the vector
	std::unique_lock<std::mutex> lock(vecMutex);
	renderTriangles.insert(renderTriangles.end(), resultCache.begin(), resultCache.end());

	return;
}

const PixelBuffer<3>* Renderer::GetPixelBuffer() const
{
	return tornado.GetPixelBuffer();
}
