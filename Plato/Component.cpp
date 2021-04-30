#include "Component.h"
#include "WorldObject.h"

void Component::SetIsEnabled(bool state)
{
	enabled = state;
	return;
}

void Component::Enable()
{
	SetIsEnabled(true);
	return;
}

void Component::Disable()
{
	SetIsEnabled(false);
	return;
}

bool Component::GetIsEnabled() const
{
	return enabled;
}

Component::Component(WorldObject* worldObject)
	:
	worldObject { worldObject },
	transform { worldObject->GetTransform() }
{
	Init();
	return;
}
