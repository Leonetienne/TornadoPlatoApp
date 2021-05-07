#include "Renderer.h"

Renderer::Renderer(const Vector2i& renderResolution)
	:
	renderResolution { renderResolution },
	tornado(renderResolution, 24),
	mainCamera { nullptr }
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
	/// MULTITHREAD THIS!!
	const Vector3d inverseCameraPosition = mainCamera->transform->GetGlobalPosition() * -1;
	const Matrix4x4 inverseCameraRotation = mainCamera->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
	
	// Convert mesh renderers to individual world space triangles
	for (const MeshRenderer* mr : meshRenderers)
	{
		const Mesh* mesh = mr->GetMesh();
	
		// Number of triangle vertex indices not a multiple of 3
		if (mesh->tris.size() % 3 != 0)
			throw std::exception("Mesh tris.size() is broken!");
	
		for (std::size_t i = 0; i < mesh->tris.size(); i += 3)
		{
			// Assign local values
			RenderTriangle3D rd;
			rd.material = mr->GetMaterial();
	
			rd.a.pos_worldSpace = mesh->v_vertices[mesh->tris[i + 0].v];
			rd.b.pos_worldSpace = mesh->v_vertices[mesh->tris[i + 1].v];
			rd.c.pos_worldSpace = mesh->v_vertices[mesh->tris[i + 2].v];
	
			rd.a.pos_textureSpace = mesh->uv_vertices[mesh->tris[i + 0].uv];
			rd.b.pos_textureSpace = mesh->uv_vertices[mesh->tris[i + 1].uv];
			rd.c.pos_textureSpace = mesh->uv_vertices[mesh->tris[i + 2].uv];
	
			rd.a.normal = mesh->normals[mesh->tris[i + 0].vn];
			rd.b.normal = mesh->normals[mesh->tris[i + 1].vn];
			rd.c.normal = mesh->normals[mesh->tris[i + 2].vn];
	
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
	
			// Add RenderTriangle to vector
			renderTriangles.push_back(rd);
		}
	}

	return;
}

const PixelBuffer<3>* Renderer::GetPixelBuffer() const
{
	return tornado.GetPixelBuffer();
}
