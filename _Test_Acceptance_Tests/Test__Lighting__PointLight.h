#pragma once
#include "../Plato/Scene.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"
#include "../Plato/PointLight.h"

using namespace Plato;

class Test__Lighting__PointLight : public Scene
{
public:
	Test__Lighting__PointLight();

	void Update(double deltaTime) override;

private:
	void UpdateColorCycle(double deltaTime);

	Components::PointLight* rgbLight;
	Transform* rgbParent;
	WorldObject* woPlane;
};
