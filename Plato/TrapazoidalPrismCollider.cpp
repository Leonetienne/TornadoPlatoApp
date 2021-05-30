#include "TrapazoidalPrismCollider.h"
#include "WorldObjectManager.h"
#include "Camera.h"

using namespace Plato;
using namespace Plato::Components;
using namespace TorGL;

TrapazoidalPrismCollider::TrapazoidalPrismCollider(WorldObject* worldObject) :
	Plato::Components::Collider(worldObject)
{
	// Create default shape (a cube unit-cube)
	SetVertex(FRONT	| LEFT  | BOTTOM,	Vector3d(-1, -1,  1));
	SetVertex(FRONT	| LEFT  | TOP,		Vector3d(-1,  1,  1));
	SetVertex(BACK	| LEFT  | BOTTOM,	Vector3d(-1, -1, -1));
	SetVertex(BACK	| LEFT  | TOP,		Vector3d(-1,  1, -1));
	SetVertex(FRONT	| RIGHT | BOTTOM,	Vector3d( 1, -1,  1));
	SetVertex(FRONT	| RIGHT | TOP,		Vector3d( 1,  1,  1));
	SetVertex(BACK	| RIGHT | BOTTOM,	Vector3d( 1, -1, -1));
	SetVertex(BACK	| RIGHT | TOP,		Vector3d( 1,  1, -1));
	
	return;
}

void TrapazoidalPrismCollider::LateUpdate(double frameTime)
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

void TrapazoidalPrismCollider::SetVertex(std::size_t index, const Vector3d value)
{
	const Vector3d& relativeVertex = value;
	const Vector3d worldspaceVertex = transform->ObjectSpaceToWorldSpace(relativeVertex);
	const Vector3d cameraspaceVertex = Camera::GetMainCamera()->WorldSpaceToCameraSpace(worldspaceVertex);

	Eule::TrapazoidalPrismCollider::SetVertex(index, relativeVertex);
	worldspaceCollider.SetVertex(index, worldspaceVertex);
	cameraspaceCollider.SetVertex(index, cameraspaceVertex);

	return;
}

const Eule::Collider* TrapazoidalPrismCollider::WorldSpaceColldier() const
{
	return &worldspaceCollider;
}

const Eule::Collider* TrapazoidalPrismCollider::CameraSpaceColldier() const
{
	return &cameraspaceCollider;
}
