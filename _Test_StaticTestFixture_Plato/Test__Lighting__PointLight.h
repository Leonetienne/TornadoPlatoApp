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
	void Render(Renderer*) override;

private:
	void SetGunAimed();
	void SetGunHip();

	Vector3d gunHolderPos_hip;
	Quaternion gunHolderRot_hip;

	Vector3d gunHolderPos_aim;
	Quaternion gunHolderRot_aim;

	bool isAimed = false;

	double fovAim = 60;
	double fovHip = 80;
	double targetFov = fovHip;
	double deltaTime = 0;

	WorldObject* gunHolder;
	WorldObject* gun;
	Transform* trCamera;
	Camera* camera;

	PointLight* pointLight;
};
