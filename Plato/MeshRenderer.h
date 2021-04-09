#pragma once
#include "Component.h"
#include "Mesh.h"
#include "../Tornado/Material.h"

class MeshRenderer : public Component
{
public:
	void SetMesh(const Mesh* mesh);
	const Mesh* GetMesh() const;

	void SetMaterial(const Material* material);
	const Material* GetMaterial() const;

private:
	MeshRenderer(WorldObject* worldObject, const Mesh* mesh, const Material* material);

	const Mesh* mesh;
	const Material* material;

	friend class WorldObject;
};
