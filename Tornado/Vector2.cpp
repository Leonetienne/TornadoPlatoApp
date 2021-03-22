#include "Vector2.h"
#include <iostream>
#include <exception>

// Good, optimized chad version for doubles
double Vector2<double>::DotProduct(const Vector2<double>& other)
{
	return (x * other.x) +
		   (y * other.y);
}

// Slow, lame version for intcels
double Vector2<int>::DotProduct(const Vector2<int>& other)
{
	int iDot = (x * other.x) + (y * other.y);
	return (double)iDot;
}



// Good, optimized chad version for doubles
double Vector2<double>::CrossProduct(const Vector2<double>& other)
{
	return (x * other.y) -
		   (y * other.x);
}

// Slow, lame version for intcels
double Vector2<int>::CrossProduct(const Vector2<int>& other)
{
	int iCross = (x * other.y) - (y * other.x);
	return (double)iCross;
}



// Good, optimized chad version for doubles
double Vector2<double>::SqrMagnitude()
{
	return (x * x) +
		   (y * y);
}

// Slow, lame version for intcels
double Vector2<int>::SqrMagnitude()
{
	int iSqrtMag = x*x + y*y;
	return (double)iSqrtMag;
}

template<typename T>
double Vector2<T>::Magnitude()
{
	return sqrt(SqrMagnitude());
}



// Method to normalize a Vector2d
void Vector2<double>::Normalize()
{
	double length = Magnitude();

	// Prevent division by 0
	if (length == 0)
	{
		x = 0;
		y = 0;
	}
	else
	{
		x /= length;
		y /= length;
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
void Vector2<int>::Normalize()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;

	return;
}



template<typename T>
Vector2<double> Vector2<T>::Lerp(const Vector2<T>& other, double t)
{
	return Vector2::Lerp(*this, other, t);
}

template<typename T>
Vector2<double> Vector2<T>::Lerp(const Vector2<T>& a, const Vector2<T>& b, double t)
{
	double t1 = 1.0f - t;

	return Vector2<double>(
		t1 * a.x + t * b.x,
		t1 * a.y + t * b.y
	);
}



template<typename T>
T& Vector2<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Array descriptor on Vector2<T> out of range!");
	}
}

template<typename T>
const T& Vector2<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	default:
		throw std::out_of_range("Array descriptor on Vector2<T> out of range!");
	}
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	return Vector2<T>(
		x + other.x,
		y + other.y
	);
}

template<typename T>
void  Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;
	return;
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	return Vector2<T>(
		x - other.x,
		y - other.y
	);
}

template<typename T>
void Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;
	return;
}

template<typename T>
Vector2<T> Vector2<T>::operator*(const T scale) const
{
	return Vector2<T>(
		x * scale,
		y * scale
	);
}

template<typename T>
void Vector2<T>::operator*=(const T scale)
{
	x *= scale;
	y *= scale;
	return;
}

template<typename T>
Vector2<T> Vector2<T>::operator/(const T scale) const
{
	return Vector2<T>(
		x / scale,
		y / scale
	);
}

template<typename T>
void Vector2<T>::operator/=(const T scale)
{
	x /= scale;
	y /= scale;
	return;
}

template<typename T>
bool Vector2<T>::operator==(const Vector2<T>& other) const
{
	return
		(x == other.x) &&
		(y == other.y);
}

template<typename T>
bool Vector2<T>::operator!=(const Vector2<T>& other) const
{
	return !operator==(other);
}

template class Vector2<int>;
template class Vector2<double>;
