#include "Renderer.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshRenderer.h"

using namespace Plato;
using namespace Plato::Components;
using namespace TorGL;

#ifdef _BENCHMARK_CONTEXT
// If we are in a benchmarking context, create a timer used to capture runtimes of inidividual methods
#include "Clock.h"
namespace {
    Plato::Clock perfTimer;
}
#endif

Renderer::Renderer(
    const Vector2i& renderResolution,
    std::size_t numThreads,
    double globalIllumination,
    Components::Camera const* camera
):
	workerPool(numThreads),
	tornado(renderResolution, numThreads, globalIllumination),
	renderResolution { renderResolution },
	camera { camera }
{
	
	worldMatrix[0] = {  1,  0,  0, 0 };
	worldMatrix[1] = {  0,  1,  0, 0 };
	worldMatrix[2] = {  0,  0,  1, 0 };
	worldMatrix[3] = {  0,  0,  0, 1 };

	return;
}


void Renderer::BeginFrame()
{
    #ifdef _BENCHMARK_CONTEXT
        perfTimer.Reset();
    #endif

    // If no camera is explicitly set, fetch the main camera
    if (camera == nullptr) {
        SetCamera(Camera::GetMainCamera());

        // If the camera is still null, error out
        if (camera == nullptr) {
            throw std::runtime_error("Attempted to initialize a renderer (BeginFrame()) without having a camera set, or a camera with id 'main_camera' in scene!");
        }
    }

	meshRenderers.clear();
	lightSourceComponents.clear();

	renderTriangles.clear();
	renderTriangles.reserve(100); // Should be a few

	tornadoLightSources.clear();
	tornadoLightSources.reserve(10);

    #ifdef _BENCHMARK_CONTEXT
        _benchmark_beginFrameTime = perfTimer.GetElapsedTime().AsMilliseconds();
    #endif

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
    // If the camera is null, error out
    if (camera == nullptr) {
        throw std::runtime_error("Attempted to perform a render (Render()) without having a camera set, or a camera with id 'main_camera' in scene!");
    }

    // TORNADO BEGIN FRAME
    #ifdef _BENCHMARK_CONTEXT
        perfTimer.Reset();
    #endif
	tornado.BeginFrame();
    #ifdef _BENCHMARK_CONTEXT
        _benchmark_tornadoBeginFrameTime = perfTimer.GetElapsedTime().AsMilliseconds();
    #endif
	
    // RESOLVE CAMERA SPACE VERTICES
    #ifdef _BENCHMARK_CONTEXT
        perfTimer.Reset();
    #endif
	ResolveRenderTriangles();
	ResolveLightSources();
    #ifdef _BENCHMARK_CONTEXT
        _benchmark_resolveCameraSpaceVerticesTime = perfTimer.GetElapsedTime().AsMilliseconds();
    #endif
	
	// TORNADO REGISTER OBJECTS
    #ifdef _BENCHMARK_CONTEXT
        perfTimer.Reset();
    #endif
	for (const RenderLightSource* lr : tornadoLightSources) {
		tornado.RegisterRender(lr);
    }
	// Register render triangles
	for (const RenderTriangle3D& rd : renderTriangles) {
		tornado.RegisterRender(&rd);
    }
    #ifdef _BENCHMARK_CONTEXT
        _benchmark_registerTornadoObjects = perfTimer.GetElapsedTime().AsMilliseconds();
    #endif
	
    // TORNADO RENDER
    #ifdef _BENCHMARK_CONTEXT
        perfTimer.Reset();
    #endif
    ProjectionProperties projectionProperties = camera->GetProjectionProperties();
    projectionProperties.SetResolution(renderResolution);
	tornado.Render(projectionProperties, worldMatrix);
    #ifdef _BENCHMARK_CONTEXT
        _benchmark_tornadoRenderTime = perfTimer.GetElapsedTime().AsMilliseconds();
    #endif

    // If benchmarking, carry over tornado metrics
    #ifdef _BENCHMARK_CONTEXT
    _benchmark_tornado_perspectiveProjectionTime = tornado._benchmark_GetPerspectiveProjectionTime();
    _benchmark_tornado_cullBackfacesTime = tornado._benchmark_GetCullBackfacesTime();
    _benchmark_tornado_drawTrianglesTime = tornado._benchmark_GetDrawTrianglesTime();
    #endif

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

    // Index of the first triangle this thread is processing
    std::size_t baseTriangleIndex = idx - mesh->tris.data();

	for (std::size_t i = 0; i < numTris; i++)
	{
		RenderTriangle3D rd;
        
        // Does our mesh define a material for this specific face?
        if (auto trisMatPair = mesh->trisMaterialIndices.find(baseTriangleIndex + i * 3); trisMatPair != mesh->trisMaterialIndices.end()) {
            // Yes, a material is defined for this face...
            // Set it.
            rd.material = trisMatPair->second;
        }
        else {
            // No, no material is defined for this face...
            // Use the mesh renderer material instead...
            rd.material = mr->GetMaterial();
        }

		// Transform vertices from object space to camera space
		rd.a.pos_worldSpace = 
			camera->WorldSpaceToCameraSpace(
				mr->transform->ObjectSpaceToWorldSpace(mesh->v_vertices[idx[i*3 + 0].v])
			);

		rd.b.pos_worldSpace = 
			camera->WorldSpaceToCameraSpace(
				mr->transform->ObjectSpaceToWorldSpace(mesh->v_vertices[idx[i*3 + 1].v])
			);

		rd.c.pos_worldSpace = 
			camera->WorldSpaceToCameraSpace(
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
		const Matrix4x4 normalTransMat = mr->transform->GetGlobalTransformationMatrix().DropTranslationComponents() * camera->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
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

std::size_t Renderer::GetNumActiveVertices() const
{
    std::size_t numVertices = 0;
    for (const MeshRenderer* mr : meshRenderers) {
        numVertices += mr->GetMesh()->v_vertices.size();
    }
    return numVertices;
}

std::size_t Renderer::GetNumActiveTris() const
{
    std::size_t numTris = 0;
    for (const MeshRenderer* mr : meshRenderers) {
        numTris += mr->GetMesh()->tris.size();
    }
    return numTris;
}

const PixelBuffer<3>* Renderer::GetPixelBuffer() const
{
	return tornado.GetPixelBuffer();
}

Camera const* Renderer::GetCamera() const
{
    return camera;
}

void Renderer::SetCamera(Camera const* camera)
{
    this->camera = camera;
}

