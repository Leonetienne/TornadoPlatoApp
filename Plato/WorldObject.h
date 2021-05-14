#pragma once
#include <string>
#include <unordered_set>
#include <type_traits>
#include "Component.h"

class Transform;

/** Representation of a World Object
*/
class WorldObject final
{
public:
	//! Public handle for this WorldObjects Transform
	Transform* const& transform;

	~WorldObject();

	//! Will delete this world object at the beginning of the next frame
	void Destroy();

	//! Will return this world objects unique id
	const std::string& GetId() const;

	//! Will return this world objects name
	const std::string& GetName() const;

	//! Will return all tags associated to this world object
	const std::unordered_set<std::string>& GetTags() const;

	//! Will check if this world object is associated to a given tag
	bool HasTag(const std::string& tag) const;

	//! Will set this world objects name
	void SetName(const std::string& name);

	//! Will set this world objects unique id. Returns false if the id is already taken
	bool SetId(const std::string& id);

	//! Will remove/clear this objects id
	void ClearId();

	//! Will associate a given tag with this world object. Does nothing if the tag is already associated.
	void AddTag(const std::string& tag);

	//! Will remove a single tag association
	void ClearTag(const std::string& tag);

	//! Will clear this world object of all tag associations
	void ClearAllTags();

	template <class T, typename ...Params>
	//! Will add a component of type T, and return a pointer to it
	T* AddComponent(Params... params);
	
	//! Will return whether or not this world object is locally enabled
	bool GetIsEnabled() const;

	//! Will return whether or not this world object is globally enabled (respecting its parents disabled-states)
	bool GetIsGloballyEnabled() const;

	//! Will locally enable or disable this world object
	void SetIsEnabled(bool state);

	//! Will locally enable this world object
	void Enable();

	//! Will locally disable this world object
	void Disable();

	//! Will return a set of all components
	const std::unordered_set<Component*>& GetComponents() const;
	//! Will return a set of all components
	std::unordered_set<Component*>& GetComponents();

	template<class T>
	//! Will return the first component of type T. Nullptr if none found
	T* GetComponentOfType();
	template<class T>
	//! Will return the first component of type T. Nullptr if none found
	const T* GetComponentOfType() const;

private:
	//! Object identification 
	std::string name;
	std::unordered_set<std::string> tags;
	std::string id;
	bool enabled = true;

	//! Registered components
	std::unordered_set<Component*> components;
	
	//! Private handle for this WorldObjects Transform
	Transform* privateHandle__Transform;

	//! Allow intimate access
	friend class WorldObjectManager;
	
	//! No instantiation without the manager! >:(
	WorldObject();
};

template<class T, typename ...Params>
inline T* WorldObject::AddComponent(Params... params)
{
	T* newComponent = new T(this, std::forward<Params>(params)...);
	Component* compPtr = dynamic_cast<Component*>(newComponent);
	
	// Oops! T is not a derivation of class Component
	if (compPtr == nullptr)
		throw std::runtime_error("Template type not derivation of Component!");

	components.insert(compPtr);
	return newComponent;
}

template<class T>
inline T* WorldObject::GetComponentOfType()
{
	for (Component* c : components)
	{
		// Try to cast pointer to T*
		T* tp = dynamic_cast<T*>(c);

		// Has it worked? if yes, return it
		if (tp != nullptr)
			return tp;
	}

	// Return nullptr if none were found
	return nullptr;
}

template<class T>
inline const T* WorldObject::GetComponentOfType() const
{
	for (const Component* c : components)
	{
		// Try to cast pointer to T*
		const T* tp = dynamic_cast<const T*>(c);

		// Has it worked? if yes, return it
		if (tp != nullptr)
			return tp;
	}

	// Return nullptr if none were found
	return nullptr;
}
