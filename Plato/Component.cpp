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

bool Component::GetIsGloballyEnabled() const
{
	// Am i disabled myself?
	if (!enabled)
		return false;
	
	// Gotta ask my parents then!
	return worldObject->GetIsGloballyEnabled();
}

Component::Component(WorldObject* worldObject)
	:
	worldObject { worldObject },
	transform { worldObject->transform }
{
	Init();
	return;
}
