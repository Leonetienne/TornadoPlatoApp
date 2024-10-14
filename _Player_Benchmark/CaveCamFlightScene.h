#pragma once
#include "BenchmarkScene.h"
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/PointLight.h"
#include "../Plato/Camera.h"

using namespace Plato;

class CaveCamFlightScene : public BenchmarkScene
{
public:
    void Setup() override;
	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;
	void Teardown() override;
    std::string GetName() const override;

private:
    std::vector<Eule::Vector3d> cameraWaypoints;
    std::size_t nextCameraWaypoint = 0;

    void SetNextCameraWaypoint();
};

