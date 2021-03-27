#include "Vector3.h"
#include <iostream>

/*
	NOTE:
	Here you will find bad, unoptimized methods for T=int.
	This is because the compiler needs a method for each type in each instanciation of the template!
	I can't generalize the methods when heavily optimizing for doubles.
	These functions will get called VERY rarely, if ever at all, for T=int, so it's ok.
	The T=int instanciation only exists to store a value-pair of two ints. Not so-much as a vector in terms of vector calculus.
*/

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



// Good, optimized chad version for doubles
Vector3<double> Vector3<double>::operator*(const Matrix4x4& mat) const
{
	Vector3<double> newVec;

	// Rotation, Scaling
	newVec.x = (mat[0][0] * x) + (mat[1][0] * y) + (mat[2][0] * z);
	newVec.y = (mat[0][1] * x) + (mat[1][1] * y) + (mat[2][1] * z);
	newVec.z = (mat[0][2] * x) + (mat[1][2] * y) + (mat[2][2] * z);

	// Translation
	newVec.x += mat[0][3];
	newVec.y += mat[1][3];
	newVec.z += mat[2][3];

	return newVec;
}

// Slow, lame version for intcels
Vector3<int> Vector3<int>::operator*(const Matrix4x4& mat) const
{
	Vector3<double> newVec;

	// Rotation, Scaling
	newVec.x = ((mat[0][0] * x) + (mat[1][0] * y) + (mat[2][0] * z));
	newVec.y = ((mat[0][1] * x) + (mat[1][1] * y) + (mat[2][1] * z));
	newVec.z = ((mat[0][2] * x) + (mat[1][2] * y) + (mat[2][2] * z));

	// Translation
	newVec.x += mat[0][3];
	newVec.y += mat[1][3];
	newVec.z += mat[2][3];

	return Vector3<int>(
		(int)newVec.x,
		(int)newVec.y,
		(int)newVec.z
	);
}



// Good, optimized chad version for doubles
void Vector3<double>::operator*=(const Matrix4x4& mat)
{
	Vector3<double> buffer = *this;

	x = (mat[0][0] * buffer.x) + (mat[0][1] * buffer.y) + (mat[0][2] * buffer.z);
	y = (mat[1][0] * buffer.x) + (mat[1][1] * buffer.y) + (mat[1][2] * buffer.z);
	z = (mat[2][0] * buffer.x) + (mat[2][1] * buffer.y) + (mat[2][2] * buffer.z);

	// Translation
	x += mat[0][3];
	y += mat[1][3];
	z += mat[2][3];

	return;
}

template<typename T>
void Vector3<T>::operator=(const Vector3<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return;
}

// Slow, lame version for intcels
void Vector3<int>::operator*=(const Matrix4x4& mat)
{
	Vector3<double> buffer(x, y, z);

	x = (int)((mat[0][0] * buffer.x) + (mat[0][1] * buffer.y) + (mat[0][2] * buffer.z));
	y = (int)((mat[1][0] * buffer.x) + (mat[1][1] * buffer.y) + (mat[1][2] * buffer.z));
	z = (int)((mat[2][0] * buffer.x) + (mat[2][1] * buffer.y) + (mat[2][2] * buffer.z));

	// Translation
	x += (int)mat[0][3];
	y += (int)mat[1][3];
	z += (int)mat[2][3];

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
