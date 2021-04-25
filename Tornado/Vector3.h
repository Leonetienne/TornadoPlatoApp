#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>
#include "Matrix4x4.h"

template <typename T> class Vector2;
template <typename T> class Vector4;

template <typename T>
class Vector3
{
public:
	Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
	Vector3(T _x, T _y, T _z) : x{ _x }, y{ _y }, z{ _z } {}

	double DotProduct(const Vector3<T>& other) const;
	Vector3<double> CrossProduct(const Vector3<T>& other) const;
	double SqrMagnitude() const;
	double Magnitude() const;
	[[nodiscard]] Vector3<double> Normalize() const;
	void NormalizeSelf();

	// Will scale self.n by scalar.n
	Vector3<T> VectorScale(const Vector3<T>& scalar) const;

	// Will lerp itself towards other by t
	void LerpSelf(const Vector3<T>& other, double t);
	// Will lerp relative to this vector
	Vector3<double> Lerp(const Vector3<T>& other, double t) const;
	// Will lerp between a and b
	static Vector3<double> Lerp(const Vector3<T>& a, const Vector3<T>& b, double t);

	// Will compare if two vectors are similar to a certain epsilon value
	bool Similar(const Vector3<T>& other, double epsilon = 0.00001) const;

	T& operator[](std::size_t idx);
	const T& operator[](std::size_t idx) const;

	Vector3<T> operator+(const Vector3<T>& other) const;
	void operator+=(const Vector3<T>& other);
	Vector3<T> operator-(const Vector3<T>& other) const;
	void operator-=(const Vector3<T>& other);
	Vector3<T> operator*(const T scale) const;
	void operator*=(const T scale);
	Vector3<T> operator/(const T scale) const;
	void operator/=(const T scale);
	Vector3<T> operator*(const Matrix4x4& mat) const;
	void operator*=(const Matrix4x4& mat);

	operator Vector2<T>() const;
	operator Vector4<T>() const;

	void operator=(const Vector3<T>& other);

	bool operator==(const Vector3<T>& other) const;
	bool operator!=(const Vector3<T>& other) const;

	friend std::ostream& operator << (std::ostream& os, const Vector3<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "]";
	}
	friend std::wostream& operator << (std::wostream& os, const Vector3<T>& v)
	{
		return os << L"[x: " << v.x << L"  y: " << v.y << L"  z: " << v.z << L"]";
	}

	T x;
	T y;
	T z;

	// Some handy predefines
	static const Vector3<double> up;
	static const Vector3<double> down;
	static const Vector3<double> right;
	static const Vector3<double> left;
	static const Vector3<double> forward;
	static const Vector3<double> backward;
	static const Vector3<double> one;
	static const Vector3<double> zero;
};

typedef Vector3<int> Vector3i;
typedef Vector3<double> Vector3d;
