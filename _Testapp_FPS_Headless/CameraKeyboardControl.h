#pragma once
#include "../Plato/Component.h"
#include "../Plato/Camera.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Application.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Mouse.h"
#include <algorithm>

using namespace Plato;

class CameraKeyboardControl : public Component
{
public:
	void Update(double deltaTime)
	{
		shiftFactor = Input::Keyboard::GetKey(Input::KEY_CODE::LSHIFT) ? shiftModifier : 1;

		MovementControl(deltaTime);
		ViewControl(deltaTime);
		AdditionalControls(deltaTime);

		return;
	}

private:
	const double internalMultiplier = 1 * 0.03;
	const double viewSensitivity = 3.3 * 0.06;

	void MovementControl(double deltaTime)
	{
		if (Input::Keyboard::GetKey(Input::KEY_CODE::A))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d(-1,0,0) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::D))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d(1,0,0) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (Input::Keyboard::GetKey(Input::KEY_CODE::W))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d(0,0,1) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::S))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d(0,0,-1) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (Input::Keyboard::GetKey(Input::KEY_CODE::Q))
			camera_yPivot->Move(Vector3d(0,-1,0) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::E))
			camera_yPivot->Move(Vector3d(0,1,0) * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

	void ViewControl(double deltaTime)
	{
		// Use mouse for all camera movement
        Vector2d emulatedDeltaMouse(0, 0);

        if (Input::Keyboard::GetKey(Input::KEY_CODE::K)) {
            emulatedDeltaMouse.y -= 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::J)) {
            emulatedDeltaMouse.y += 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::H)) {
            emulatedDeltaMouse.x -= 100.0 * viewSensitivity;
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::L)) {
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
		// Esc to exit
		if (Input::Keyboard::GetKeyDown(Input::KEY_CODE::ESCAPE))
			Input::Application::Exit();

		// Fov keys 1, and 2
        if (Input::Keyboard::GetKey(Input::KEY_CODE::NUM_1)) {
            cameraComponent->SetFov(cameraComponent->GetFov() + 0.1 * deltaTime);
        }
        if (Input::Keyboard::GetKey(Input::KEY_CODE::NUM_2)) {
            cameraComponent->SetFov(cameraComponent->GetFov() - 0.1 * deltaTime);
        }
		const double mouseDelta = Input::Mouse::GetMousewheelDelta();
		if (mouseDelta != 0)
			cameraComponent->SetFov(cameraComponent->GetFov() - 0.1 * mouseDelta * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

private:
	CameraKeyboardControl(WorldObject* worldObject, Transform* camera_yPivot, Transform* camera, double movementSpeed, double lookingSpeed, double shiftModifier)
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
