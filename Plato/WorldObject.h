#pragma once

class Transform;

class WorldObject
{
public:
	Transform* GetTransform();
	const Transform* GetTransform() const;

private:

	Transform* transform;

	friend class WorldObjectManager;
	
	// No instantiation without the manager! >:(
	WorldObject();
};
