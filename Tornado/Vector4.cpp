#include "Vector4.h"
#include <iostream>

// Good, optimized chad version for doubles
double Vector4<double>::SqrMagnitude() const
{
	return (x * x) +
		   (y * y) +
		   (z * z) +
		   (w * w);
}

// Slow, lame version for intcels
double Vector4<int>::SqrMagnitude() const
{
	return ((double)x * (double)x) +
		   ((double)y * (double)y) +
		   ((double)z * (double)z) +
		   ((double)w * (double)w);
}

template<typename T>
double Vector4<T>::Magnitude() const
{
	return sqrt(SqrMagnitude());
}



// Method to normalize a Vector43d
void Vector4<double>::Normalize()
{
	double length = Magnitude();

	// Prevent division by 0
	if (length == 0)
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	else
	{
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
void Vector4<int>::Normalize()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;
	z = 0;
	w = 0;

	return;
}



template<typename T>
T& Vector4<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Array descriptor on Vector4<T> out of range!");
	}
}

template<typename T>
const T& Vector4<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	default:
		throw std::out_of_range("Array descriptor on Vector4<T> out of range!");
	}
}

template<typename T>
Vector4<double> Vector4<T>::Lerp(const Vector4<T>& other, double t)
{
	return Vector4::Lerp(*this, other, t);
}

template<typename T>
Vector4<double> Vector4<T>::Lerp(const Vector4<T>& a, const Vector4<T>& b, double t)
{
	double t1 = 1.0f - t;

	return Vector4<double>(
		t1 * a.x + t * b.x,
		t1 * a.y + t * b.y,
		t1 * a.z + t * b.z,
		t1 * a.w + t * b.w
	);
}



template<typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T> other) const
{
	return Vector4<T>(
		x + other.x,
		y + other.y,
		z + other.z,
		w + other.w
	);
}

template<typename T>
void  Vector4<T>::operator+=(const Vector4<T> other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return;
}

template<typename T>
Vector4<T> Vector4<T>::operator-(const Vector4<T> other) const
{
	return Vector4<T>(
		x - other.x,
		y - other.y,
		z - other.z,
		w - other.w
	);
}

template<typename T>
void Vector4<T>::operator-=(const Vector4<T> other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return;
}

template<typename T>
Vector4<T> Vector4<T>::operator*(const T scale) const
{
	return Vector4<T>(
		x * scale,
		y * scale,
		z * scale,
		w * scale
	);
}

template<typename T>
void Vector4<T>::operator*=(const T scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	return;
}

template<typename T>
Vector4<T> Vector4<T>::operator/(const T scale) const
{
	return Vector4<T>(
		x / scale,
		y / scale,
		z / scale,
		w / scale
	);
}

template<typename T>
void Vector4<T>::operator/=(const T scale)
{
	x /= scale;
	y /= scale;
	z /= scale;
	w /= scale;

	return;
}

template class Vector4<int>;
template class Vector4<double>;
