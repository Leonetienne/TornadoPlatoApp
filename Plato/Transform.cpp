#include "Transform.h"

Transform::Transform()
{
	scaleMatrix.a = 1;
	scaleMatrix.f = 1;
	scaleMatrix.k = 1;

	translationMatrix.a = 1;
	translationMatrix.f = 1;
	translationMatrix.k = 1;

	parent = nullptr;
	worldObject = nullptr; // Will get set immediately after instantiation by the WorldObjectManager

	return;
}

Transform::~Transform()
{

	return;
}

void Transform::SetPosition(const Vector3d& position)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

	translationMatrix.d = position.x;
	translationMatrix.h = position.y;
	translationMatrix.l = position.z;

	return;
}

void Transform::SetScale(const Vector3d& scale)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

	scaleMatrix.a = scale.x;
	scaleMatrix.f = scale.y;
	scaleMatrix.k = scale.z;

	return;
}

void Transform::SetRotation(const Quaternion& rot)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

	rotation.SetRawValues(rot.GetRawValues());

	return;
}

void Transform::Move(const Vector3d& dPos)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

	translationMatrix.d += dPos.x;
	translationMatrix.h += dPos.y;
	translationMatrix.l += dPos.z;

	return;
}

void Transform::Scale(const Vector3d& dScale)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

	scaleMatrix.d *= dScale.x;
	scaleMatrix.h *= dScale.y;
	scaleMatrix.l *= dScale.z;

	return;
}

void Transform::Rotate(const Quaternion& dRot)
{
	rotation *= dRot;
	return;
}

const Vector3d Transform::GetPosition() const
{
	return Vector3d(
		translationMatrix.d,
		translationMatrix.h,
		translationMatrix.l
	);
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
	cache__IsLocalTransformationMatrix_UpToDate = false;
	return rotation;
}

Transform* Transform::GetParent() const
{
	return parent;
}

void Transform::SetParent(Transform* newParent)
{
	// Prevent alabama tree
	for (Transform* it = newParent; it != nullptr; it = it->GetParent())
		if (it == this)
			return; // Abort

	// Say goodbye to current parent, if not an orphan
	if (parent != nullptr)
		parent->children.erase(this);

	// Assign new parent, and notify it (if the parent an actual parent)
	parent = newParent;
	if (parent != nullptr)
		parent->children.insert(this);

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

WorldObject* Transform::GetWorldObject()
{
	return worldObject;
}

const WorldObject* Transform::GetWorldObject() const
{
	return worldObject;
}

Matrix4x4 Transform::GetLocalTransformationMatrix() const
{
	// Recalculate if cache is invalid
	if (!cache__IsLocalTransformationMatrix_UpToDate)
	{
		cache_localTransformationMatrix = scaleMatrix;
		cache_localTransformationMatrix *= rotation.ToRotationMatrix();
		cache_localTransformationMatrix *= translationMatrix;

		cache__IsLocalTransformationMatrix_UpToDate = true;
	}

	return cache_localTransformationMatrix;
}
