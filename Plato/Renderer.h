#pragma once
#include "../Tornado/Tornado.h"
#include "Camera.h"
#include "MeshRenderer.h"

class Renderer
{
public:
	Renderer(const Vector2i& renderResolution);

	void SetMainCamera(Camera* camera);
	const Camera* GetMainCamera() const;

	void BeginFrame();
	void RegisterMeshRenderer(const MeshRenderer* mr);
	void Render();

	const PixelBuffer<3>* GetPixelBuffer() const;

private:
	// Will translate meshes (and their transforms) to camera-space render triangles
	void ResolveRenderTriangles();

	Tornado tornado;
	std::vector<const MeshRenderer*> meshRenderers;
	std::vector<RenderTriangle3D> renderTriangles;
	Matrix4x4 worldMatrix;
	const Vector2i& renderResolution;
	const Camera* mainCamera;
};
