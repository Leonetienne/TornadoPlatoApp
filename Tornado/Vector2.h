#pragma once
#include <cstdlib>
#include <sstream>

template <typename T>
class Vector2
{
public:
	Vector2() : x{ 0 }, y{ 0 } {}
	Vector2(T _x, T _y) : x{ _x }, y{ _y } {}

	double DotProduct(const Vector2<T>& other);
	double CrossProduct(const Vector2<T>& other);
	double SqrMagnitude();
	double Magnitude();
	void Normalize();
	Vector2<double> Lerp(const Vector2<T>& other, double t);
	static Vector2<double> Lerp(const Vector2<T>& a, const Vector2<T>& b, double t);

	T& operator[](std::size_t idx);
	const T& operator[](std::size_t idx) const;

	Vector2<T> operator+(const Vector2<T>& other) const;
	void operator+=(const Vector2<T>& other);
	Vector2<T> operator-(const Vector2<T>& other) const;
	void operator-=(const Vector2<T>& other);
	Vector2<T> operator*(const T scale) const;
	void operator*=(const T scale);
	Vector2<T> operator/(const T scale) const;
	void operator/=(const T scale);
	
	void operator=(const Vector2<T>& other);

	bool operator==(const Vector2<T>& other) const;
	bool operator!=(const Vector2<T>& other) const;

	friend std::ostream& operator << (std::ostream& os, const Vector2<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "]";
	}
	friend std::wostream& operator << (std::wostream& os, const Vector2<T>& v)
	{
		return os << L"[x: " << v.x << L"  y: " << v.y << L"]";
	}

	T x;
	T y;
};

typedef Vector2<int> Vector2i;
typedef Vector2<double> Vector2d;
