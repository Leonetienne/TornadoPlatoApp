#pragma once
#include <unordered_set>
#include "WorldObject.h"
#include "Transform.h"

namespace Plato
{
	// TODO:
	// Optimize name, id, tag lookups via hashes

	/** Manager class for WorldObjects.
	* This is the only class able to instanciate WorldObject's and Transform's.
	* This class will take complete care of them. When creating a new WorldObject,
	* it will automatically create a new Transform alongside it, link them, call their Component's hook methods
	* and free their memory either during runtime or in teardown.
	*
	* IMPORTANT:
	* Don NOT delete WorldObject and Transform instances yourself!! Don't do it! If you have to, for whatever reason,
	* delete a WorldObject immediately, do it like this:
	* ```
	* wObject->Destroy();
	* WorldObjectManager::DeleteFlaggedObjects();
	* ``
	*/
	class WorldObjectManager
	{
	public:
		//! Will create a new world object just by a parent
		static WorldObject* NewWorldObject(Transform* parent);

		//! Will create a new world object by an optional name and optional parent
		static WorldObject* NewWorldObject(const std::string& name = "World Object", Transform* parent = nullptr);

		//! Will return a world object that goes by this id. Nullptr if none
		static WorldObject* FindObjectById(const std::string& id);

		//! Will search for world objects by their name. Only returns absolute matches.
		static std::unordered_set<WorldObject*> FindObjectsByName(const std::string& name);

		//! Will search for world objects by their associated tags.
		static std::unordered_set<WorldObject*> FindObjectsByTag(const std::string& tag);

		//! Will register this WorldObject (and all its children) to be deleted with the next call of DeleteFlaggedObjects(), which is most likely at the end of the current frame
		static void RegisterWorldObjectForDeletion(WorldObject* wo);

		//! Will return the number of world objects
		static std::size_t GetNumObjects();

		//! Will look for objects marked to be deleted and will delete them.  
		//! Call this once at the absolute beginning of every frame
		static void DeleteFlaggedObjects();

		//! Will call the hook method "Update()" on all world objects.  
		//! Only call this ONCE per frame in your main loop!!
		static void CallHook__Update(double frametime);

		//! Will call the hook method "LateUpdate()" on all world objects.  
		//! Only call this ONCE per frame in your main loop!!
		static void CallHook__LateUpdate(double frametime);

		//! Will call the hook method "Update()" on all world objects.  
		//! Only call this ONCE per frame in your main loop!!
		static void CallHook__Render(Renderer* renderer);

		//! Will free all world objects
		static void Free();

	private:

		// Will call delete on a world object and its transform
		static void FreeWorldObject(WorldObject* wo);

		static std::unordered_set<WorldObject*> worldObjects;
		static std::unordered_set<WorldObject*> objectsFlaggedForDeletion;

		// No instantation! >:(
		WorldObjectManager();
	};
}
