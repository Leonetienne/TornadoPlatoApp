#include "WorldObject.h"

WorldObject::WorldObject()
{
	transform = nullptr; // Will get set immediately after instantiation by the WorldObjectManager
	return;
}

Transform* WorldObject::GetTransform()
{
	return transform;
}

const Transform* WorldObject::GetTransform() const
{
	return transform;
}
