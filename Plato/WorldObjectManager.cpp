#include "WorldObjectManager.h"

WorldObject* WorldObjectManager::NewWorldObject(const std::string& name, Transform* parent)
{
	// Create new pair
	WorldObject* newWorldObject = new WorldObject();
	Transform* newTransform = new Transform();

	// Connect pair <3
	newWorldObject->transform = newTransform;
	newTransform->worldObject = newWorldObject;

	// Assign metadata
	newWorldObject->SetName(name);
	newTransform->SetParent(parent);

	// Put in list of objects
	worldObjects.push_back(newWorldObject);

	// Call Init hook
	newWorldObject->Init();

	return newWorldObject;
}

WorldObject* WorldObjectManager::FindObjectById(const std::string& id)
{
	// Fast-reject for bullshit parameter
	if (id.length() == 0)
		return nullptr;

	for (WorldObject* wo : worldObjects)
		if (wo->GetId() == id)
			return wo;

	return nullptr;
}

std::vector<WorldObject*> WorldObjectManager::FindObjectsByName(const std::string& name)
{
	std::vector<WorldObject*> results;

	for (WorldObject* wo : worldObjects)
		if (wo->GetName() == name)
			results.push_back(wo);

	return results;
}

std::vector<WorldObject*> WorldObjectManager::FindObjectsByTag(const std::string& tag)
{
	std::vector<WorldObject*> results;

	for (WorldObject* wo : worldObjects)
		if (wo->HasTag(tag))
			results.push_back(wo);

	return results;
}

void WorldObjectManager::DeleteFlaggedObjects()
{
	for (long long int i = worldObjects.size() - 1; i >= 0; i--)
	{
		if (worldObjects[i]->deleteMe)
		{
			// Call OnDelete hook
			worldObjects[i]->OnDestroy();

			// Free memory
			FreeWorldObject(worldObjects[i]);

			// Remove "corpse" from vector
			worldObjects.erase(worldObjects.cbegin() + i);
		}
	}

	return;
}

void WorldObjectManager::CallHook__Update(double frametime)
{
	for (WorldObject* wo : worldObjects)
		wo->Update(frametime);

	return;
}

void WorldObjectManager::CallHook__Render(double renderer)
{
	for (WorldObject* wo : worldObjects)
		wo->Render(renderer);

	return;
}

std::size_t WorldObjectManager::GetNumObjects()
{
	return worldObjects.size();
}

void WorldObjectManager::Free()
{
	for (WorldObject* wo : worldObjects)
	{
		// Call OnDelete hook
		wo->OnDestroy();

		FreeWorldObject(wo);
	}

	worldObjects.clear();
	worldObjects.shrink_to_fit();

	return;
}

void WorldObjectManager::FreeWorldObject(WorldObject* wo)
{
	delete wo->GetTransform();
	delete wo;
	return;
}

std::vector<WorldObject*> WorldObjectManager::worldObjects;
