#pragma once
#include <vector>
#include "WorldObject.h"
#include "Transform.h"

// TODO:
// Optimize name, id, tag lookups via hashes

class WorldObjectManager
{
public:
	// Will create a new world object just by a parent
	static WorldObject* NewWorldObject(Transform* parent);

	// Will create a new world object by an optional name and optional parent
	static WorldObject* NewWorldObject(const std::string& name = "World Object", Transform* parent = nullptr);

	// Will return a world object that goes by this id. Nullptr if none
	static WorldObject* FindObjectById(const std::string& id);

	// Will search for world objects by their name. Only returns absolute matches.
	static std::vector<WorldObject*> FindObjectsByName(const std::string& name);

	// Will search for world objects by their associated tags.
	static std::vector<WorldObject*> FindObjectsByTag(const std::string& tag);

	// Will return the number of world objects
	static std::size_t GetNumObjects();

	// Will look for objects marked to be deleted and will delete them.
	// Call this once at the absolute beginning of every frame
	static void DeleteFlaggedObjects();

	// Will call the hook method "Update()" on all world objects.
	// Only call this ONCE per frame in your main loop!!
	static void CallHook__Update(double frametime);

	// Will call the hook method "Update()" on all world objects.
	// Only call this ONCE per frame in your main loop!!
	static void CallHook__Render(Renderer* renderer);

	// Will free all world objects
	static void Free();

private:

	// Will call delete on a world object and its transform
	static void FreeWorldObject(WorldObject* wo);

	static std::vector<WorldObject*> worldObjects;

	// No instantation! >:(
	WorldObjectManager();
};
