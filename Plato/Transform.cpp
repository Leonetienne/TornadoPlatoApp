#include "Transform.h"
#include <iostream>

Transform::Transform()
	:
	worldObject { privateHandle__WorldObject }
{
	parent = nullptr;
	privateHandle__WorldObject = nullptr; // Will get set immediately after instantiation by the WorldObjectManager

	return;
}

Transform::~Transform()
{

	return;
}

void Transform::SetPosition(const Vector3d& position)
{
	InvalidateLocalTransform();

	translationMatrix.SetTranslationComponent(position);

	return;
}

void Transform::SetScale(const Vector3d& scale)
{
	InvalidateLocalTransform();

	scaleMatrix.a = scale.x;
	scaleMatrix.f = scale.y;
	scaleMatrix.k = scale.z;

	return;
}

void Transform::SetRotation(const Quaternion& rot)
{
	InvalidateLocalTransform();

	rotation.SetRawValues(rot.GetRawValues());

	return;
}

void Transform::Move(const Vector3d& dPos)
{
	InvalidateLocalTransform();

	translationMatrix.d += dPos.x;
	translationMatrix.h += dPos.y;
	translationMatrix.l += dPos.z;

	return;
}

void Transform::Scale(const Vector3d& dScale)
{
	InvalidateLocalTransform();

	scaleMatrix.a *= dScale.x;
	scaleMatrix.f *= dScale.y;
	scaleMatrix.k *= dScale.z;

	return;
}

void Transform::Rotate(const Quaternion& dRot)
{
	InvalidateLocalTransform();

	rotation *= dRot;
	return;
}

const Vector3d Transform::GetPosition() const
{
	return translationMatrix.GetTranslationComponent();
}

const Vector3d Transform::GetScale() const
{
	return Vector3d(
		scaleMatrix.a,
		scaleMatrix.f,
		scaleMatrix.k
	);
}

const Quaternion& Transform::GetRotation() const
{
	return rotation;
}

Quaternion& Transform::GetWriteableRotation()
{
	InvalidateLocalTransform();
	return rotation;
}

Transform* Transform::GetParent() const
{
	return parent;
}

void Transform::SetParent(Transform* newParent, bool keepAbsoluteTransform)
{
	// Prevent alabama hierarchy
	for (Transform* it = newParent; it != nullptr; it = it->GetParent())
		if (it == this)
			return; // Abort

	// Gather global transformation data
	Matrix4x4 localInWorld;
	Quaternion oldRotation;
	if (keepAbsoluteTransform)
	{
		localInWorld = GetGlobalTransformationMatrix();
		oldRotation = GetGlobalRotation();
	}

	// Say goodbye to current parent, if not an orphan
	if (parent != nullptr)
		parent->children.erase(this);

	// Assign new parent, and notify it (if the parent an actual parent)
	parent = newParent;
	if (parent != nullptr)
		parent->children.insert(this);
	
	// Abort here, if we don't want to keep the absolute transform
	if (!keepAbsoluteTransform)
		return;

	// This code will only be reached if (keepAbsoluteTransform == true)
	// Let's restore the absolute transform as good as possible

	// Invalidate caches
	InvalidateLocalTransform();

	// Gather global transformation matrix of new parent
	Matrix4x4 pNewParentGlobalTransform;
	if (parent != nullptr)
		pNewParentGlobalTransform = parent->GetGlobalTransformationMatrix();

	// Move our object to local space of the new parent
	const Matrix4x4 localInNewParent = pNewParentGlobalTransform.Inverse3x3() * localInWorld;
	
	Vector3d nPos = localInNewParent.GetTranslationComponent();
	Quaternion nRot = oldRotation;

	// Adjust for rotation
	if (parent != nullptr)
	{
		nPos = parent->GetGlobalRotation().Inverse() * nPos;
		nRot = oldRotation * parent->GetGlobalRotation().Inverse();
	}

	// Set position and rotation
	SetPosition(nPos);
	SetRotation(nRot);

	// Approximate scale. This might cause shearing
	// Mathematically impossible to solve without shearing
	Quaternion r = oldRotation;
	if (parent != nullptr)
		r = oldRotation * parent->GetRotation().Inverse();

	Matrix4x4 m = r.ToRotationMatrix().Inverse3x3() * localInNewParent;
	Vector3d nScl = Vector3d(m.a, m.f, m.k);
	SetScale(nScl);

	return;
}

void Transform::AddChild(Transform* child)
{
	child->SetParent(this);
	return;
}

const Transform* Transform::operator[](std::size_t i) const
{
	if (i >= children.size())
		throw std::out_of_range("Transform child index out of range");

	std::set<Transform*>::iterator it = children.begin();
	std::advance(it, i);

	return *it;
}

Transform* Transform::operator[](std::size_t i)
{
	if (i >= children.size())
		throw std::out_of_range("Transform child index out of range");

	std::set<Transform*>::iterator it = children.begin();
	std::advance(it, i);

	return *it;
}

std::size_t Transform::GetNumChildren() const
{
	return children.size();
}

Matrix4x4 Transform::GetLocalTransformationMatrix() const
{
	// Recalculate if cache is invalid
	if (!cache__IsLocalTransformationMatrix_UpToDate)
	{
		cache__localTransformationMatrix = Matrix4x4();
		cache__localTransformationMatrix *= rotation.ToRotationMatrix();
		cache__localTransformationMatrix *= scaleMatrix;
		cache__localTransformationMatrix *= translationMatrix;

		cache__IsLocalTransformationMatrix_UpToDate = true;
	}

	return cache__localTransformationMatrix;
}

Matrix4x4 Transform::GetGlobalTransformationMatrix() const
{
	// Recalculate if cache is invalid
	if (!cache__IsGlobalTransformation_UpToDate)
		RecalculateGlobalTransformCache();

	return cache__globalTransformationMatrix;
}

Vector3d Transform::GetGlobalPosition() const
{
	// Recalculate if cache is invalid
	if (!cache__IsGlobalTransformation_UpToDate)
		RecalculateGlobalTransformCache();

	return cache__GlobalPosition;
}

Quaternion Transform::GetGlobalRotation() const
{
	// Recalculate if cache is invalid
	if(!cache__IsGlobalTransformation_UpToDate)
		RecalculateGlobalTransformCache();

	return cache__GlobalRotation;
}

void Transform::Reset()
{
	SetPosition(Vector3d::zero);
	SetScale(Vector3d::one);
	SetRotation(Quaternion());

	return;
}

void Transform::InvalidateLocalTransform()
{
	// Invalidate the local transform
	cache__IsLocalTransformationMatrix_UpToDate = false;

	// Since we changed our local transform, our global transform,
	// and that of all children, have also become invalid.
	InvalidateGlobalTransform();

	return;
}

void Transform::InvalidateGlobalTransform()
{
	// Invalidate own
	cache__IsGlobalTransformation_UpToDate = false;

	// Invalidate of children
	for (Transform* tr : children)
		tr->InvalidateGlobalTransform();

	return;
}

void Transform::RecalculateGlobalTransformCache() const
{
	// Re-Calculate global transformation matrix
	cache__globalTransformationMatrix = GetLocalTransformationMatrix();
	for (const Transform* tr = this->GetParent(); tr != nullptr; tr = tr->GetParent())
	{
		Vector3d newPos;
		newPos = cache__globalTransformationMatrix.GetTranslationComponent();

		newPos *= tr->rotation.ToRotationMatrix();

		cache__globalTransformationMatrix.SetTranslationComponent(newPos);
		cache__globalTransformationMatrix = tr->GetLocalTransformationMatrix() * cache__globalTransformationMatrix;
	}

	// Re-Calculate global rotation
	// If we are doing this the other way around (parent -> child, instead of child -> parent),
	// nested rotations (like, camera_yPivot -> camera) break...
	cache__GlobalRotation = Quaternion();
	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		cache__GlobalRotation *= tr->rotation;
	}

	// Re-Calculate global position
	cache__GlobalPosition = cache__globalTransformationMatrix.GetTranslationComponent();

	// Validate cache
	cache__IsGlobalTransformation_UpToDate = true;

	return;
}
