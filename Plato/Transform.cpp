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
	InvalidateLocalTransform();

	translationMatrix.d = position.x;
	translationMatrix.h = position.y;
	translationMatrix.l = position.z;

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
	InvalidateLocalTransform();
	return rotation;
}

Transform* Transform::GetParent() const
{
	return parent;
}

void Transform::SetParent(Transform* newParent, bool keepAbsoluteTransform)
{
	// Prevent alabama tree
	for (Transform* it = newParent; it != nullptr; it = it->GetParent())
		if (it == this)
			return; // Abort

	// Transform from parent(old)-space to global world space
	if ((parent != nullptr) && (keepAbsoluteTransform))
	{
		// Resolve global 
		this->Scale(Vector3d(
			 parent->GetGlobalScale().x, // These Getters use cached values. It's okay
			 parent->GetGlobalScale().y,
			 parent->GetGlobalScale().z
		));

		Matrix4x4 parentGlobalScaleMatrix;
		parentGlobalScaleMatrix.a = parent->GetGlobalScale().x; // These Getters use cached values. It's okay
		parentGlobalScaleMatrix.f = parent->GetGlobalScale().y;
		parentGlobalScaleMatrix.k = parent->GetGlobalScale().z;
		this->SetPosition(this->GetPosition() * parentGlobalScaleMatrix);

		this->Rotate(parent->GetGlobalRotation());
		this->SetPosition(this->GetPosition() * parent->GetGlobalRotation().Inverse().ToRotationMatrix());

		this->Move(parent->GetGlobalPosition());
	}

	// Say goodbye to current parent, if not an orphan
	if (parent != nullptr)
		parent->children.erase(this);

	// Assign new parent, and notify it (if the parent an actual parent)
	parent = newParent;
	if (parent != nullptr)
	{
		parent->children.insert(this);
		
		// Transform from global world space to parent(new)-space
		this->Move(parent->GetGlobalPosition() * -1);

		this->Rotate(parent->GetGlobalRotation().Inverse());
		this->SetPosition(this->GetPosition() * parent->GetGlobalRotation().ToRotationMatrix());

		// Resolve new local transform
		this->Scale(Vector3d(
			1.0 / parent->GetGlobalScale().x, // These Getters use cached values. It's okay
			1.0 / parent->GetGlobalScale().y,
			1.0 / parent->GetGlobalScale().z
		));
		Matrix4x4 parentGlobalInverseScaleMatrix;
		parentGlobalInverseScaleMatrix.a = 1.0 / parent->GetGlobalScale().x; // These Getters use cached values. It's okay
		parentGlobalInverseScaleMatrix.f = 1.0 / parent->GetGlobalScale().y;
		parentGlobalInverseScaleMatrix.k = 1.0 / parent->GetGlobalScale().z;
		this->SetPosition(this->GetPosition() * parentGlobalInverseScaleMatrix);
	}

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
		cache__localTransformationMatrix = scaleMatrix;
		cache__localTransformationMatrix *= rotation.ToRotationMatrix();
		cache__localTransformationMatrix *= translationMatrix;

		cache__IsLocalTransformationMatrix_UpToDate = true;
	}

	return cache__localTransformationMatrix;
}

Matrix4x4 Transform::GetGlobalTransformationMatrix() const
{
	// Recalculate if cache is invalid
	if (!cache__IsGlobalTransformationUpToDate)
		RecalculateGlobalTransformCache();

	return cache__globalTransformationMatrix;
}

Vector3d Transform::GetGlobalPosition() const
{
	// Recalculate if cache is invalid
	if (!cache__IsGlobalTransformationUpToDate)
		RecalculateGlobalTransformCache();

	return cache__GlobalPosition;
}

Quaternion Transform::GetGlobalRotation() const
{
	// Recalculate if cache is invalid
	if(!cache__IsGlobalTransformationUpToDate)
		RecalculateGlobalTransformCache();

	return cache__GlobalRotation;
}

Vector3d Transform::GetGlobalScale() const
{
	// Recalculate if cache is invalid
	if(!cache__IsGlobalTransformationUpToDate)
		RecalculateGlobalTransformCache();

	return cache__GlobalScale;
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
	cache__IsGlobalTransformationUpToDate = false;

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
		newPos = Vector3d(
			cache__globalTransformationMatrix.d,
			cache__globalTransformationMatrix.h,
			cache__globalTransformationMatrix.l
		);
		newPos *= tr->scaleMatrix * tr->rotation.ToRotationMatrix();

		cache__globalTransformationMatrix.d = newPos.x;
		cache__globalTransformationMatrix.h = newPos.y;
		cache__globalTransformationMatrix.l = newPos.z;
		cache__globalTransformationMatrix *= tr->GetLocalTransformationMatrix();
	}

	// Re-Calculate global scale
	cache__GlobalScale = Vector3d::one;
	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		cache__GlobalScale.x *= tr->GetScale().x;
		cache__GlobalScale.y *= tr->GetScale().y;
		cache__GlobalScale.z *= tr->GetScale().z;
	}

	// Re-Calculate global rotation
	cache__GlobalRotation = Quaternion();
	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		cache__GlobalRotation *= tr->rotation;
	}

	// Re-Calculate global position
	cache__GlobalPosition = Vector3d::zero * cache__globalTransformationMatrix; // <-- This just got re-calculated

	// Validate cache
	cache__IsGlobalTransformationUpToDate = true;

	return;
}
