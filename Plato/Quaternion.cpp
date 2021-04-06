#include "Quaternion.h"
#include "../Tornado/Constants.h"

Quaternion::Quaternion()
{
	return;
}

Quaternion::Quaternion(const Vector4d values)
{
	v = values;
	return;
}

Quaternion::Quaternion(const Quaternion& q)
{
	v = q.v;
	return;
}

Quaternion::Quaternion(const Vector3d eulerAngles)
{
	Vector3d eulerRad = eulerAngles * Deg2Rad;

	double cy = cos(eulerRad.z * 0.5);
	double sy = sin(eulerRad.z * 0.5);
	double cp = cos(eulerRad.y * 0.5);
	double sp = sin(eulerRad.y * 0.5);
	double cr = cos(eulerRad.x * 0.5);
	double sr = sin(eulerRad.x * 0.5);

	v.w = cr * cp * cy + sr * sp * sy;
	v.x = sr * cp * cy - cr * sp * sy;
	v.y = cr * sp * cy + sr * cp * sy;
	v.z = cr * cp * sy - sr * sp * cy;

	return;
}

Quaternion::Quaternion(const Matrix4x4 rotMatrix)
{
	const Matrix4x4& m = rotMatrix; // easier to read;

	double tr = m.a + m.f + m.k;

	if (tr > 0) {
		double S = sqrt(tr + 1.0) * 2;
		v.w = 0.25 * S;
		v.x = (m.j - m.g) / S;
		v.y = (m.c - m.i) / S;
		v.z = (m.e - m.b) / S;
	}
	else if ((m.a > m.f) & (m.a > m.k)) {
		double S = sqrt(1.0 + m.a - m.f - m.k) * 2;
		v.w = (m.j - m.g) / S;
		v.x = 0.25 * S;
		v.y = (m.b + m.e) / S;
		v.z = (m.c + m.i) / S;
	}
	else if (m.f > m.k) {
		double S = sqrt(1.0 + m.f - m.a - m.k) * 2;
		v.w = (m.c - m.i) / S;
		v.x = (m.b + m.e) / S;
		v.y = 0.25 * S;
		v.z = (m.g + m.j) / S;
	}
	else {
		double S = sqrt(1.0 + m.k - m.a - m.f) * 2;
		v.w = (m.e - m.b) / S;
		v.x = (m.c + m.i) / S;
		v.y = (m.g + m.j) / S;
		v.z = 0.25 * S;
	}

	return;
}

Quaternion::~Quaternion()
{

	return;
}

Quaternion Quaternion::operator = (const Quaternion& q)
{
	InvalidateCache();

	v = q.v;

	return (*this);
}

Quaternion Quaternion::operator * (const Quaternion& q) const
{
	return Quaternion(Vector4d(
		v.w * q.v.x + v.x * q.v.w + v.y * q.v.z - v.z * q.v.y,
		v.w * q.v.y + v.y * q.v.w + v.z * q.v.x - v.x * q.v.z,
		v.w * q.v.z + v.z * q.v.w + v.x * q.v.y - v.y * q.v.x,
		v.w * q.v.w - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z
	));
}

Quaternion Quaternion::operator*(const double scale) const
{
	return Quaternion(v * scale);
}

Quaternion Quaternion::operator / (Quaternion& q) const
{
	return ((*this) * (q.Inverse()));
}

Quaternion& Quaternion::operator *= (const Quaternion& q)
{
	InvalidateCache();

	Vector4d bufr = v;
	v.x = bufr.w * q.v.x + bufr.x * q.v.w + bufr.y * q.v.z - bufr.z * q.v.y; // x
	v.y = bufr.w * q.v.y + bufr.y * q.v.w + bufr.z * q.v.x - bufr.x * q.v.z; // y
	v.z = bufr.w * q.v.z + bufr.z * q.v.w + bufr.x * q.v.y - bufr.y * q.v.x; // z
	v.w = bufr.w * q.v.w - bufr.x * q.v.x - bufr.y * q.v.y - bufr.z * q.v.z; // w

	return (*this);
}

Quaternion& Quaternion::operator*=(const double scale)
{
	InvalidateCache();

	v *= scale;
	return (*this);
}

Quaternion& Quaternion::operator /= (const Quaternion& q)
{
	InvalidateCache();

	(*this) = (*this) * q.Inverse();
	return (*this);
}

Vector3d Quaternion::operator*(const Vector3d& p)
{
	return RotateVector(p);
}

bool Quaternion::operator == (const Quaternion& q) const
{
	return ((v.w == q.v.w) || (v.x == q.v.x) || (v.y == q.v.y) || (v.z == q.v.z));
}

bool Quaternion::operator != (const Quaternion& q) const
{
	return ((v.w != q.v.w) || (v.x != q.v.x) || (v.y != q.v.y) || (v.z != q.v.z));
}

Quaternion Quaternion::Inverse() const
{
	return Conjugate() * (1.0 / v.SqrMagnitude());
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(Vector4d(-v.x, -v.y, -v.z, v.w));
}

Quaternion Quaternion::UnitQuaternion() const
{
	return (*this) * (1.0 / v.Magnitude());
}

Vector3d Quaternion::RotateVector(const Vector3d vec) const
{
	Quaternion pure(Vector4d(vec.x, vec.y, vec.z, 0));

	Quaternion f = (*this) * pure * Inverse();

	Vector3d toRet;
	toRet.x = f.v.x;
	toRet.y = f.v.y;
	toRet.z = f.v.z;

	return toRet;
}

Vector3d Quaternion::ToEulerAngles() const
{
	if (!isCacheUpToDate_euler)
	{
		Vector3d euler;
		// roll (x-axis rotation)
		double sinr_cosp = 2.0 * (v.w * v.x + v.y * v.z);
		double cosr_cosp = 1.0 - 2.0 * (v.x * v.x + v.y * v.y);
		euler.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2.0 * (v.w * v.y - v.z * v.x);
		if (std::abs(sinp) >= 1)
			euler.y = std::copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
			euler.y = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2.0 * (v.w * v.z + v.x * v.y);
		double cosy_cosp = 1.0 - 2.0 * (v.y * v.y + v.z * v.z);
		euler.z = std::atan2(siny_cosp, cosy_cosp);

		euler *= Rad2Deg;

		eulerCache = euler;
		isCacheUpToDate_matrix = true;
	}

	return eulerCache;
}

Matrix4x4 Quaternion::ToRotationMatrix() const
{
	if (!isCacheUpToDate_matrix)
	{
		Matrix4x4 m;

		double sqw = v.w * v.w;
		double sqx = v.x * v.x;
		double sqy = v.y * v.y;
		double sqz = v.z * v.z;

		// invs (inverse square length) is only required if quaternion is not already normalised
		double invs = 1.0 / (sqx + sqy + sqz + sqw);
		m.a = (1 - (2 * sqy) - (2 * sqz)) * invs; // since sqw + sqx + sqy + sqz =1/invs*invs
		m.f = (1 - (2 * sqx) - (2 * sqz)) * invs;
		m.k = (1 - (2 * sqx) - (2 * sqy)) * invs;

		m.e = ((2 * v.x * v.y) - (2 * v.w * v.z)) * invs;
		m.b = ((2 * v.x * v.y) + (2 * v.w * v.z)) * invs;

		m.i = ((2 * v.x * v.z) + (2 * v.w * v.y)) * invs;
		m.c = ((2 * v.x * v.z) - (2 * v.w * v.y)) * invs;

		m.j = ((2 * v.y * v.z) - (2 * v.w * v.x)) * invs;
		m.g = ((2 * v.y * v.z) + (2 * v.w * v.x)) * invs;

		matrixCache = m;
		isCacheUpToDate_matrix = true;
	}

	return matrixCache;
}

Vector4d Quaternion::GetRawValues() const
{
	return v;
}

Quaternion Quaternion::AngleBetween(const Quaternion& other) const
{
	return other * Conjugate();
}

void Quaternion::SetRawValues(const Vector4d values)
{
	InvalidateCache();

	v = values;

	return;
}

Quaternion Quaternion::FromMatrix(const Matrix4x4 mat)
{
	return Quaternion(mat);
}

Quaternion Quaternion::FromEuler(const Vector3d euler)
{
	return Quaternion(euler);
}

void Quaternion::InvalidateCache()
{
	isCacheUpToDate_euler = false;
	isCacheUpToDate_matrix = false;
	return;
}

std::ostream& operator << (std::ostream& os, const Quaternion& q)
{
	os << "[" << q.v << "]";
	return os;
}

std::wostream& operator<<(std::wostream& os, const Quaternion& q)
{
	os << L"[" << q.v << L"]";
	return os;
}
