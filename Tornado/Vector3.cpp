#include "Vector3.h"
#include <iostream>

// Good, optimized chad version for doubles
double Vector3<double>::DotProduct(const Vector3<double>& other)
{
	return (x * other.x) +
		   (y * other.y) + 
		   (z * other.z);
}

// Slow, lame version for intcels
double Vector3<int>::DotProduct(const Vector3<int>& other)
{
	int iDot = (x * other.x) + (y * other.y) + (z * other.z);
	return (double)iDot;
}



// Good, optimized chad version for doubles
Vector3<double> Vector3<double>::CrossProduct(const Vector3<double>& other)
{
	Vector3<double> cp;
	cp.x = (y * other.z) - (z * other.y);
	cp.y = (z * other.x) - (x * other.z);
	cp.z = (x * other.y) - (y * other.x);

	return cp;
}

// Slow, lame version for intcels
Vector3<double> Vector3<int>::CrossProduct(const Vector3<int>& other)
{
	Vector3<double> cp;
	cp.x = ((double)y * (double)other.z) - ((double)z * (double)other.y);
	cp.y = ((double)z * (double)other.x) - ((double)x * (double)other.z);
	cp.z = ((double)x * (double)other.y) - ((double)y * (double)other.x);

	return cp;
}



// Good, optimized chad version for doubles
double Vector3<double>::SqrMagnitude()
{
	return (x * x) +
		   (y * y) +
		   (z * z);
}

// Slow, lame version for intcels
double Vector3<int>::SqrMagnitude()
{
	int iSqrMag = x*x + y*y + z*z;
	return (double)iSqrMag;
}

template <typename T>
double Vector3<T>::Magnitude()
{
	return sqrt(SqrMagnitude());
}



// Method to normalize a Vector3d
void Vector3<double>::Normalize()
{
	double length = Magnitude();

	// Prevent division by 0
	if (length == 0)
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		x /= length;
		y /= length;
		z /= length;
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
void Vector3<int>::Normalize()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;
	z = 0;

	return;
}



template<typename T>
T& Vector3<T>::operator[](std::size_t idx)
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("Array descriptor on Vector3<T> out of range!");
	}
}

template<typename T>
const T& Vector3<T>::operator[](std::size_t idx) const
{
	switch (idx)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		throw std::out_of_range("Array descriptor on Vector3<T> out of range!");
	}
}

template<typename T>
Vector3<double> Vector3<T>::Lerp(const Vector3<T>& other, double t)
{
	return Vector3::Lerp(*this, other, t);
}

template<typename T>
Vector3<double> Vector3<T>::Lerp(const Vector3<T>& a, const Vector3<T>& b, double t)
{
	double t1 = 1.0f - t;

	return Vector3<double>(
		t1 * a.x + t * b.x,
		t1 * a.y + t * b.y,
		t1 * a.z + t * b.z
	);
}


template<typename T>
Vector3<T> Vector3<T>::operator+(const Vector3<T>& other) const
{
	return Vector3<T>(
		x + other.x,
		y + other.y,
		z + other.z
	);
}

template<typename T>
void  Vector3<T>::operator+=(const Vector3<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return;
}

template<typename T>
Vector3<T> Vector3<T>::operator-(const Vector3<T>& other) const
{
	return Vector3<T>(
		x - other.x,
		y - other.y,
		z - other.z
	);
}

template<typename T>
void Vector3<T>::operator-=(const Vector3<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return;
}

template<typename T>
Vector3<T> Vector3<T>::operator*(const T scale) const
{
	return Vector3<T>(
		x * scale,
		y * scale,
		z * scale
	);
}

template<typename T>
void Vector3<T>::operator*=(const T scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return;
}

template<typename T>
Vector3<T> Vector3<T>::operator/(const T scale) const
{
	return Vector3<T>(
		x / scale,
		y / scale,
		z / scale
	);
}

template<typename T>
void Vector3<T>::operator/=(const T scale)
{
	x /= scale;
	y /= scale;
	z /= scale;
	return;
}

// This is the method for T=double
// This is good. Use this!
Vector3<double> Vector3<double>::operator*(const Matrix4x4& mat) const
{
	Vector3<double> newVec;

	// Rotation, Scaling
	newVec.x = (mat.a * x) + (mat.b * y) + (mat.c * z);
	newVec.y = (mat.e * x) + (mat.f * y) + (mat.g * z);
	newVec.z = (mat.i * x) + (mat.j * y) + (mat.k * z);

	// Translation
	newVec.x += mat.d;
	newVec.y += mat.h;
	newVec.z += mat.l;

	return newVec;
}

// This is just that we have SOME variant for T=int.
// Not optimized and should not be used.
Vector3<int> Vector3<int>::operator*(const Matrix4x4& mat) const
{
	Vector3<int> newVec;

	// Rotation, Scaling
	newVec.x = (int)((mat.a * x) + (mat.b * y) + (mat.c * z));
	newVec.y = (int)((mat.e * x) + (mat.f * y) + (mat.g * z));
	newVec.z = (int)((mat.i * x) + (mat.j * y) + (mat.k * z));

	// Translation
	newVec.x += (int)mat.d;
	newVec.y += (int)mat.h;
	newVec.z += (int)mat.l;

	return newVec;
}

// This is the method for T=double
// This is good. Use this!
void Vector3<double>::operator*=(const Matrix4x4& mat)
{
	Vector3<double> buffer = *this;

	x = (mat.a * buffer.x) + (mat.b * buffer.y) + (mat.c * buffer.z);
	y = (mat.e * buffer.x) + (mat.f * buffer.y) + (mat.g * buffer.z);
	z = (mat.i * buffer.x) + (mat.j * buffer.y) + (mat.k * buffer.z);

	// Translation
	x += mat.d;
	y += mat.h;
	z += mat.l;

	return;
}

// This is just that we have SOME variant for T=int.
// Not optimized and should not be used.
void Vector3<int>::operator*=(const Matrix4x4& mat)
{
	Vector3<double> buffer;
	buffer.x = x;
	buffer.y = y;

	x = (int)((mat.a * buffer.x) + (mat.b * buffer.y) + (mat.c * buffer.z));
	y = (int)((mat.e * buffer.x) + (mat.f * buffer.y) + (mat.g * buffer.z));
	z = (int)((mat.i * buffer.x) + (mat.j * buffer.y) + (mat.k * buffer.z));

	// Translation
	x += (int)mat.d;
	y += (int)mat.h;
	z += (int)mat.l;

	return;
}

template<typename T>
bool Vector3<T>::operator==(const Vector3<T>& other) const
{
	return
		(x == other.x) &&
		(y == other.y) &&
		(z == other.z);
}

template<typename T>
bool Vector3<T>::operator!=(const Vector3<T>& other) const
{
	return !operator==(other);
}

template class Vector3<int>;
template class Vector3<double>;
