#pragma once
#include "Transform.h"

class WorldObject;
class Renderer;

class Component
{
public:
	WorldObject* const worldObject;
	Transform* const transform;

	// Will set the enabled-status of this component. Disabled components receive no hook calls!
	void SetIsEnabled(bool state);

	// Will enable this component. Disabled components receive no hook calls!
	void Enable();

	// Will disable this component. Disabled components receive no hook calls!
	void Disable();

	// Will return whether or not this component is enabled
	bool GetIsEnabled() const;

protected:
	// ### GAME ENGINE HOOKS ### //

	// Gets called once after the object is all set up
	virtual void Init() {};

	// Gets called every frame
	virtual void Update(double frametime) {};

	// Gets called every frame just before rendering Register your Renderables in renderer
	virtual void Render(Renderer* renderer) {};

	// Gets called just before this object gets deleted
	virtual void OnDestroy() {};

protected:
	Component(WorldObject* worldObject);

private:
	bool enabled = true;

	// Allow access to call hooks
	friend class WorldObjectManager;
};
