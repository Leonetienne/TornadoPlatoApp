#pragma once
#include <Windows.h>
#include "../Plato/Component.h"

class Rotator : Component
{
public:
	void Update(double)
	{
		if (!GetAsyncKeyState(VK_SPACE))
			transform->Rotate(Quaternion(Vector3d(
				0.5 * speed,
				1.0 * speed,
				2.0 * speed
			)));


		if (GetAsyncKeyState('K'))
			transform->SetRotation(Quaternion(Vector3d::zero));

		return;
	}

private:
	Rotator(WorldObject* worldObject, double speed)
		:
		Component(worldObject)
	{
		this->speed = speed;
	}
	double speed;

	friend WorldObject;
};
