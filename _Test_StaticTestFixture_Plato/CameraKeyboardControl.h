#pragma once
#include <Windows.h>
#include "../Plato/Component.h"
#include "../Plato/Camera.h"
#include "../Plato/WorldObject.h"

class CameraKeyboardControl : Component
{
public:
	void Update(double deltaTime)
	{
		shiftFactor = GetAsyncKeyState(VK_LSHIFT) ? shiftModifier : 1;

		MovementControl(deltaTime);
		ViewControl(deltaTime);
		AdditionalControls(deltaTime);

		return;
	}

private:
	const double internalMultiplier = 0.05;

	void MovementControl(double deltaTime)
	{
		if (GetAsyncKeyState('A'))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::left * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		if (GetAsyncKeyState('D'))
			camera_yPivot->Move(camera->GetGlobalRotation() * Vector3d::right * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (GetAsyncKeyState('W'))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::forward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		if (GetAsyncKeyState('S'))
			camera_yPivot->Move(camera->GetGlobalRotation() * -Vector3d::backward * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		if (GetAsyncKeyState('Q'))
			camera_yPivot->Move(Vector3d::down * movementSpeed * shiftFactor * deltaTime * internalMultiplier);
		if (GetAsyncKeyState('E'))
			camera_yPivot->Move(Vector3d::up * movementSpeed * shiftFactor * deltaTime * internalMultiplier);

		return;
	}

	void ViewControl(double deltaTime)
	{
		if (GetAsyncKeyState('Y'))
			camera_yPivot->Rotate(Quaternion(Vector3d(0, -2, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (GetAsyncKeyState('X'))
			camera_yPivot->Rotate(Quaternion(Vector3d(0, 2, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (GetAsyncKeyState('C'))
			camera->Rotate(Quaternion(Vector3d(2, 0, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));
		if (GetAsyncKeyState('F'))
			camera->Rotate(Quaternion(Vector3d(-2, 0, 0) * lookingSpeed * shiftFactor * deltaTime * internalMultiplier));

		return;
	}

	void AdditionalControls(double deltaTime)
	{
		if (GetAsyncKeyState('1'))
			cameraComponent->SetFov(cameraComponent->GetFov() - 1 * shiftFactor * deltaTime * internalMultiplier);
		if (GetAsyncKeyState('2'))
			cameraComponent->SetFov(cameraComponent->GetFov() + 1 * shiftFactor * deltaTime * internalMultiplier);

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
