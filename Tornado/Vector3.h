#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>
#include "Matrix4x4.h"

template <typename T>
class Vector3
{
public:
	Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
	Vector3(T _x, T _y, T _z) : x{ _x }, y{ _y }, z{ _z } {}

	double DotProduct(const Vector3<T>& other);
	Vector3<double> CrossProduct(const Vector3<T>& other);
	double SqrMagnitude();
	double Magnitude();
	void Normalize();
	Vector3<double> Lerp(const Vector3<T>& other, double t);
	static Vector3<double> Lerp(const Vector3<T>& a, const Vector3<T>& b, double t);

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

	friend std::ostream& operator << (std::ostream& os, const Vector3<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "]";
	}

	T x;
	T y;
	T z;
};

typedef Vector3<int> Vector3i;
typedef Vector3<double> Vector3d;
