#include "Component.h"

Component::Component(WorldObject* worldObject)
	:
	worldObject { worldObject },
	transform { worldObject->GetTransform() }
{
	Init();
	return;
}
