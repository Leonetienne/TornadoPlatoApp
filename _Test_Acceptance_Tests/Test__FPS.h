#pragma once
#include "TestFixture.h"
#include "../Plato/MeshRenderer.h"
#include "../Tornado/Material.h"
#include "../Plato/Camera.h"

using namespace Plato;

class Test__FPS : public TestFixture
{
public:
	Test__FPS();

	void Update(double deltaTime) override;
	void Render(Renderer* renderer) override;

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
	Quaternion targetRot = gunHolderRot_hip;
	Vector3d targetPos = gunHolderPos_hip;
	double deltaTime = 0;

	WorldObject* gunHolder;
	WorldObject* gun;
	Transform* trCamera;
	Components::Camera* camera;
};
