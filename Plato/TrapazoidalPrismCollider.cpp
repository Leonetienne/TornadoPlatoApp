#include "TrapazoidalPrismCollider.h"
#include "WorldObjectManager.h"

PTrapazoidalPrismCollider::PTrapazoidalPrismCollider(WorldObject* worldObject) :
	PCollider(worldObject)
{
	cam = WorldObjectManager::FindObjectById("main_camera")->GetComponentOfType<Camera>();
	return;
}

#include <iostream>
void PTrapazoidalPrismCollider::LateUpdate(double frameTime)
{
	Matrix4x4 inverseCameraRotation = cam->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
	Vector3d  inverseCameraPosition = -cam->transform->GetGlobalPosition();

	// Update the trapazoidal collider
	for (std::size_t i = 0; i < 8; i++)
	{
		// TODO: Optimize this
		const Vector3d& relativeVertex = GetVertex(i);
		const Vector3d worldspaceVertex = transform->GetRotation() * (relativeVertex + transform->GetGlobalPosition());
		const Vector3d cameraspaceVertex = (relativeVertex + inverseCameraPosition) * inverseCameraRotation;

		worldspaceCollider.SetVertex(i, worldspaceVertex);
		cameraspaceCollider.SetVertex(i, cameraspaceVertex);
	}

	return;
}

void PTrapazoidalPrismCollider::SetVertex(std::size_t index, const Vector3d value)
{
	// For consistency we need to calculate world, and camera space values aswell just after setting a vertex.
	// TODO: Optimize this

	Matrix4x4 inverseCameraRotation = cam->transform->GetGlobalRotation().Inverse().ToRotationMatrix();
	Vector3d  inverseCameraPosition = -cam->transform->GetGlobalPosition();

	const Vector3d& relativeVertex = value;
	const Vector3d worldspaceVertex = transform->GetRotation() * (relativeVertex + transform->GetGlobalPosition());
	const Vector3d cameraspaceVertex = (relativeVertex + inverseCameraPosition) * inverseCameraRotation;

	TrapazoidalPrismCollider::SetVertex(index, relativeVertex);
	worldspaceCollider.SetVertex(index, worldspaceVertex);
	cameraspaceCollider.SetVertex(index, cameraspaceVertex);

	return;
}

const Collider* PTrapazoidalPrismCollider::WorldSpaceColldier() const
{
	return &worldspaceCollider;
}

const Collider* PTrapazoidalPrismCollider::CameraSpaceColldier() const
{
	return &cameraspaceCollider;
}
