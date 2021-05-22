#pragma once
#include "Component.h"
#include "Mesh.h"
#include "../Tornado/Material.h"

class Renderer;

/** Component to render meshes. 
* This component will take a reference to a mesh and a material, and will automatically render it at the end of each frame.
*/
class MeshRenderer : public Component
{
public:
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();
	const Mesh* GetMesh() const;

	void SetMaterial(Material* material);
	Material* GetMaterial();
	const Material* GetMaterial() const;

	void Render(Renderer* renderer);

private:
	MeshRenderer(WorldObject* worldObject, Mesh* mesh, Material* material);

	Mesh* mesh;
	Material* material;

	friend class WorldObject;
};
