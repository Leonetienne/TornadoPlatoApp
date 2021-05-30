#pragma once
#include <Windows.h>
#include "../Plato/Component.h"

using namespace Plato;

class Rotator : Component
{
public:
	void Update(double deltaTime)
	{
		if (!GetAsyncKeyState(VK_SPACE))
			transform->Rotate(Quaternion(speed * deltaTime * 0.05));


		if (GetAsyncKeyState('K'))
			transform->SetRotation(Quaternion(Vector3d::zero));

		return;
	}

private:
	Rotator(WorldObject* worldObject, const Vector3d& speed)
		:
		Component(worldObject),
		speed { speed }
	{
		return;
	}

	Rotator(WorldObject* worldObject, double speed)
		:
		Component(worldObject),
		speed { Vector3d(speed * 0.5, speed * 1.0, speed * 2.0) }
	{
		return;
	}

	Vector3d speed;

	friend WorldObject;
};
