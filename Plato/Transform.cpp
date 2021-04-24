#include "Transform.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Matrix4x4& o)
{
	for (std::size_t y = 0; y < 4; y++)
	{
		for (std::size_t x = 0; x < 4; x++)
			os << " | " <<o[y][x];

		os << " |" << std::endl;
	}

	return os;
}

Matrix4x4 Transpose(Matrix4x4 o)
{
	Matrix4x4 m;

	m[1][1] = o[1][1];
	m[1][2] = o[2][1];
	m[1][3] = o[3][1];
	
	m[2][1] = o[1][2];
	m[2][2] = o[2][2];
	m[0][3] = o[3][2];
	
	m[3][1] = o[1][3];
	m[3][2] = o[2][3];
	m[3][3] = o[3][3];

	return m;
}

Matrix4x4 Matrix4Mult(Matrix4x4 a, Matrix4x4 b)
{
	Matrix4x4 m;

	m[0][0] = (a[0][0]*b[0][0]) + (a[0][1]*b[1][0]) + (a[0][2]*b[2][0]) + (a[0][3]*b[3][0]);
	m[0][1] = (a[0][0]*b[0][1]) + (a[0][1]*b[1][1]) + (a[0][2]*b[2][1]) + (a[0][3]*b[3][1]);
	m[0][2] = (a[0][0]*b[0][2]) + (a[0][1]*b[1][2]) + (a[0][2]*b[2][2]) + (a[0][3]*b[3][2]);
	m[0][3] = (a[0][0]*b[0][3]) + (a[0][1]*b[1][3]) + (a[0][2]*b[2][3]) + (a[0][3]*b[3][3]);

	m[1][0] = (a[1][0]*b[0][0]) + (a[1][1]*b[1][0]) + (a[1][2]*b[2][0]) + (a[1][3]*b[3][0]);
	m[1][1] = (a[1][0]*b[0][1]) + (a[1][1]*b[1][1]) + (a[1][2]*b[2][1]) + (a[1][3]*b[3][1]);
	m[1][2] = (a[1][0]*b[0][2]) + (a[1][1]*b[1][2]) + (a[1][2]*b[2][2]) + (a[1][3]*b[3][2]);
	m[1][3] = (a[1][0]*b[0][3]) + (a[1][1]*b[1][3]) + (a[1][2]*b[2][3]) + (a[1][3]*b[3][3]);

	m[2][0] = (a[2][0]*b[0][0]) + (a[2][1]*b[1][0]) + (a[2][2]*b[2][0]) + (a[2][3]*b[2][0]);
	m[2][1] = (a[2][0]*b[0][1]) + (a[2][1]*b[1][1]) + (a[2][2]*b[2][1]) + (a[2][3]*b[2][1]);
	m[2][2] = (a[2][0]*b[0][2]) + (a[2][1]*b[1][2]) + (a[2][2]*b[2][2]) + (a[2][3]*b[2][2]);
	m[2][3] = (a[2][0]*b[0][3]) + (a[2][1]*b[1][3]) + (a[2][2]*b[2][3]) + (a[2][3]*b[2][3]);

	m[3][0] = (a[3][0]*b[0][0]) + (a[3][1]*b[1][0]) + (a[3][2]*b[2][0]) + (a[3][3]*b[2][0]);
	m[3][1] = (a[3][0]*b[0][1]) + (a[3][1]*b[1][1]) + (a[3][2]*b[2][1]) + (a[3][3]*b[2][1]);
	m[3][2] = (a[3][0]*b[0][2]) + (a[3][1]*b[1][2]) + (a[3][2]*b[2][2]) + (a[3][3]*b[2][2]);
	m[3][3] = (a[3][0]*b[0][3]) + (a[3][1]*b[1][3]) + (a[3][2]*b[2][3]) + (a[3][3]*b[2][3]);


	return m;
}

#define MATSIZE 3
/*
* https://www.studypug.com/algebra-help/2-x-2-invertible-matrix#:~:text=An%20invertible%20matrix%20is%20a,the%20matrix%20is%20not%200.
*/
Matrix4x4 GetCofactors(Matrix4x4 o, int p, int q, int n)
{
	Matrix4x4 cofs;

	std::size_t i = 0;
	std::size_t j = 0;

	for (std::size_t y = 0; y < n; y++)
		for (std::size_t x = 0; x < n; x++)
		{
			if ((y != p) && (x != q))
				cofs[i][j++] = o[y][x];

			if (j == n-1)
			{
				j = 0;
				i++;
			}
		}

	return cofs;
}

double Determinant(Matrix4x4 o, int n)
{
	double d = 0;
	double sign = 1;

	if (n == 1)
		return o[0][0];

	for (std::size_t x = 0; x < n; x++)
	{
		Matrix4x4 cofs = GetCofactors(o, 0, x, n);

		d += sign * o[0][x] * Determinant(cofs, n-1);
		sign = -sign;
	}

	return d;
}

Matrix4x4 Adjoint(Matrix4x4 o)
{
	Matrix4x4 adj;
	double sign = 1;
	
	for (std::size_t i = 0; i < MATSIZE; i++)
		for (std::size_t j = 0; j < MATSIZE; j++)
		{
			Matrix4x4 cofs = GetCofactors(o, i, j, MATSIZE);

			// sign of adj[j][i] positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj[j][i] = sign * (Determinant(cofs, MATSIZE-1));
		}

	return adj;
}

#include <Windows.h> // beep on error
Matrix4x4 InverseMatrix(Matrix4x4 o)
{
	Matrix4x4 inv;

	double det = Determinant(o, MATSIZE);
	if (det == 0.0)
	{
		Beep(500, 500);
		return Matrix4x4();
	}

	Matrix4x4 adj = Adjoint(o);

	for (std::size_t i = 0; i < MATSIZE; i++)
		for (std::size_t j = 0; j < MATSIZE; j++)
			inv[i][j] = adj[i][j] / det;

	return inv;
}

Transform::Transform()
{
	scaleMatrix.a = 1;
	scaleMatrix.f = 1;
	scaleMatrix.k = 1;
	scaleMatrix.p = 1;

	translationMatrix.a = 1;
	translationMatrix.f = 1;
	translationMatrix.k = 1;
	translationMatrix.p = 1;

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
	// Prevent alabama hierarchy
	for (Transform* it = newParent; it != nullptr; it = it->GetParent())
		if (it == this)
			return; // Abort

	// Save global transform
	//const Vector3d globalPosition = GetGlobalPosition();
	//const Vector3d globalScale = GetLossyScale();
	//const Quaternion globalRotation = GetGlobalRotation();

	//Matrix4x4 pOldParentGlobalTransform;
	//if (parent != nullptr)
	//	pOldParentGlobalTransform = parent->GetGlobalTransformationMatrix();
	//else
	//{
	//	pOldParentGlobalTransform.a = 1;
	//	pOldParentGlobalTransform.f = 1;
	//	pOldParentGlobalTransform.k = 1;
	//	pOldParentGlobalTransform.p = 1;
	//}
	//const Matrix4x4 localTransform = GetLocalTransformationMatrix();

	const Matrix4x4 localInWorld = GetGlobalTransformationMatrix();
	const Quaternion oldRotation = GetGlobalRotation();

	// Say goodbye to current parent, if not an orphan
	if (parent != nullptr)
		parent->children.erase(this);

	// Assign new parent, and notify it (if the parent an actual parent)
	parent = newParent;
	if (parent != nullptr)
		parent->children.insert(this);

	// Invalidate caches
	InvalidateLocalTransform();

	Matrix4x4 pNewParentGlobalTransform;
	if (parent != nullptr)
		pNewParentGlobalTransform = parent->GetGlobalTransformationMatrix();
	else
	{
		pNewParentGlobalTransform.a = 1;
		pNewParentGlobalTransform.f = 1;
		pNewParentGlobalTransform.k = 1;
		pNewParentGlobalTransform.p = 1;
	}

	const Matrix4x4 inv_pNewParentGlobalTransform = InverseMatrix(pNewParentGlobalTransform);
	inv_pNewParentGlobalTransform.d = -pNewParentGlobalTransform.d;
	inv_pNewParentGlobalTransform.h = -pNewParentGlobalTransform.h;
	inv_pNewParentGlobalTransform.l = -pNewParentGlobalTransform.l;
	const Matrix4x4 localInNewParent = inv_pNewParentGlobalTransform * localInWorld;
	
	Vector3d nPos = Vector3d(localInNewParent.d, localInNewParent.h, localInNewParent.l);
	Quaternion nRot = oldRotation;

	if (parent != nullptr)
	{
		nPos = parent->GetGlobalRotation().Inverse() * nPos;

		nRot = oldRotation * parent->GetGlobalRotation().Inverse();
	}

	SetPosition(nPos);
	SetRotation(nRot);

	Quaternion r = oldRotation;
	if (parent != nullptr)
		r = oldRotation * parent->GetRotation().Inverse();

	Matrix4x4 toWorld = localInNewParent;
	//if (parent != nullptr)
	//	toWorld = parent->GetGlobalTransformationMatrix() * localInNewParent;

	Matrix4x4 m = InverseMatrix(r.ToRotationMatrix()) * (toWorld);
	Vector3d nScl = Vector3d(m.a, m.f, m.k);
	SetScale(nScl);
	std::cout << nScl << std::endl;

	//SetGlobalRotation(globalRotation);
	//SetGlobalScale(globalScale);
	//SetGlobalPosition(globalPosition);



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
		cache__localTransformationMatrix = rotation.ToRotationMatrix();
		cache__localTransformationMatrix.p = 1;
		cache__localTransformationMatrix *= scaleMatrix;
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

Vector3d Transform::GetLossyScale() const
{
	// Recalculate if cache is invalid
	if(!cache__IsGlobalTransformationUpToDate)
		RecalculateGlobalTransformCache();

	return cache__LossyScale;
}

void Transform::SetGlobalPosition(const Vector3d& pos)
{
	const Vector3d pGlobalPosition   = (parent != nullptr) ? parent->GetGlobalPosition() : Vector3d::zero;
	const Vector3d pGlobalScale		 = (parent != nullptr) ? parent->GetLossyScale() : Vector3d::one;
	const Quaternion pGlobalRotation = (parent != nullptr) ? parent->GetGlobalRotation() : Quaternion();

	// Init
	SetPosition(pos - pGlobalPosition);

	// Scale
	Matrix4x4 invscale_p;
	invscale_p.a = 1.0 / pGlobalScale.x;
	invscale_p.f = 1.0 / pGlobalScale.y;
	invscale_p.k = 1.0 / pGlobalScale.z;
	SetPosition(GetPosition() * invscale_p);

	// Rotate
	SetPosition(GetPosition() * pGlobalRotation.ToRotationMatrix());

	return;
}

void Transform::SetGlobalScale(const Vector3d& scale)
{
	// parent global scale
	const Vector3d pLossyScale = (parent != nullptr) ? GetLossyScale() : Vector3d::one;

	SetScale(Vector3d(
		scale.x / (pLossyScale.x / GetScale().x),
		scale.y / (pLossyScale.y / GetScale().y),
		scale.z / (pLossyScale.z / GetScale().z)
	));

	return;
}

void Transform::SetGlobalRotation(const Quaternion& rot)
{
	// parent global rotation
	const Quaternion pGlobalRotation = (parent != nullptr) ? parent->GetGlobalRotation() : Quaternion();

	SetRotation(rot * pGlobalRotation.Inverse());

	return;
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

		newPos *= tr->rotation.ToRotationMatrix();

		cache__globalTransformationMatrix.d = newPos.x;
		cache__globalTransformationMatrix.h = newPos.y;
		cache__globalTransformationMatrix.l = newPos.z;
		cache__globalTransformationMatrix = tr->GetLocalTransformationMatrix() * cache__globalTransformationMatrix;
	}

	/*cache__globalTransformationMatrix = GetLocalTransformationMatrix();
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
		cache__globalTransformationMatrix = tr->GetLocalTransformationMatrix() * cache__globalTransformationMatrix;
	}*/


	// Re-Calculate global rotation
	// If we are doing this the other way around (parent -> child, instead of child -> parent),
	// nested rotations (like, camera_yPivot -> camera) break...
	cache__GlobalRotation = Quaternion();
	for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	{
		cache__GlobalRotation *= tr->rotation;
	}

	//cache__LossyScale = Vector3d::one;
	//for (const Transform* tr = this; tr != nullptr; tr = tr->GetParent())
	//{
	//	cache__LossyScale.x *= GetScale().x;
	//	cache__LossyScale.y *= GetScale().y;
	//	cache__LossyScale.z *= GetScale().z;
	//}
	
	// Re-Calculate global position
	cache__GlobalPosition = Vector3d(
		cache__globalTransformationMatrix.d,
		cache__globalTransformationMatrix.h,
		cache__globalTransformationMatrix.l
	);

	// The highest object in this objects hierarchy tree, that is not nullptr
	const Transform* lastParent = nullptr;
	for (const Transform* tr = this->parent; tr != nullptr; tr = tr->GetParent())
		if (tr->GetParent() == nullptr)
			lastParent = tr;

	//if (lastParent != nullptr)
	{
		Matrix4x4 ls_tmp;
		ls_tmp = (cache__GlobalRotation).ToRotationMatrix();
		ls_tmp.d = cache__GlobalPosition.x;
		ls_tmp.h = cache__GlobalPosition.y;
		ls_tmp.l = cache__GlobalPosition.z;

		// Re-Calculate lossy scale
		ls_tmp = InverseMatrix(ls_tmp) * cache__globalTransformationMatrix;
		cache__LossyScale.x = ls_tmp.a;
		cache__LossyScale.y = ls_tmp.f;
		cache__LossyScale.z = ls_tmp.k;
	}
	//else
	//{
	//	cache__LossyScale = GetScale();
	//}

	// Validate cache
	cache__IsGlobalTransformationUpToDate = true;

	return;
}
