#pragma once
#include <string>
#include <set>

class Transform;

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


protected:
	// ### GAME ENGINE HOOKS ### //
	
	// Hook function. Override this. Gets called once after the object is all set up. DONT CALL YOURSELF!
	void Init() {};

	// Hook function. Override this. Gets called every frame. DONT CALL YOURSELF!
	void Update(double frametime) {};

	// Hook function. Override this. Gets called every frame just before rendering Register your Renderables in renderer. DONT CALL YOURSELF!
	void Render(double renderer) {};

	// Hook function. Override this. Gets called just before this object gets deleted. DONT CALL YOURSELF!
	void OnDestroy() {};

private:
	// Object identification
	std::string name;
	std::set<std::string> tags;
	std::string id;

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
