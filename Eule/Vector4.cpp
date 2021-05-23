#include "Vector4.h"
#include "Math.h"
#include <iostream>

//#define _TORNADO_NO_INTRINSICS_
#ifndef _TORNADO_NO_INTRINSICS_
#include <immintrin.h>
#endif

/*
	NOTE:
	Here you will find bad, unoptimized methods for T=int.
	This is because the compiler needs a method for each type in each instantiation of the template!
	I can't generalize the methods when heavily optimizing for doubles.
	These functions will get called VERY rarely, if ever at all, for T=int, so it's ok.
	The T=int instantiation only exists to store a value-pair of two ints. Not so-much as a vector in terms of vector calculus.
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


Vector4<double> Vector4<double>::VectorScale(const Vector4<double>& scalar) const
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Load vectors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_scalar = _mm256_set_pd(scalar.w, scalar.z, scalar.y, scalar.x);

	// Multiply them
	__m256d __product = _mm256_mul_pd(__vector_self, __vector_scalar);

	// Retrieve result
	double result[4];
	_mm256_storeu_pd(result, __product);

	// Return value
	return Vector4<double>(
		result[0],
		result[1],
		result[2],
		result[3]
		);

	#else

	return Vector4<double>(
			x * scalar.x,
			y * scalar.y,
			z * scalar.z,
			w * scalar.w
		);
	#endif
}


Vector4<int> Vector4<int>::VectorScale(const Vector4<int>& scalar) const
{
	return Vector4<int>(
			x * scalar.x,
			y * scalar.y,
			z * scalar.z,
			w * scalar.w
		);
}



template<typename T>
Vector4<double> Vector4<T>::Normalize() const
{
	Vector4<double> norm(x, y, z, w);
	norm.NormalizeSelf();

	return norm;
}

// Method to normalize a Vector43d
void Vector4<double>::NormalizeSelf()
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
		#ifndef _TORNADO_NO_INTRINSICS_

		// Load vector and length into registers
		__m256d __vec = _mm256_set_pd(w, z, y, x);
		__m256d __len = _mm256_set1_pd(length);

		// Divide
		__m256d __prod = _mm256_div_pd(__vec, __len);

		// Extract and set values
		double prod[4];
		_mm256_storeu_pd(prod, __prod);

		x = prod[0];
		y = prod[1];
		z = prod[2];
		w = prod[3];

		#else

		x /= length;
		y /= length;
		z /= length;
		w /= length;

		#endif
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
void Vector4<int>::NormalizeSelf()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;
	z = 0;
	w = 0;

	return;
}



template<typename T>
bool Vector4<T>::Similar(const Vector4<T>& other, double epsilon) const
{
	return
		(::Math::Similar(x, other.x, epsilon)) &&
		(::Math::Similar(y, other.y, epsilon)) &&
		(::Math::Similar(z, other.z, epsilon)) &&
		(::Math::Similar(w, other.w, epsilon))
	;
}

template<typename T>
Vector4<int> Vector4<T>::ToInt() const
{
	return Vector4<int>((int)x, (int)y, (int)z, (int)w);
}

template<typename T>
Vector4<double> Vector4<T>::ToDouble() const
{
	return Vector4<double>((double)x, (double)y, (double)z, (double)w);
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
	const double it = 1.0 - t; // Inverse t

	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_other = _mm256_set_pd(other.w, other.z, other.y, other.x);
	__m256d __t = _mm256_set1_pd(t);
	__m256d __it = _mm256_set1_pd(it); // Inverse t

	// Procedure:
	// (__vector_self * __it) + (__vector_other * __t)

	__m256d __sum = _mm256_set1_pd(0); // this will hold the sum of the two multiplications

	__sum = _mm256_fmadd_pd(__vector_self, __it, __sum);
	__sum = _mm256_fmadd_pd(__vector_other, __t, __sum);

	// Retrieve result, and apply it
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	x = sum[0];
	y = sum[1];
	z = sum[2];
	w = sum[3];

	#else

	x = it * x + t * other.x;
	y = it * y + t * other.y;
	z = it * z + t * other.z;
	w = it * w + t * other.w;

	#endif

	return;
}



// Slow, lame version for intcels
void Vector4<int>::LerpSelf(const Vector4<int>& other, double t)
{
	const double it = 1.0 - t;

	x = (int)(it * (double)x + t * (double)other.x);
	y = (int)(it * (double)y + t * (double)other.y);
	z = (int)(it * (double)z + t * (double)other.z);
	w = (int)(it * (double)w + t * (double)other.w);

	return;
}

Vector4<double> Vector4<double>::Lerp(const Vector4<double>& other, double t) const
{
	Vector4d copy(*this);
	copy.LerpSelf(other, t);

	return copy;
}

Vector4<double> Vector4<int>::Lerp(const Vector4<int>& other, double t) const
{
	Vector4d copy(this->ToDouble());
	copy.LerpSelf(other.ToDouble(), t);

	return copy;
}



Vector4<double> Vector4<double>::operator+(const Vector4<double>& other) const
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_other = _mm256_set_pd(other.w, other.z, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	return Vector4<double>(
			sum[0],
			sum[1],
			sum[2],
			sum[3]
		);

	#else

	return Vector4<double>(
		x + other.x,
		y + other.y,
		z + other.z,
		w + other.w
	);
	#endif
}

template<typename T>
Vector4<T> Vector4<T>::operator+(const Vector4<T>& other) const
{
	return Vector4<T>(
			x + other.x,
			y + other.y,
			z + other.z,
			w + other.w
		);
}



void Vector4<double>::operator+=(const Vector4<double>& other)
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_other = _mm256_set_pd(other.w, other.z, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	x = sum[0];
	y = sum[1];
	z = sum[2];
	w = sum[3];

	#else

	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;

	#endif

	return;
}

template<typename T>
void Vector4<T>::operator+=(const Vector4<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return;
}



Vector4<double> Vector4<double>::operator-(const Vector4<double>& other) const
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_other = _mm256_set_pd(other.w, other.z, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	return Vector4<double>(
			diff[0],
			diff[1],
			diff[2],
			diff[3]
		);

	#else

	return Vector4<double>(
			x - other.x,
			y - other.y,
			z - other.z,
			w - other.w
		);
	#endif
}

template<typename T>
Vector4<T> Vector4<T>::operator-(const Vector4<T>& other) const
{
	return Vector4<T>(
		x - other.x,
		y - other.y,
		z - other.z,
		w - other.w
	);
}



void Vector4<double>::operator-=(const Vector4<double>& other)
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __vector_other = _mm256_set_pd(other.w, other.z, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	x = diff[0];
	y = diff[1];
	z = diff[2];
	w = diff[3];

	#else

	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;

	#endif

	return;
}

template<typename T>
void Vector4<T>::operator-=(const Vector4<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return;
}



Vector4<double> Vector4<double>::operator*(const double scale) const
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector4<double>(
			prod[0],
			prod[1],
			prod[2],
			prod[3]
		);

	#else

	return Vector4<double>(
			x * scale,
			y * scale,
			z * scale,
			w * scale
		);

	#endif
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



void Vector4<double>::operator*=(const double scale)
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];
	z = prod[2];
	w = prod[3];

	#else

	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;

	#endif

	return;
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



Vector4<double> Vector4<double>::operator/(const double scale) const
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector4<double>(
		prod[0],
		prod[1],
		prod[2],
		prod[3]
	);

	#else

	return Vector4<double>(
			x / scale,
			y / scale,
			z / scale,
			w / scale
		);

	#endif
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



void Vector4<double>::operator/=(const double scale)
{
	#ifndef _TORNADO_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(w, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];
	z = prod[2];
	w = prod[3];

	#else

	x /= scale;
	y /= scale;
	z /= scale;
	w /= scale;

	#endif
	return;
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
Vector4<double> Vector4<double>::operator*(const Matrix4x4& mat) const
{
	Vector4<double> newVec;
	
	newVec.x = (mat[0][0] * x) + (mat[0][1] * y) + (mat[0][2] * z) + (mat[0][3] * w);
	newVec.y = (mat[1][0] * x) + (mat[1][1] * y) + (mat[1][2] * z) + (mat[1][3] * w);
	newVec.z = (mat[2][0] * x) + (mat[2][1] * y) + (mat[2][2] * z) + (mat[2][3] * w);
	newVec.w = (mat[3][0] * x) + (mat[3][1] * y) + (mat[3][2] * z) + (mat[3][3] * w);

	return newVec;
}

// Slow, lame version for intcels
Vector4<int> Vector4<int>::operator*(const Matrix4x4& mat) const
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
void Vector4<double>::operator*=(const Matrix4x4& mat)
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
Vector4<T> Vector4<T>::operator-() const
{
	return Vector4<T>(
		-x,
		-y,
		-z,
		-w
	);
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

template<typename T>
void Vector4<T>::operator=(Vector4<T>&& other) noexcept
{
	x = std::move(other.x);
	y = std::move(other.y);
	z = std::move(other.z);
	w = std::move(other.w);

	return;
}

// Slow, lame version for intcels
void Vector4<int>::operator*=(const Matrix4x4& mat)
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

#include "Vector2.h"
#include "Vector3.h"
template<typename T>
Vector4<T>::operator Vector2<T>() const
{
	return Vector2<T>(x, y);
}

template<typename T>
Vector4<T>::operator Vector3<T>() const
{
	return Vector3<T>(x, y, z);
}

template class Vector4<int>;
template class Vector4<double>;

// Some handy predefines
template <typename T>
const Vector4<double> Vector4<T>::up(0, 1, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::down(0, -1, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::right(1, 0, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::left(-1, 0, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::forward(1, 0, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::backward(-1, 0, 0, 0);
template <typename T>
const Vector4<double> Vector4<T>::future(0, 0, 0, 1);
template <typename T>
const Vector4<double> Vector4<T>::past(0, 0, 0, -1);
template <typename T>
const Vector4<double> Vector4<T>::one(1, 1, 1, 1);
template <typename T>
const Vector4<double> Vector4<T>::zero(0, 0, 0, 0);
