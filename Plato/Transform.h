#pragma once
#include <set>
#include "Quaternion.h"
#include "../Tornado/Matrix4x4.h"
#include "../Tornado/Vector3.h"

class WorldObject;

class Transform final
{
public:
	~Transform();

	// Will set the transforms local position
	void SetPosition(const Vector3d& position);

	// Will set the transforms local scale
	void SetScale(const Vector3d& scale);

	// Will set the transforms local rotation
	void SetRotation(const Quaternion& rot);

	// Will add a vector to the local position
	void Move(const Vector3d& dPos);

	// Will scale the local scale by the supplied values
	void Scale(const Vector3d& dScale);

	// Will rotate the object locally by dRot
	void Rotate(const Quaternion& dRot);

	// Will return the transforms local position
	const Vector3d GetPosition() const;

	// Will return the transforms local scale
	const Vector3d GetScale() const;

	// Will return the transforms local rotation
	const Quaternion& GetRotation() const;

	// This will return a writeable reference of the rotation representation.
	// Don't use this for reading only, because calling this will invalidate the transformation matrix cache!!
	Quaternion& GetWriteableRotation();

	// Will return the transforms parent
	Transform* GetParent() const;

	// Will set this transforms new parent
	void SetParent(Transform* newParent);

	// Will set a Transform as a child. This will delete the existing parent-child relationship of the supplied transform!
	void AddChild(Transform* child);

	// Will reference a child at index i
	const Transform* operator[](std::size_t i) const;
	Transform* operator[](std::size_t i);
	
	// Will return the amount of children
	std::size_t GetNumChildren() const;

	// Will return the transforms corresponding world object
	WorldObject* GetWorldObject();
	const WorldObject* GetWorldObject() const;

	// Will return a transformation matrix relative to this transforms origin
	Matrix4x4 GetLocalTransformationMatrix() const;

private:
	Matrix4x4 scaleMatrix;
	Matrix4x4 translationMatrix;
	Quaternion rotation;

	Transform* parent;
	WorldObject* worldObject;

	mutable Matrix4x4 cache_localTransformationMatrix;
	mutable bool cache__IsLocalTransformationMatrix_UpToDate = false;

	// Stores pointers to all child transforms
	std::set<Transform*> children;

private:
	// No public instantiation! >:(
	Transform();

	friend class WorldObjectManager;
};
