#pragma once
#include <Windows.h>
#include "../Plato/Component.h"
#include "../Plato/Camera.h"
#include "../Plato/WorldObject.h"
#include "../Plato/Application.h"
#include "../Plato/Keyboard.h"
#include "../Plato/Mouse.h"

class CameraKeyboardControl : Component
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
	const double internalMultiplier = 0.05;

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
		if (Input::Keyboard::GetKey(Input::KEY_CODE::Y))
			camera_yPivot->Rotate(Quaternion(Vector3d(0, -2, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (Input::Keyboard::GetKey(Input::KEY_CODE::X))
			camera_yPivot->Rotate(Quaternion(Vector3d(0, 2, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (Input::Keyboard::GetKey(Input::KEY_CODE::C))
			camera->Rotate(Quaternion(Vector3d(2, 0, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (Input::Keyboard::GetKey(Input::KEY_CODE::F))
			camera->Rotate(Quaternion(Vector3d(-2, 0, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));

		return;
	}

	void AdditionalControls(double deltaTime)
	{
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
		cameraComponent = camera->worldObject->GetComponentOfType<Camera>();

		shiftFactor = 1; // Gets calculated at the beginning of Update()

		return;
	}
	double movementSpeed;
	double lookingSpeed;
	double shiftModifier;

	double shiftFactor;

	Transform* camera_yPivot;
	Transform* camera;
	Camera* cameraComponent;

	friend WorldObject;
};
