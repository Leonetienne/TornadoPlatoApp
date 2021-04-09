#include "MeshRenderer.h"
#include "Renderer.h"

MeshRenderer::MeshRenderer(WorldObject* worldObject, const Mesh* mesh, const Material* material)
	:
	Component(worldObject)
{
	this->mesh = mesh;
	this->material = material;

	return;
}

void MeshRenderer::SetMesh(const Mesh* mesh)
{
	this->mesh = mesh;
	return;
}

const Mesh* MeshRenderer::GetMesh() const
{
	return mesh;
}

void MeshRenderer::SetMaterial(const Material* material)
{
	this->material = material;
	return;
}

const Material* MeshRenderer::GetMaterial() const
{
	return material;
}

void MeshRenderer::Render(Renderer* renderer)
{
	renderer->RegisterMeshRenderer(this);
	return;
}
