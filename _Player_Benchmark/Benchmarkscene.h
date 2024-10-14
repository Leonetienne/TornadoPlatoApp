#pragma once
#include "../Plato/MeshRenderer.h"
#include "../Plato/Material.h"
#include "../Plato/PointLight.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Benchmarkscene
{
public:
	Benchmarkscene();

	void Update(double deltaTime);
	void Render(Renderer* renderer);

private:
    std::vector<Eule::Vector3d> cameraWaypoints;
    std::size_t nextCameraWaypoint = 0;
    Components::Camera* camera;

    void SetNextCameraWaypoint();
};

