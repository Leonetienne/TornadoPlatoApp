#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/PointLight.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Test__Benchmarkscene : public TestFixture
{
public:
	Test__Benchmarkscene();

	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;

private:
    std::vector<Eule::Vector3d> cameraWaypoints;
    std::size_t nextCameraWaypoint = 0;
    Components::Camera* camera;

    void SetNextCameraWaypoint();
};
