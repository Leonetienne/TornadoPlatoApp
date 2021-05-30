#pragma once
#include "../Tornado/Tornado.h"
#include "../Tornado/WorkerPool.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LightSource.h"
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
		Renderer(const Vector2i& renderResolution);

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
	};
}
