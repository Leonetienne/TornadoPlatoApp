#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"

class Test__FPS : public TestFixture
{
public:
	Test__FPS();

	void Update(double deltaTime);

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

	WorldObject* gunHolder;
	WorldObject* gun;
	Transform* trCamera;
	Camera* camera;
};
