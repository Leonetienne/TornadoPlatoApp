#pragma once
#include "../Plato/Scene.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"

class Test__TransformHierarchy__ChangeParent : public Plato::Scene
{
public:
	Test__TransformHierarchy__ChangeParent();

	void Update(double frametime);

private:
    Plato::Transform* CreateLink(Plato::Transform* parent);

    Plato::WorldObject* wo_coob;

    Plato::Transform* jointRoot;
	Plato::Transform* newParent;
};
