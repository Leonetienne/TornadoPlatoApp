#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Test__Cube : public TestFixture
{
public:
	Test__Cube();

	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;

private:
    double speedFac = 1.5;
	Components::Camera* camera;
    WorldObject* cube;
    WorldObject* light;
};

