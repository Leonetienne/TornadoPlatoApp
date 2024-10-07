#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Test__Yubi : public TestFixture
{
public:
	Test__Yubi();

	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;

private:
	Components::Camera* camera;
};

