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

Transform* WorldObject::GetTransform()
{
	return transform;
}

const Transform* WorldObject::GetTransform() const
{
	return transform;
}

void WorldObject::Destroy()
{
	deleteMe = true;
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

const std::set<std::string>& WorldObject::GetTags() const
{
	return tags;
}

bool WorldObject::HasTag(const std::string& tag) const
{
	for (const std::string& ctag : tags)
		if (ctag == tag)
			return true;

	return false;
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

const std::set<Component*>& WorldObject::GetComponents() const
{
	return components;
}

std::set<Component*>& WorldObject::GetComponents()
{
	return components;
}
