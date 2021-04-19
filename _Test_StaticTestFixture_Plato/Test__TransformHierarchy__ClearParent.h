#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"

class Test__TransformHierarchy__ClearParent : public TestFixture
{
public:
	Test__TransformHierarchy__ClearParent();

	void Update(double frametime);

private:
	Transform* CreateLink(Transform* parent);

	Mesh mesh_coob;
	WorldObject* wo_coob;
	Texture txt_coob;
	Material mat_coob;

	Transform* jointRoot;
};
