#include "WorldObjectManager.h"

WorldObject* WorldObjectManager::NewWorldObject(Transform* parent)
{
	// Create new pair
	WorldObject* newWorldObject = new WorldObject();
	Transform* newTransform = new Transform();

	// Connect pair <3
	newWorldObject->transform = newTransform;
	newTransform->worldObject = newWorldObject;

	// Assign parent
	newTransform->SetParent(parent);

	// Put in list of objects
	worldObjects.push_back(newWorldObject);

	return newWorldObject;
}

void WorldObjectManager::Free()
{
	for (WorldObject* wo : worldObjects)
	{
		// The transforms get allocated here, so they'll get freed here
		delete wo->transform;
		wo->transform = nullptr;

		delete wo;
		wo = nullptr;
	}

	worldObjects.clear();
	worldObjects.shrink_to_fit();

	return;
}

std::vector<WorldObject*> WorldObjectManager::worldObjects;
