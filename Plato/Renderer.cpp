#include "Renderer.h"

Renderer::Renderer(const Vector2i& renderResolution)
	:
	renderResolution { renderResolution },
	tornado(renderResolution, 24),
	mainCamera { nullptr }
{
	
	worldMatrix[0] = { 1, 0, 0, 0 };
	worldMatrix[1] = { 0, 1, 0, 0 };
	worldMatrix[2] = { 0, 0, -1, 0 };
	worldMatrix[3] = { 0, 0, 0, 0 };

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
	ResolveRenderTriangles();

	tornado.BeginFrame();
	
	for (const RenderTriangle3D& rd : renderTriangles)
		tornado.RegisterRender(&rd);

	tornado.Render(mainCamera->GetProjectionProperties(), worldMatrix);

	return;
}

void Renderer::ResolveRenderTriangles()
{
	/// MULTITHREAD THIS!!
	const Vector3d inverseCameraPosition = mainCamera->transform->GetPosition() * -1;
	const Matrix4x4 inverseCameraRotation = mainCamera->transform->GetRotation().Inverse().ToRotationMatrix();

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

			rd.a.pos_worldSpace = mesh->v_vertices[mesh->tris[i].v + 0];
			rd.b.pos_worldSpace = mesh->v_vertices[mesh->tris[i].v + 1];
			rd.c.pos_worldSpace = mesh->v_vertices[mesh->tris[i].v + 2];

			rd.a.pos_textureSpace = mesh->uv_vertices[mesh->tris[i].uv + 0];
			rd.b.pos_textureSpace = mesh->uv_vertices[mesh->tris[i].uv + 1];
			rd.c.pos_textureSpace = mesh->uv_vertices[mesh->tris[i].uv + 2];

			rd.a.normal = mesh->normals[mesh->tris[i].vn + 0];
			rd.b.normal = mesh->normals[mesh->tris[i].vn + 1];
			rd.c.normal = mesh->normals[mesh->tris[i].vn + 2];

			// Apply world space transformation
			const Matrix4x4& transformationMatrix = mr->transform->GetLocalTransformationMatrix();;
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
