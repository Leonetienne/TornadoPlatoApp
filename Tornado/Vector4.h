#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>

template <typename T>
class Vector4
{
public:
	Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
	Vector4(T _x, T _y, T _z, T _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w } {}

	double SqrMagnitude() const;
	double Magnitude() const;
	void Normalize();

	Vector4<T> operator+(const Vector4<T> other) const;
	void operator+=(const Vector4<T> other);
	Vector4<T> operator-(const Vector4<T> other) const;
	void operator-=(const Vector4<T> other);
	Vector4<T> operator*(const T scale) const;
	void operator*=(const T scale);
	Vector4<T> operator/(const T scale) const;
	void operator/=(const T scale);

	friend std::ostream& operator << (std::ostream& os, const Vector4<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "  w: " << v.w << "]";
	}

	T x;
	T y;
	T z;
	T w;
};

typedef Vector4<int> Vector4i;
typedef Vector4<double> Vector4d;
