#pragma once
#include "../../../Plato/Scene.h"
#include "../../../Plato/PointLight.h"
#include "../../../Plato/Camera.h"
#include "../../../Plato/WorldObject.h"

class MC_FurnaceScene : public Plato::Scene
{
public:
	MC_FurnaceScene();

private:
    double speedFac = 1.5;
    Plato::Components::Camera* camera;
    Plato::WorldObject* cube;
    Plato::WorldObject* light;
};

