#pragma once
#include "../Plato/Component.h"
#include "../Plato/Camera.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Application.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Mouse.h"
#include <algorithm>

using namespace Plato;

/**
    Rotation control for the object this component is attached to.
    LJ for Z, -Z
    IK for X, -X
    UO for Y, -Y
* */
class Rotator : public Component
{
public:
	void Update(double deltaTime)
	{

        if (Input::Keyboard::GetKey(Input::KEY_CODE::I)) {
            speedFac.x += 0.005 * deltaTime;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::K)) {
            speedFac.x -=  0.005 * deltaTime;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::L)) {
            speedFac.z += 0.005 * deltaTime;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::J)) {
            speedFac.z -=  0.005 * deltaTime;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::U)) {
            speedFac.y += 0.005 * deltaTime;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::O)) {
            speedFac.y -=  0.005 * deltaTime;
        }

        if (Input::Keyboard::GetKey(Input::KEY_CODE::SPACE)) {
            speedFac = Vector3d::zero;
        }

        this->transform->Rotate(Vector3d(
            0.025 * speedFac.x * deltaTime,
            0.025 * speedFac.y * deltaTime,
            0.025 * speedFac.z * deltaTime
        ));

		return;
	}

private:
    Vector3d speedFac = Vector3d::zero;


private:
    Rotator(WorldObject* worldObject):
		Component(worldObject)
    {
	}

	friend WorldObject;
};

