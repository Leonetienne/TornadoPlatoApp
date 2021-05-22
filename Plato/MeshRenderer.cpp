#include "MeshRenderer.h"
#include "Renderer.h"

MeshRenderer::MeshRenderer(WorldObject* worldObject, Mesh* mesh, Material* material)
	:
	Component(worldObject)
{
	this->mesh = mesh;
	this->material = material;

	return;
}

void MeshRenderer::SetMesh(Mesh* mesh)
{
	this->mesh = mesh;
	return;
}

Mesh* MeshRenderer::GetMesh()
{
	return mesh;
}

const Mesh* MeshRenderer::GetMesh() const
{
	return mesh;
}

void MeshRenderer::SetMaterial(Material* material)
{
	this->material = material;
	return;
}

Material* MeshRenderer::GetMaterial()
{
	return material;
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
