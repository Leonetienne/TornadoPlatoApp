#pragma once
#include "../Plato/Scene.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"

class Test__TransformHierarchy__ClearParent : public Plato::Scene
{
public:
	Test__TransformHierarchy__ClearParent();

	void Update(double frametime);

private:
    Plato::Transform* CreateLink(Plato::Transform* parent);

    Plato::WorldObject* wo_coob;

    Plato::Transform* jointRoot;
};
