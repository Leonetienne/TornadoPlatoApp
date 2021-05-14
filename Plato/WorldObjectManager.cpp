#include "WorldObjectManager.h"
#include "Component.h"

#ifdef _DEBUG
#include <iostream>
#endif

WorldObject* WorldObjectManager::NewWorldObject(const std::string& name, Transform* parent)
{
	// Create new pair
	try
	{
		WorldObject* newWorldObject = new WorldObject();
		Transform* newTransform = new Transform();
	
		// Connect pair <3
		newWorldObject->privateHandle__Transform = newTransform;
		newTransform->privateHandle__WorldObject = newWorldObject;

		// Assign metadata
		newWorldObject->SetName(name);
		newTransform->SetParent(parent, false);
		newTransform->Reset();

		// Put in list of objects
		worldObjects.insert(newWorldObject);

		return newWorldObject;

	}
	catch (std::bad_alloc& bac)
	{
		#ifdef _DEBUG
		std::cerr << "Bad alloc!: " << bac.what() << std::endl;
		#else
		(void*)&bac; // See compiler? bac is not ununsed :3
		#endif
	}
	return nullptr;
}

WorldObject* WorldObjectManager::NewWorldObject(Transform* parent)
{
	return NewWorldObject("World Object", parent);
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

std::unordered_set<WorldObject*> WorldObjectManager::FindObjectsByName(const std::string& name)
{
	std::unordered_set<WorldObject*> results;

	for (WorldObject* wo : worldObjects)
		if (wo->GetName() == name)
			results.insert(wo);

	return results;
}

std::unordered_set<WorldObject*> WorldObjectManager::FindObjectsByTag(const std::string& tag)
{
	std::unordered_set<WorldObject*> results;

	for (WorldObject* wo : worldObjects)
		if (wo->HasTag(tag))
			results.insert(wo);

	return results;
}

void WorldObjectManager::RegisterWorldObjectForDeletion(WorldObject* wo)
{
	// Add wo to delete list
	objectsFlaggedForDeletion.insert(wo);

	// Recursively add all children to delete list
	for (Transform* tr : wo->transform->GetChildren())
		RegisterWorldObjectForDeletion(tr->worldObject);

	return;
}

void WorldObjectManager::DeleteFlaggedObjects()
{
	// For each object that should be deleted
	if (objectsFlaggedForDeletion.size() > 0)
		for (WorldObject* wo : objectsFlaggedForDeletion)
		{
			// Call OnDestroy hooks
			if (wo->GetIsGloballyEnabled())
				for (Component* co : wo->GetComponents())
					if (co->GetIsGloballyEnabled())
						co->OnDestroy();

			// Remove world object from main set
			worldObjects.erase(wo);

			// Free memory
			FreeWorldObject(wo);
		}

	// Clear set of objects that should be deleted
	objectsFlaggedForDeletion.clear();

	return;
}

void WorldObjectManager::CallHook__Update(double frametime)
{
	for (WorldObject* wo : worldObjects)
		if (wo->GetIsGloballyEnabled())
			for (Component* co : wo->components)
				if (co->GetIsGloballyEnabled())
					co->Update(frametime);

	return;
}

void WorldObjectManager::CallHook__LateUpdate(double frametime)
{
	for (WorldObject* wo : worldObjects)
		if (wo->GetIsGloballyEnabled())
			for (Component* co : wo->components)
				if (co->GetIsGloballyEnabled())
					co->LateUpdate(frametime);

	return;
}

void WorldObjectManager::CallHook__Render(Renderer* renderer)
{
	for (WorldObject* wo : worldObjects)
		if (wo->GetIsGloballyEnabled())
			for (Component* co : wo->components)
				if (co->GetIsGloballyEnabled())
					co->Render(renderer);

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
		// Call OnDestroy hooks
		if (wo->GetIsGloballyEnabled())
			for (Component* co : wo->components)
				if (co->GetIsGloballyEnabled())
					co->OnDestroy();

		FreeWorldObject(wo);
	}

	worldObjects.clear();

	return;
}

void WorldObjectManager::FreeWorldObject(WorldObject* wo)
{
	// Orphan the objects children

	// Cache the child-set because SetParent() will modify the set behind GetChildren()!
	std::unordered_set<Transform*> cachedChildren = wo->transform->GetChildren();

	// Do the orphaning fest
	for (Transform* tr : cachedChildren)
		tr->SetParent(nullptr, false);

	// Clear own parent relationship
	wo->transform->SetParent(nullptr, false);
	
	// Delete transform and world object
	delete wo->transform;
	delete wo;

	return;
}

std::unordered_set<WorldObject*> WorldObjectManager::worldObjects;
std::unordered_set<WorldObject*> WorldObjectManager::objectsFlaggedForDeletion;
