#pragma once
#include <Windows.h>
#include "../Plato/Component.h"

class Rotator : Component
{
public:
	void Update(double deltaTime)
	{
		if (!GetAsyncKeyState(VK_SPACE))
			transform->Rotate(Quaternion(Vector3d(
				0.5,
				1.0,
				2.0
			) * speed * deltaTime * 0.05));


		if (GetAsyncKeyState('K'))
			transform->SetRotation(Quaternion(Vector3d::zero));

		return;
	}

private:
	Rotator(WorldObject* worldObject, double speed)
		:
		Component(worldObject),
		speed { speed }
	{
		return;
	}
	double speed;

	friend WorldObject;
};
