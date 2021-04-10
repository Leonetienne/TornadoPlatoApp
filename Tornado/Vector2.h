#pragma once
#include <cstdlib>
#include <sstream>

template <typename T> class Vector3;
template <typename T> class Vector4;

template <typename T>
class Vector2
{
public:
	Vector2() : x{ 0 }, y{ 0 } {}
	Vector2(T _x, T _y) : x{ _x }, y{ _y } {}

	double DotProduct(const Vector2<T>& other) const;
	double CrossProduct(const Vector2<T>& other) const;
	double SqrMagnitude() const;
	double Magnitude() const;
	void Normalize();
	
	// Will lerp itself towards other by t
	void LerpSelf(const Vector2<T>& other, double t);
	// Will lerp relative to this vector
	Vector2<double> Lerp(const Vector2<T>& other, double t) const;
	// Will lerp between a and b
	static Vector2<double> Lerp(const Vector2<T>& a, const Vector2<T>& b, double t);

	// Will compare if two vectors are similar to a certain epsilon value
	bool Similar(const Vector2<T>& other, double epsilon = 0.00001) const;

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

	operator Vector3<T>() const;
	operator Vector4<T>() const;
	
	void operator=(const Vector2<T>& other);

	bool operator==(const Vector2<T>& other) const;
	bool operator!=(const Vector2<T>& other) const;

	friend std::ostream& operator<< (std::ostream& os, const Vector2<T>& v)
	{
		return os << "[x: " << v.x << "  y: " << v.y << "]";
	}
	friend std::wostream& operator<< (std::wostream& os, const Vector2<T>& v)
	{
		return os << L"[x: " << v.x << L"  y: " << v.y << L"]";
	}

	T x;
	T y;

	// Some handy predefines
	static const Vector2<double> up;
	static const Vector2<double> down;
	static const Vector2<double> right;
	static const Vector2<double> left;
	static const Vector2<double> one;
	static const Vector2<double> zero;
};

typedef Vector2<int> Vector2i;
typedef Vector2<double> Vector2d;
