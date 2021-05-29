#include "Quaternion.h"
#include "Constants.h"

//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
#include <immintrin.h>
#endif

Quaternion::Quaternion()
{
	v = Vector4d(0, 0, 0, 1);
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

	#ifndef _EULE_NO_INTRINSICS_

	// Calculate sine and cos values
	__m256d __vec = _mm256_set_pd(0, eulerRad.z, eulerRad.y, eulerRad.x);
	__vec = _mm256_mul_pd(__vec, _mm256_set1_pd(0.5));
	__m256d __cos;
	__m256d __sin = _mm256_sincos_pd(&__cos, __vec);

	// Create multiplication vectors
	double sin[4];
	double cos[4];

	_mm256_storeu_pd(sin, __sin);
	_mm256_storeu_pd(cos, __cos);

	__m256d __a = _mm256_set_pd(cos[0], cos[0], sin[0], cos[0]);
	__m256d __b = _mm256_set_pd(cos[1], sin[1], cos[1], cos[1]);
	__m256d __c = _mm256_set_pd(sin[2], cos[2], cos[2], cos[2]);

	__m256d __d = _mm256_set_pd(sin[0], sin[0], cos[0], sin[0]);
	__m256d __e = _mm256_set_pd(sin[1], cos[1], sin[1], sin[1]);
	__m256d __f = _mm256_set_pd(cos[2], sin[2], sin[2], sin[2]);

	// Multiply them
	__m256d __abc;
	__abc = _mm256_mul_pd(__a, __b);
	__abc = _mm256_mul_pd(__abc, __c);

	__m256d __def;
	__def = _mm256_mul_pd(__d, __e);
	__def = _mm256_mul_pd(__def, __f);

	// Extract results
	double abc[4];
	double def[4];

	_mm256_storeu_pd(abc, __abc);
	_mm256_storeu_pd(def, __def);

	// Sum them up
	v.w = abc[0] + def[0];
	v.x = abc[1] - def[1];
	v.y = abc[2] + def[2];
	v.z = abc[3] - def[3];

	#else

	const double cy = cos(eulerRad.z * 0.5);
	const double sy = sin(eulerRad.z * 0.5);
	const double cp = cos(eulerRad.y * 0.5);
	const double sp = sin(eulerRad.y * 0.5);
	const double cr = cos(eulerRad.x * 0.5);
	const double sr = sin(eulerRad.x * 0.5);

	v.w = cr * cp * cy + sr * sp * sy;
	v.x = sr * cp * cy - cr * sp * sy;
	v.y = cr * sp * cy + sr * cp * sy;
	v.z = cr * cp * sy - sr * sp * cy;

	#endif

	return;
}

Quaternion::~Quaternion()
{
	return;
}

Quaternion Quaternion::operator= (const Quaternion& q)
{
	InvalidateCache();

	v = q.v;

	return (*this);
}

Quaternion Quaternion::operator* (const Quaternion& q) const
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

Quaternion Quaternion::operator/ (Quaternion& q) const
{
	return ((*this) * (q.Inverse()));
}

Quaternion& Quaternion::operator*= (const Quaternion& q)
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

Quaternion& Quaternion::operator/= (const Quaternion& q)
{
	InvalidateCache();

	(*this) = (*this) * q.Inverse();
	return (*this);
}

Vector3d Quaternion::operator*(const Vector3d& p) const
{
	return RotateVector(p);
}

bool Quaternion::operator== (const Quaternion& q) const
{
	return (v.Similar(q.v)) || (v.Similar(q.v * -1));
}

bool Quaternion::operator!= (const Quaternion& q) const
{
	return (!v.Similar(q.v)) && (!v.Similar(q.v * -1));
}

Quaternion Quaternion::Inverse() const
{
	if (!isCacheUpToDate_inverse)
	{
		cache_inverse = (Conjugate() * (1.0 / v.SqrMagnitude())).v;

		isCacheUpToDate_inverse = true;
	}

	return Quaternion(cache_inverse);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(Vector4d(-v.x, -v.y, -v.z, v.w));
}

Quaternion Quaternion::UnitQuaternion() const
{
	return (*this) * (1.0 / v.Magnitude());
}

Vector3d Quaternion::RotateVector(const Vector3d& vec) const
{
	Quaternion pure(Vector4d(vec.x, vec.y, vec.z, 0));

	//Quaternion f = Conjugate() * pure * (*this);
	//Quaternion f = Inverse().Conjugate() * pure * (this->Inverse());
	
	
	Quaternion f = Inverse() * pure * (*this);

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

		cache_euler = euler;
		isCacheUpToDate_matrix = true;
	}

	return cache_euler;
}

Matrix4x4 Quaternion::ToRotationMatrix() const
{
	if (!isCacheUpToDate_matrix)
	{
		Matrix4x4 m;

		const double sqx = v.x * v.x;
		const double sqy = v.y * v.y;
		const double sqz = v.z * v.z;
		const double sqw = v.w * v.w;
		const double x = v.x;
		const double y = v.y;
		const double z = v.z;
		const double w = v.w;

		// invs (inverse square length) is only required if quaternion is not already normalised
		double invs = 1.0 / (sqx + sqy + sqz + sqw);
		
		// since sqw + sqx + sqy + sqz =1/invs*invs

		// yaw (y)
		m.c = ((2 * x * z) - (2 * w * y)) * invs;
		m.f = (1 - (2 * sqx) - (2 * sqz)) * invs;
		m.i = ((2 * x * z) + (2 * w * y)) * invs;

		// pitch (x)
		m.a = (1 - (2 * sqy) - (2 * sqz)) * invs;
		m.g = ((2 * y * z) + (2 * w * x)) * invs;
		m.j = ((2 * y * z) - (2 * w * x)) * invs;

		// roll (z)
		m.b = ((2 * x * v.y) + (2 * w * z)) * invs;
		m.e = ((2 * x * v.y) - (2 * w * z)) * invs;
		m.k = (1 - (2 * sqx) - (2 * sqy)) * invs;

		m.p = 1;
		
		cache_matrix = m;
		isCacheUpToDate_matrix = true;
	}

	return cache_matrix;
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

Quaternion Quaternion::Lerp(const Quaternion& other, double t) const
{
	return Quaternion(v.Lerp(other.v, t)).UnitQuaternion();
}

void Quaternion::InvalidateCache()
{
	isCacheUpToDate_euler = false;
	isCacheUpToDate_matrix = false;
	isCacheUpToDate_inverse = false;

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
