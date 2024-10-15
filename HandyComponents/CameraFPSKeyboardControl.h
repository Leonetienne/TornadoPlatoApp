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
    FPS keyboard controls for camera movement. With mouse!
    WASDQE for movement, Mouse for looking.
* */
class CameraFPSKeyboardControl : public Component
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
	const double internalMultiplier = 0.05 * 7;

	void MovementControl(double deltaTime)
	{
		// This is just experimental code.
		// Moves the viewport as the window moves.
		{
			static Vector2d lastPos = Input::Application::GetWindowRect().pos;
			static bool skipFirst = false;

			Vector2d dPos = Input::Application::GetWindowRect().pos - lastPos;

			if (skipFirst)
			{
				camera_yPivot->Move(camera->GetGlobalRotation() *
					(
						(
							Vector3d::left *
							movementSpeed * shiftFactor * deltaTime * internalMultiplier * dPos.x * -0.05
							)
						+
						(
							Vector3d::up *
							movementSpeed * shiftFactor * deltaTime * internalMultiplier * dPos.y * -0.05
							)
						)
				);
			}

			if (dPos.SqrMagnitude() > 0)
				skipFirst = true;

			lastPos = Input::Application::GetWindowRect().pos;
		}

		if (Input::Keyboard::GetKey(Input::KEY_CODE::A))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::left * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::D))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::right * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (Input::Keyboard::GetKey(Input::KEY_CODE::W))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::forward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::S))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::backward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (Input::Keyboard::GetKey(Input::KEY_CODE::Q))
			camera_yPivot->Move(Vector3d::down * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		
		if (Input::Keyboard::GetKey(Input::KEY_CODE::E))
			camera_yPivot->Move(Vector3d::up * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

	void ViewControl(double deltaTime)
	{
		// Initialize delta mouse position
		const Vector2d halfWinSize = Input::Application::GetWindowRect().size / 2.0;
		const Vector2d deltaMouse = Input::Mouse::GetMouseDelta().ToDouble() - halfWinSize.ToInt().ToDouble();

		// Handle y rotation
		const double mx = (deltaMouse.x / 100.0) * lookingSpeed;
		camera_yPivot->Rotate(Quaternion(Vector3d(0, mx * deltaTime, 0)));

		// Handle x rotation
		const double my = (deltaMouse.y / 100.0) * lookingSpeed;
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

		// Fov by mousewheel
		const double mouseDelta = Input::Mouse::GetMousewheelDelta();
		if (mouseDelta != 0)
			cameraComponent->SetFov(cameraComponent->GetFov() - 0.1 * mouseDelta * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

private:
	CameraFPSKeyboardControl(WorldObject* worldObject, Transform* camera_yPivot, Transform* camera, double movementSpeed, double lookingSpeed, double shiftModifier)
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
