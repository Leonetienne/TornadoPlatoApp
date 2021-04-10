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

	scaleMatrix.a *= dScale.x;
	scaleMatrix.f *= dScale.y;
	scaleMatrix.k *= dScale.z;

	return;
}

void Transform::Rotate(const Quaternion& dRot)
{
	cache__IsLocalTransformationMatrix_UpToDate = false;

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

void Transform::SetParent(Transform* newParent, bool keepAbsoluteTransform)
{
	// Prevent alabama tree
	for (Transform* it = newParent; it != nullptr; it = it->GetParent())
		if (it == this)
			return; // Abort

	if ((parent != nullptr) && (keepAbsoluteTransform))
	{
		// Resolve global 
		this->Scale(Vector3d(
			 parent->GetGlobalScale().x,
			 parent->GetGlobalScale().y,
			 parent->GetGlobalScale().z
		));

		Matrix4x4 parentGlobalScaleMatrix;
		parentGlobalScaleMatrix.a = parent->GetGlobalScale().x;
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
		
		this->Move(parent->GetGlobalPosition() * -1);

		this->Rotate(parent->GetGlobalRotation().Inverse());
		this->SetPosition(this->GetPosition() * parent->GetGlobalRotation().ToRotationMatrix());

		// Resolve new local transform
		this->Scale(Vector3d(
			1.0 / parent->GetGlobalScale().x,
			1.0 / parent->GetGlobalScale().y,
			1.0 / parent->GetGlobalScale().z
		));
		Matrix4x4 parentGlobalInverseScaleMatrix;
		parentGlobalInverseScaleMatrix.a = 1.0 / parent->GetGlobalScale().x;
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
		cache_localTransformationMatrix = scaleMatrix;
		cache_localTransformationMatrix *= rotation.ToRotationMatrix();
		cache_localTransformationMatrix *= translationMatrix;

		cache__IsLocalTransformationMatrix_UpToDate = true;
	}

	return cache_localTransformationMatrix;
}

Matrix4x4 Transform::GetGlobalTransformationMatrix() const
{
	Matrix4x4 glob = GetLocalTransformationMatrix();

	// Apply global scale
	for (const Transform* tr = this->GetParent(); tr != nullptr; tr = tr->GetParent())
	{
		Vector3d newPos;
		newPos = Vector3d(glob.d, glob.h, glob.l);
		newPos *= tr->scaleMatrix;
		newPos *= tr->rotation.ToRotationMatrix();

		glob.d = newPos.x;
		glob.h = newPos.y;
		glob.l = newPos.z;
		glob *= tr->GetLocalTransformationMatrix();
	}

	return glob;
}

Vector3d Transform::GetGlobalPosition() const
{
	// Transform the local origin by our global transformation matrix
	return Vector3d::zero * GetGlobalTransformationMatrix();
}

Quaternion Transform::GetGlobalRotation() const
{
	Quaternion rot;

	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		rot *= tr->rotation;
	}

	return rot;
}

Vector3d Transform::GetGlobalScale() const
{
	Vector3d scale = Vector3d::one;

	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		scale.x *= tr->GetScale().x;
		scale.y *= tr->GetScale().y;
		scale.z *= tr->GetScale().z;
	}

	return scale;
}
