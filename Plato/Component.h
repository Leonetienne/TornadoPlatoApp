#pragma once
#include "WorldObject.h"
#include "Transform.h"

class Component
{
public:
	WorldObject* const worldObject;
	Transform* const transform;

protected:
	// ### GAME ENGINE HOOKS ### //

	// Gets called once after the object is all set up
	virtual void Init() {};

	// Gets called every frame
	virtual void Update(double frametime) {};

	// Gets called every frame just before rendering Register your Renderables in renderer
	virtual void Render(double renderer) {};

	// Gets called just before this object gets deleted
	virtual void OnDestroy() {};

protected:
	Component(WorldObject* worldObject);

private:

	// Allow access to call hooks
	friend class WorldObjectManager;
};
