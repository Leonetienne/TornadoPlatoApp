#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Test__MC : public TestFixture
{
public:
	Test__MC();

	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;

private:
};

