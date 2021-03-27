#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>
#include "Matrix4x4.h"

template <typename T>
class Vector4
{
public:
	Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
	Vector4(T _x, T _y, T _z, T _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w } {}

	double SqrMagnitude() const;
	double Magnitude() const;
	void Normalize();
	Vector4<double> Lerp(const Vector4<T>& other, double t);
	static Vector4<double> Lerp(const Vector4<T>& a, const Vector4<T>& b, double t);

	T& operator[](std::size_t idx);
	const T& operator[](std::size_t idx) const;

	Vector4<T> operator+(const Vector4<T> other) const;
	void operator+=(const Vector4<T> other);
	Vector4<T> operator-(const Vector4<T> other) const;
	void operator-=(const Vector4<T> other);
	Vector4<T> operator*(const T scale) const;
	void operator*=(const T scale);
	Vector4<T> operator/(const T scale) const;
	void operator/=(const T scale);
	Vector4<T> operator*(const Matrix4x4 mat) const;
	void operator*=(const Matrix4x4 mat);

	void operator=(const Vector4<T>& other);

	bool operator==(const Vector4<T>& other) const;
	bool operator!=(const Vector4<T>& other) const;

	friend std::ostream& operator << (std::ostream& os, const Vector4<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "  w: " << v.w << "]";
	}
	friend std::wostream& operator << (std::wostream& os, const Vector4<T>& v)
	{
		return os << L"[x: " << v.x << L"  y: " << v.y << L"  z: " << v.z << L"  w: " << v.w << L"]";
	}

	T x;
	T y;
	T z;
	T w;
};

typedef Vector4<int> Vector4i;
typedef Vector4<double> Vector4d;
