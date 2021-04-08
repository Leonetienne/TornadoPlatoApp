#include "Vector2.h"
#include "Similar.h"
#include <iostream>
#include <exception>

/*
	NOTE:
	Here you will find bad, unoptimized methods for T=int.
	This is because the compiler needs a method for each type in each instantiation of the template!
	I can't generalize the methods when heavily optimizing for doubles.
	These functions will get called VERY rarely, if ever at all, for T=int, so it's ok.
	The T=int instantiation only exists to store a value-pair of two ints. Not so-much as a vector in terms of vector calculus.
*/

// Good, optimized chad version for doubles
double Vector2<double>::DotProduct(const Vector2<double>& other) const
{
	return (x * other.x) +
		   (y * other.y);
}

// Slow, lame version for intcels
double Vector2<int>::DotProduct(const Vector2<int>& other) const
{
	int iDot = (x * other.x) +
			   (y * other.y);

	return (double)iDot;
}



// Good, optimized chad version for doubles
double Vector2<double>::CrossProduct(const Vector2<double>& other) const
{
	return (x * other.y) -
		   (y * other.x);
}

// Slow, lame version for intcels
double Vector2<int>::CrossProduct(const Vector2<int>& other) const
{
	int iCross = (x * other.y) -
				 (y * other.x);

	return (double)iCross;
}



// Good, optimized chad version for doubles
double Vector2<double>::SqrMagnitude() const
{
	return (x * x) +
		   (y * y);
}

// Slow, lame version for intcels
double Vector2<int>::SqrMagnitude() const
{
	int iSqrMag = x*x + y*y;
	return (double)iSqrMag;
}

template<typename T>
double Vector2<T>::Magnitude() const
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


// Good, optimized chad version for doubles
void Vector2<double>::LerpSelf(const Vector2<double>& other, double t)
{
	double t1 = 1.0f - t;

	x = t1 * x + t * other.x;
	y = t1 * y + t * other.y;
	
	return;
}

// Slow, lame version for intcels
void Vector2<int>::LerpSelf(const Vector2<int>& other, double t)
{
	double t1 = 1.0f - t;

	x = (int)(t1 * (double)x + t * (double)other.x);
	y = (int)(t1 * (double)y + t * (double)other.y);

	return;
}

template<typename T>
Vector2<double> Vector2<T>::Lerp(const Vector2<T>& other, double t) const
{
	double t1 = 1.0f - t;

	return Vector2<double>(
		t1 * x + t * other.x,
		t1 * y + t * other.y
	);
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
bool Vector2<T>::Similar(const Vector2<T>& other, double epsilon) const
{
	return
		(::Similar(x, other.x, epsilon)) &&
		(::Similar(y, other.y, epsilon))
	;
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
void Vector2<T>::operator=(const Vector2<T>& other)
{
	x = other.x;
	y = other.y;

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

// Don't want these includes above the other stuff
#include "Vector3.h"
#include "Vector4.h"
template<typename T>
Vector2<T>::operator Vector3<T>() const
{
	return Vector3<T>(x, y, 0);
}

template<typename T>
Vector2<T>::operator Vector4<T>() const
{
	return Vector4<T>(x, y, 0, 0);
}

template class Vector2<int>;
template class Vector2<double>;
