#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"
#include "../Plato/PointLight.h"

class Test__Lighting__PointLight : public TestFixture
{
public:
	Test__Lighting__PointLight();

	void Update(double deltaTime) override;

private:
	void UpdateSoftnessPulse(double deltaTime);
	void UpdateColorCycle(double deltaTime);

	PointLight* rgbLight;
};
