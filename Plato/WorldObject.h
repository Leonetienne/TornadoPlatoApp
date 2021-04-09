#pragma once
#include <string>
#include <set>
#include <type_traits>

class Transform;
class Component;

class WorldObject final
{
public:
	Transform* GetTransform();
	const Transform* GetTransform() const;

	// Will delete this world object at the beginning of the next frame
	void Destroy();

	// Will return this world objects unique id
	const std::string& GetId() const;

	// Will return this world objects name
	const std::string& GetName() const;

	// Will return all tags associated to this world object
	const std::set<std::string>& GetTags() const;

	// Will check if this world object is associated to a given tag
	bool HasTag(const std::string& tag) const;

	// Will set this world objects name
	void SetName(const std::string& name);

	// Will set this world objects unique id. Returns false if the id is already taken
	bool SetId(const std::string& id);

	// Will remove/clear this objects id
	void ClearId();

	// Will associate a given tag with this world object. Does nothing if the tag is already associated.
	void AddTag(const std::string& tag);

	// Will remove a single tag association
	void ClearTag(const std::string& tag);

	// Will clear this world object of all tag associations
	void ClearAllTags();

	template <class T, typename ...Params>
	T* CreateComponent(Params&&... params);
	

	// Will return a set of all components
	const std::set<Component*>& GetComponents() const;
	// Will return a set of all components
	std::set<Component*>& GetComponents();

	template<class T>
	// Will return the first component of type T. Nullptr if none found
	T* GetComponentOfType(T t);
	template<class T>
	// Will return the first component of type T. Nullptr if none found
	const T* GetComponentOfType(T t) const;

private:
	// Object identification
	std::string name;
	std::set<std::string> tags;
	std::string id;

	// Registered components
	std::set<Component*> components;
	
	// The associated 3d transform
	Transform* transform;

	// If true, the WorldObjectManager will delete this object at the beginning of the next frame.
	// This way, deleting this object will not screw over scripts mid - frame
	bool deleteMe = false;

	// Allow intimate access
	friend class WorldObjectManager;
	
	// No instantiation without the manager! >:(
	WorldObject();
};

template<class T, typename ...Params>
inline T* WorldObject::CreateComponent(Params&&... params)
{
	T* newComponent = new T(this, std::forward<Params>(params)...);
	Component* compPtr = dynamic_cast<Component*>(newComponent);
	
	// Oops! T is not a derivation if Component
	if (compPtr == nullptr)
		throw std::exception("Template type not derivation of Component!");

	components.insert(compPtr);
	return newComponent;
}

template<class T>
inline T* WorldObject::GetComponentOfType(T t)
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
inline const T* WorldObject::GetComponentOfType(T t) const
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
