#pragma once
#include "Camera.h"
#include "MeshRenderer.h"
#include "LightSource.h"
#include "../Tornado/Tornado.h"
#include "../Tornado/WorkerPool.h"
#include <mutex>

namespace Plato
{
	/** Responsible to resolve renderables to RenderTriangle3D's.
	* This class is responsible for taking all renderable components, resolving meshes to absolute vertex coordinates,
	* putting it all in RenerTriangle3D's, and forwarding these to Tornado.
	*/
	class Renderer
	{
	public:
        // If numThreads is 0, std::thread::hardware_concurrency() will be used to determine
        // the max number of threads of the system automatically
		Renderer(const Vector2i& renderResolution, const std::size_t numThreads = 0, double globalIllumination = 0);

		void BeginFrame();
		void RegisterLightSource(const Components::LightSource* lr);
		void RegisterMeshRenderer(const Components::MeshRenderer* mr);
		void Render();

		const TorGL::PixelBuffer<3>* GetPixelBuffer() const;

	private:

		// Will translate plato light sources to tornado light sources
		void ResolveLightSources();

		// Will translate meshes (and their transforms) to camera-space render triangles
		void ResolveRenderTriangles();

		// Will resolve a single render triangle from a single mesh renderer
		void Thread__ResolveMeshRenderer_RenderTriangle(
			const Components::MeshRenderer* mr,
			const MeshVertexIndices* idx,
			std::size_t numTris
		);

		TorGL::WorkerPool workerPool;

		TorGL::Tornado tornado;
		std::vector<const Components::MeshRenderer*> meshRenderers;
		std::vector<const Components::LightSource*> lightSourceComponents;

		std::vector<TorGL::RenderTriangle3D> renderTriangles;
		std::vector<TorGL::RenderLightSource*> tornadoLightSources;

		Eule::Matrix4x4 worldMatrix;
		const Vector2i& renderResolution;
		Components::Camera* mainCamera;
		bool hasCamera = false;

		std::mutex vecMutex;

        // Benchmarking stuff
        #ifdef _BENCHMARK_CONTEXT
        double _benchmark_beginFrameTime;
        double _benchmark_resolveCameraSpaceVerticesTime;
        double _benchmark_registerTornadoObjects;
        double _benchmark_tornadoBeginFrameTime;
        double _benchmark_tornadoRenderTime;

        // Tornado carried metrics
        double _benchmark_tornado_perspectiveProjectionTime;
        double _benchmark_tornado_cullBackfacesTime;
        double _benchmark_tornado_drawTrianglesTime;

        public:
        const inline double _benchmark_GetBeginFrameTime() {
            return _benchmark_beginFrameTime;
        }
        const inline double _benchmark_GetResolveCameraSpaceVerticesTime() {
            return _benchmark_resolveCameraSpaceVerticesTime;
        }
        const inline double _benchmark_RegisterTornadoObjectsTime() {
            return _benchmark_registerTornadoObjects;
        }
        const inline double _benchmark_GetTornadoBeginFrameTime() {
            return _benchmark_tornadoBeginFrameTime;
        }
        const inline double _benchmark_GetTornadoRenderTime() {
            return _benchmark_tornadoRenderTime;
        }

        // Tornado carried metrics
        const inline double _benchmark_GetTornadoRenderPerspectiveProjectionTime() {
            return _benchmark_tornado_perspectiveProjectionTime;
        }
        const inline double _benchmark_GetTornadoRenderCullBackfacesTime() {
            return _benchmark_tornado_cullBackfacesTime;
        }
        const inline double _benchmark_GetTornadoRenderDrawTrianglesTime() {
            return _benchmark_tornado_drawTrianglesTime;
        }
        private:
        #endif
	};
}
