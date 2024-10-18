#pragma once
#include "../../BenchmarkScene.h"
#include "../../../Plato/MeshRenderer.h"
#include "../../../Plato/Material.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/Camera.h"

class CaveCamFlightScene : public BenchmarkScene
{
public:
    CaveCamFlightScene();
    void Setup() override;
	void Update(double deltaTime) override;

private:
    std::vector<Eule::Vector3d> cameraWaypoints;
    std::size_t nextCameraWaypoint = 0;

    void SetNextCameraWaypoint();
};

