#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"

class Test__TransformHierarchy__Curl : public TestFixture
{
public:
	Test__TransformHierarchy__Curl();

	void Update(double frametime);

private:
	Transform* CreateLink(Transform* parent);

	WorldObject* wo_coob;

	Transform* jointRoot;
};
