#include "Vector4.h"
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
	int iSqrMag = x*x + y*y + z*z + w*w;
	return (double)iSqrMag;
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



// Good, optimized chad version for doubles
void Vector4<double>::LerpSelf(const Vector4<double>& other, double t)
{
	double t1 = 1.0f - t;

	x = t1 * x + t * other.x;
	y = t1 * y + t * other.y;
	z = t1 * z + t * other.z;
	w = t1 * w + t * other.w;

	return;
}

// Slow, lame version for intcels
void Vector4<int>::LerpSelf(const Vector4<int>& other, double t)
{
	double t1 = 1.0f - t;

	x = (int)(t1 * (double)x + t * (double)other.x);
	y = (int)(t1 * (double)y + t * (double)other.y);
	z = (int)(t1 * (double)z + t * (double)other.z);
	w = (int)(t1 * (double)w + t * (double)other.w);

	return;
}

template<typename T>
Vector4<double> Vector4<T>::Lerp(const Vector4<T>& other, double t) const
{
	double t1 = 1.0 - t;

	return Vector4<double>(
		t1 * x + t * other.x,
		t1 * y + t * other.y,
		t1 * z + t * other.z,
		t1 * w + t * other.w
	);
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

template<typename T>
bool Vector4<T>::operator==(const Vector4<T>& other) const
{
	return
		(x == other.x) &&
		(y == other.y) &&
		(z == other.z) &&
		(w == other.w);
}



// Good, optimized chad version for doubles
Vector4<double> Vector4<double>::operator*(const Matrix4x4 mat) const
{
	Vector4<double> newVec;
	
	newVec.x = (mat[0][0] * x) + (mat[0][1] * y) + (mat[0][2] * z) + (mat[0][3] * w);
	newVec.y = (mat[1][0] * x) + (mat[1][1] * y) + (mat[1][2] * z) + (mat[1][3] * w);
	newVec.z = (mat[2][0] * x) + (mat[2][1] * y) + (mat[2][2] * z) + (mat[2][3] * w);
	newVec.w = (mat[3][0] * x) + (mat[3][1] * y) + (mat[3][2] * z) + (mat[3][3] * w);

	return newVec;
}

// Slow, lame version for intcels
Vector4<int> Vector4<int>::operator*(const Matrix4x4 mat) const
{
	Vector4<double> newVec;

	newVec.x = (mat[0][0] * x) + (mat[0][1] * y) + (mat[0][2] * z) + (mat[0][3] * w);
	newVec.y = (mat[1][0] * x) + (mat[1][1] * y) + (mat[1][2] * z) + (mat[1][3] * w);
	newVec.z = (mat[2][0] * x) + (mat[2][1] * y) + (mat[2][2] * z) + (mat[2][3] * w);
	newVec.w = (mat[3][0] * x) + (mat[3][1] * y) + (mat[3][2] * z) + (mat[3][3] * w);

	return Vector4<int>(
		(int)newVec.x,
		(int)newVec.y,
		(int)newVec.z,
		(int)newVec.w
	);
}



// Good, optimized chad version for doubles
void Vector4<double>::operator*=(const Matrix4x4 mat)
{
	Vector4<double> buffer = *this;

	// Should this still be reversed...? like, instead of mat[x][y], use mat[y][m]
	// idk right now. check that if something doesn't work
	x = (mat[0][0] * buffer.x) + (mat[0][1] * buffer.y) + (mat[0][2] * buffer.z) + (mat[0][3] * buffer.w);
	y = (mat[1][0] * buffer.x) + (mat[1][1] * buffer.y) + (mat[1][2] * buffer.z) + (mat[1][3] * buffer.w);
	z = (mat[2][0] * buffer.x) + (mat[2][1] * buffer.y) + (mat[2][2] * buffer.z) + (mat[2][3] * buffer.w);
	w = (mat[3][0] * buffer.x) + (mat[3][1] * buffer.y) + (mat[3][2] * buffer.z) + (mat[3][3] * buffer.w);

	return;
}

template<typename T>
void Vector4<T>::operator=(const Vector4<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;

	return;
}

// Slow, lame version for intcels
void Vector4<int>::operator*=(const Matrix4x4 mat)
{
	Vector4<double> buffer(x, y, z, w);

	// Should this still be reversed...? like, instead of mat[x][y], use mat[y][m]
	// idk right now. check that if something doesn't work
	x = (int)((mat[0][0] * buffer.x) + (mat[0][1] * buffer.y) + (mat[0][2] * buffer.z) + (mat[0][3] * buffer.w));
	y = (int)((mat[1][0] * buffer.x) + (mat[1][1] * buffer.y) + (mat[1][2] * buffer.z) + (mat[1][3] * buffer.w));
	z = (int)((mat[2][0] * buffer.x) + (mat[2][1] * buffer.y) + (mat[2][2] * buffer.z) + (mat[2][3] * buffer.w));
	w = (int)((mat[3][0] * buffer.x) + (mat[3][1] * buffer.y) + (mat[3][2] * buffer.z) + (mat[3][3] * buffer.w));

	return;
}

template<typename T>
bool Vector4<T>::operator!=(const Vector4<T>& other) const
{
	return !operator==(other);
}

template class Vector4<int>;
template class Vector4<double>;
