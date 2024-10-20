#pragma once
#include "../Plato/Component.h"
#include "../Plato/Camera.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Application.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Mouse.h"
#include "../Keybinds.h"
#include <algorithm>
#include <iostream>

using namespace Plato;

/**
    Exclusive keyboard controls for camera movement. No mouse!
    WASDQE for movement, HJKL for looking.
    1 and 2 for FOV
    Press Z to dump the camera position and rotation.
* */
class CameraExclKeyboardControl : public Component
{
public:
	void Update(double deltaTime)
	{
		shiftFactor = Input::Keyboard::GetKey(KB_FPSCAM_SPRINT) ? shiftModifier : 1;

		MovementControl(deltaTime);
		ViewControl(deltaTime);
		AdditionalControls(deltaTime);

        if (Input::Keyboard::GetKeyDown(KB_FPSCAM_DUMP_COORDS)) {
            std::cout << "CAMERA POSITION: " << transform->GetGlobalPosition() << std::endl;
            std::cout << "CAMERA ROTATION: " << transform->GetGlobalRotation().ToEulerAngles() << std::endl;
        }

		return;
	}

private:
	const double internalMultiplier = 1 * 0.03;
	const double viewSensitivity = 3.3 * 0.06;

	void MovementControl(double deltaTime)
	{
        if (Input::Keyboard::GetKey(KB_FPSCAM_STRAFE_LEFT))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::left * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
        if (Input::Keyboard::GetKey(KB_FPSCAM_STRAFE_RIGHT))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::right * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

        if (Input::Keyboard::GetKey(KB_FPSCAM_FORWARD))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::forward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
        if (Input::Keyboard::GetKey(KB_FPSCAM_BACKWARD))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::backward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

        if (Input::Keyboard::GetKey(KB_FPSCAM_DOWN))
			camera_yPivot->Move(Vector3d::down * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
        if (Input::Keyboard::GetKey(KB_FPSCAM_UP))
			camera_yPivot->Move(Vector3d::up * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

	void ViewControl(double deltaTime)
	{
		// Use mouse for all camera movement
        Vector2d emulatedDeltaMouse(0, 0);

        if (Input::Keyboard::GetKey(KB_FPSCAM_LOOK_UP)) {
            emulatedDeltaMouse.y -= 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(KB_FPSCAM_LOOK_DOWN)) {
            emulatedDeltaMouse.y += 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(KB_FPSCAM_LOOK_LEFT)) {
            emulatedDeltaMouse.x -= 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(KB_FPSCAM_LOOK_RIGHT)) {
            emulatedDeltaMouse.x += 100.0 * viewSensitivity;
        }

		// Handle y rotation
		const double mx = (emulatedDeltaMouse.x / 100.0) * lookingSpeed;
		camera_yPivot->Rotate(Quaternion(Vector3d(0, mx * deltaTime, 0)));

		// Handle x rotation
		const double my = (emulatedDeltaMouse.y / 100.0) * lookingSpeed;
		static double totalXRot = 0;
		totalXRot += my * deltaTime;
		
		// now clamp xrot to -89.5 to 89.5
		totalXRot = std::max<double>(std::min<double>(totalXRot, 89.5), -89.5);

		// Now hard-set this x rot
		camera->SetRotation(Quaternion(Vector3d(totalXRot, 0, 0)));

		return;
	}

	void AdditionalControls(double deltaTime)
	{
		// Fov keys 1, and 2
        if (Input::Keyboard::GetKey(KB_FPSCAM_INCREASE_FOV)) {
            cameraComponent->SetFov(cameraComponent->GetFov() + 0.1 * deltaTime);
        }
        if (Input::Keyboard::GetKey(KB_FPSCAM_DECREASE_FOV)) {
            cameraComponent->SetFov(cameraComponent->GetFov() - 0.1 * deltaTime);
        }
		const double mouseDelta = Input::Mouse::GetMousewheelDelta();
		if (mouseDelta != 0)
			cameraComponent->SetFov(cameraComponent->GetFov() - 0.1 * mouseDelta * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

private:
	CameraExclKeyboardControl(WorldObject* worldObject, Transform* camera_yPivot, Transform* camera, double movementSpeed, double lookingSpeed, double shiftModifier)
		:
		Component(worldObject),
		movementSpeed { movementSpeed },
		lookingSpeed { lookingSpeed },
		shiftModifier { shiftModifier },
		camera_yPivot { camera_yPivot },
		camera { camera }
	{
		cameraComponent = Components::Camera::GetMainCamera();

		shiftFactor = 1; // Gets calculated at the beginning of Update()

		return;
	}
	double movementSpeed;
	double lookingSpeed;
	double shiftModifier;

	double shiftFactor;

	Transform* camera_yPivot;
	Transform* camera;
	Components::Camera* cameraComponent;

	friend WorldObject;
};
