#include "WorldObject.h"
#include "WorldObjectManager.h"

WorldObject::WorldObject()
	:
	transform { privateHandle__Transform }
{
	privateHandle__Transform = nullptr; // Will get set immediately after instantiation by the WorldObjectManager
	return;
}

WorldObject::~WorldObject()
{
	for (Component* co : components)
		delete co;

	components.clear();
	return;
}

void WorldObject::Destroy()
{
	// Save computing the recursive deletion, when they're already marked to be deleted
	if (deleteMe)
		return;

	// Delete self
	deleteMe = true;

	// Delete children
	for (Transform* tr : transform->GetChildren())
		tr->worldObject->Destroy();

	return;
}

const std::string& WorldObject::GetId() const
{
	return id;
}

const std::string& WorldObject::GetName() const
{
	return name;
}

const std::unordered_set<std::string>& WorldObject::GetTags() const
{
	return tags;
}

bool WorldObject::HasTag(const std::string& tag) const
{
	return tags.find(tag) != tags.end();
}

void WorldObject::SetName(const std::string& name)
{
	this->name = name;
	return;
}

bool WorldObject::SetId(const std::string& id)
{
	// Check that that id is not assigned yet
	if (WorldObjectManager::FindObjectById(id) != nullptr)
		return false;

	// If not, set it ias the own
	this->id = id;
	return true;
}

void WorldObject::ClearId()
{
	id = "";
	return;
}

void WorldObject::AddTag(const std::string& tag)
{
	tags.insert(tag);
	return;
}

void WorldObject::ClearTag(const std::string& tag)
{
	tags.erase(tag);
}

void WorldObject::ClearAllTags()
{
	tags.clear();
	return;
}

bool WorldObject::GetIsEnabled() const
{
	return enabled;
}

bool WorldObject::GetIsGloballyEnabled() const
{
	// Am i disabled myself?
	if (!enabled)
		return false;

	// Do i have a parent?
	if (transform->GetParent() != nullptr)
		// Gotta ask them then!
		return transform->GetParent()->worldObject->GetIsGloballyEnabled();
	
	// No parent, no worries
	return true;
}

void WorldObject::SetIsEnabled(bool state)
{
	enabled = state;
	return;
}

void WorldObject::Enable()
{
	SetIsEnabled(true);
	return;
}

void WorldObject::Disable()
{
	SetIsEnabled(false);
	return;
}

const std::unordered_set<Component*>& WorldObject::GetComponents() const
{
	return components;
}

std::unordered_set<Component*>& WorldObject::GetComponents()
{
	return components;
}
