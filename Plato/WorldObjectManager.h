#pragma once
#include <vector>
#include "WorldObject.h"
#include "Transform.h"

class WorldObjectManager
{
public:
	// Will create a new world object
	static WorldObject* NewWorldObject(Transform* parent = nullptr);

	// Will free all world objects
	static void Free();

private:
	static std::vector<WorldObject*> worldObjects;

	// No instantation! >:(
	WorldObjectManager();
};
