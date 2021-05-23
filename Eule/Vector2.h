#pragma once
#include <cstdlib>
#include <sstream>

template <typename T> class Vector3;
template <typename T> class Vector4;

/** Representation of a 2d vector.
* Contains a lot of utility methods.
*/
template <typename T>
class Vector2
{
public:
	Vector2() : x{ 0 }, y{ 0 } {}
	Vector2(T _x, T _y) : x{ _x }, y{ _y } {}
	Vector2(const Vector2<T>& other)     = default;
	Vector2(Vector2<T>&& other) noexcept = default;

	//! Will compute the dot product to another Vector2
	double DotProduct(const Vector2<T>& other) const;

	//! Will compute the cross product to another Vector2
	double CrossProduct(const Vector2<T>& other) const;

	//! Will compute the square magnitude
	double SqrMagnitude() const;

	//! Will compute the magnitude
	double Magnitude() const;

	//! Will return the normalization of this vector
	[[nodiscard]] Vector2<double> Normalize() const;

	//! Will normalize this vector
	void NormalizeSelf();

	//! Will scale self.n by scalar.n
	Vector2<T> VectorScale(const Vector2<T>& scalar) const;

	//! Will lerp itself towards other by t
	void LerpSelf(const Vector2<T>& other, double t);

	//! Will return a lerp result between this and another vector
	[[nodiscard]] Vector2<double> Lerp(const Vector2<T>& other, double t) const;

	//! Will compare if two vectors are similar to a certain epsilon value
	[[nodiscard]] bool Similar(const Vector2<T>& other, double epsilon = 0.00001) const;

	//! Will convert this vector to a Vector2i
	[[nodiscard]] Vector2<int> ToInt() const;

	//! Will convert this vector to a Vector2d
	[[nodiscard]] Vector2<double> ToDouble() const;

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
	Vector2<T> operator-() const;

	operator Vector3<T>() const; //! Conversion method
	operator Vector4<T>() const; //! Conversion method
	
	void operator=(const Vector2<T>& other);
	void operator=(Vector2<T>&& other) noexcept;

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
