#include "TrapazoidalPrismCollider.h"
#include "WorldObjectManager.h"
#include "Camera.h"

PTrapazoidalPrismCollider::PTrapazoidalPrismCollider(WorldObject* worldObject) :
	PCollider(worldObject)
{
	// Create default shape (a cube unit-cube)
	using TPC = PTrapazoidalPrismCollider;
	SetVertex(TPC::FRONT	| TPC::LEFT  | TPC::BOTTOM, Vector3d(-1, -1,  1));
	SetVertex(TPC::FRONT	| TPC::LEFT  | TPC::TOP,	Vector3d(-1,  1,  1));
	SetVertex(TPC::BACK		| TPC::LEFT  | TPC::BOTTOM, Vector3d(-1, -1, -1));
	SetVertex(TPC::BACK		| TPC::LEFT  | TPC::TOP,	Vector3d(-1,  1, -1));
	SetVertex(TPC::FRONT	| TPC::RIGHT | TPC::BOTTOM, Vector3d( 1, -1,  1));
	SetVertex(TPC::FRONT	| TPC::RIGHT | TPC::TOP,	Vector3d( 1,  1,  1));
	SetVertex(TPC::BACK		| TPC::RIGHT | TPC::BOTTOM, Vector3d( 1, -1, -1));
	SetVertex(TPC::BACK		| TPC::RIGHT | TPC::TOP,	Vector3d( 1,  1, -1));
	
	return;
}

void PTrapazoidalPrismCollider::LateUpdate(double frameTime)
{
	// Update the trapazoidal collider
	for (std::size_t i = 0; i < 8; i++)
	{
		const Vector3d& relativeVertex = GetVertex(i);
		const Vector3d worldspaceVertex = transform->ObjectSpaceToWorldSpace(relativeVertex);
		const Vector3d cameraspaceVertex = Camera::GetMainCamera()->WorldSpaceToCameraSpace(worldspaceVertex);

		worldspaceCollider.SetVertex(i, worldspaceVertex);
		cameraspaceCollider.SetVertex(i, cameraspaceVertex);
	}

	return;
}

void PTrapazoidalPrismCollider::SetVertex(std::size_t index, const Vector3d value)
{
	const Vector3d& relativeVertex = value;
	const Vector3d worldspaceVertex = transform->ObjectSpaceToWorldSpace(relativeVertex);
	const Vector3d cameraspaceVertex = Camera::GetMainCamera()->WorldSpaceToCameraSpace(worldspaceVertex);

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
