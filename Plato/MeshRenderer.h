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
	void SetMesh(const Mesh* mesh);
	const Mesh* GetMesh() const;

	void SetMaterial(const Material* material);
	const Material* GetMaterial() const;

	void Render(Renderer* renderer);

private:
	MeshRenderer(WorldObject* worldObject, const Mesh* mesh, const Material* material);

	const Mesh* mesh;
	const Material* material;

	friend class WorldObject;
};
