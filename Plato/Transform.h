#pragma once
#include <unordered_set>
#include "Quaternion.h"
#include "../Tornado/Matrix4x4.h"
#include "../Tornado/Vector3.h"

class WorldObject;

class Transform final
{
public:
	// Public handle for this Transforms WorldObject
	WorldObject* const& worldObject;

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
	// Set keepAbsoluteTransform to false to keep the local transformation values
	// !!! BEWARE !!! THIS METHOD WILL MODIFY ITS PARENTS children CONTAINER!!!
	// !!! THIS WILL CAUSE HARD-TO-DEBUG EXCEPTIONS WHEN CALLING THIS WHILST ITERATING OVER GetChildren() !!!
	// If you want to use this method whilst iterating over GetChildren(), copy the return value beforehand!
	void SetParent(Transform* newParent, bool keepAbsoluteTransform = true);

	// Will set a Transform as a child. This will delete the existing parent-child relationship of the supplied transform!
	void AddChild(Transform* child);

	// Will reference a child at index i
	const Transform* operator[](std::size_t i) const;
	Transform* operator[](std::size_t i);
	
	// Will return the amount of children
	std::size_t GetNumChildren() const;

	// Will return a set of all children
	std::unordered_set<Transform*>& GetChildren();
	// Will return a set of all children
	const std::unordered_set<Transform*>& GetChildren() const;

	// Will return a transformation matrix relative to this transforms origin
	Matrix4x4 GetLocalTransformationMatrix() const;

	// Will return a transformation matrix relative to the world origin
	Matrix4x4 GetGlobalTransformationMatrix() const;

	// Will return this transforms position relative to the world origin
	Vector3d GetGlobalPosition() const;

	// Will return this transforms rotation relative to the world origin
	Quaternion GetGlobalRotation() const;

	// Will reset this transforms local values
	void Reset();

private:
	Matrix4x4 scaleMatrix;
	Matrix4x4 translationMatrix;
	Quaternion rotation;

	// Private handle for this Transforms WorldObject
	WorldObject* privateHandle__WorldObject;

	Transform* parent;

	// Cache data
	mutable Matrix4x4	cache__localTransformationMatrix;
	mutable bool		cache__IsLocalTransformationMatrix_UpToDate = false;
	mutable Matrix4x4	cache__globalTransformationMatrix;
	mutable bool		cache__IsGlobalTransformation_UpToDate = false;
	mutable Vector3d	cache__GlobalPosition;
	mutable Quaternion	cache__GlobalRotation;

	// Stores pointers to all child transforms
	std::unordered_set<Transform*> children;

private:
	// Will cause the local transform values to be re-calculated.
	void InvalidateLocalTransform();

	// Will cause the global transform values to be re-calculated. It also forwards this call to all children
	void InvalidateGlobalTransform();

	// Will recalculate the global transform values. You should only call this when cache__isGlobal... is false
	void RecalculateGlobalTransformCache() const;

	// No public instantiation! >:(
	Transform();

	friend class WorldObjectManager;
};
