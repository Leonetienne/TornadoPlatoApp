#pragma once

/*** ./../Eule/Matrix4x4.h ***/

#pragma once
#include <cstring>
#include <array>
#include <ostream>

namespace Eule
{
	template <class T>
	class Vector3;
	typedef Vector3<double> Vector3d;

	/** A matrix 4x4 class representing a 3d transformation.
	* This matrix consists of a 3x3 matrix containing scaling and rotation information, and a vector (d,h,l)
	* representing the translation.
	*
	* ```
	* myMatrix[y][x] = 3
	*
	*  X ==============>
	* Y
	* |  # # # # # # # # # # #
	* |  #   a   b   c   d   #
	* |  #                   #
	* |  #   e   f   g   h   #
	* |  #                   #
	* V  #   i   j   k   l   #
	*    #                   #
	*    #   m   n   o   p   #
	*    # # # # # # # # # # #
	*
	* ```
	*
	* Note: This class can also be used to compute regular 4x4 multiplications. Use Multiply4x4() for that.
	*/

	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const Matrix4x4& other);
		Matrix4x4(Matrix4x4&& other) noexcept;

		//! Array holding the matrices values
		std::array<std::array<double, 4>, 4> v;

		Matrix4x4 operator*(const Matrix4x4& other) const;
		void operator*=(const Matrix4x4& other);

		Matrix4x4 operator/(const Matrix4x4& other) const;
		void operator/=(const Matrix4x4& other);

		//! Cellwise scaling
		Matrix4x4 operator*(const double scalar) const;
		//! Cellwise scaling
		void operator*=(const double scalar);

		//! Cellwise division
		Matrix4x4 operator/(const double denominator) const;
		//! Cellwise division
		void operator/=(const double denominator);

		//! Cellwise addition
		Matrix4x4 operator+(const Matrix4x4& other) const;
		//! Cellwise addition
		void operator+=(const Matrix4x4& other);

		//! Cellwise subtraction
		Matrix4x4 operator-(const Matrix4x4& other) const;
		//! Cellwise subtraction
		void operator-=(const Matrix4x4& other);


		std::array<double, 4>& operator[](std::size_t y);
		const std::array<double, 4>& operator[](std::size_t y) const;

		void operator=(const Matrix4x4& other);
		void operator=(Matrix4x4&& other) noexcept;

		bool operator==(const Matrix4x4& other);
		bool operator==(const Matrix4x4& other) const;
		bool operator!=(const Matrix4x4& other);
		bool operator!=(const Matrix4x4& other) const;

		//! Will return d,h,l as a Vector3d(x,y,z)
		const Vector3d GetTranslationComponent() const;
		//! Will set d,h,l from a Vector3d(x,y,z)
		void SetTranslationComponent(const Vector3d& trans);

		//! Will return this Matrix4x4 with d,h,l being set to 0
		Matrix4x4 DropTranslationComponents() const;

		//! Will return the 3x3 transpose of this matrix
		Matrix4x4 Transpose3x3() const;

		//! Will return the 4x4 transpose of this matrix
		Matrix4x4 Transpose4x4() const;

		//! Will return the Matrix4x4 of an actual 4x4 multiplication. operator* only does a 3x3
		Matrix4x4 Multiply4x4(const Matrix4x4& o) const;

		//! Will return the cofactors of this matrix, by dimension n
		Matrix4x4 GetCofactors(std::size_t p, std::size_t q, std::size_t n) const;

		//! Will return the determinant, by dimension n
		double Determinant(std::size_t n) const;

		//! Will return the adjoint of this matrix, by dimension n
		Matrix4x4 Adjoint(std::size_t n) const;

		//! Will return the 3x3-inverse of this matrix.  
		//! Meaning, the 3x3 component will be inverted, and the translation component will be negated
		Matrix4x4 Inverse3x3() const;

		//! Will return the full 4x4-inverse of this matrix
		Matrix4x4 Inverse4x4() const;

		//! Will check if the 3x3-component is inversible
		bool IsInversible3x3() const;

		//! Will check if the entire matrix is inversible
		bool IsInversible4x4() const;

		//! Will compare if two matrices are similar to a certain epsilon value
		bool Similar(const Matrix4x4& other, double epsilon = 0.00001) const;

		friend std::ostream& operator<< (std::ostream& os, const Matrix4x4& m);
		friend std::wostream& operator<< (std::wostream& os, const Matrix4x4& m);

		// Shorthands
		double& a = v[0][0];
		double& b = v[0][1];
		double& c = v[0][2];
		double& d = v[0][3];
		double& e = v[1][0];
		double& f = v[1][1];
		double& g = v[1][2];
		double& h = v[1][3];
		double& i = v[2][0];
		double& j = v[2][1];
		double& k = v[2][2];
		double& l = v[2][3];
		double& m = v[3][0];
		double& n = v[3][1];
		double& o = v[3][2];
		double& p = v[3][3];
	};
}

/*** ./../Eule/Math.h ***/

#pragma once
#include <stdexcept>

namespace Eule
{
	/** Math utility class containing basic functions.
	*/
	class Math
	{
	public:
		//! Will return the bigger of two values
		[[nodiscard]] static constexpr double Max(const double a, const double b);

		//! Will return the smaller of two values
		[[nodiscard]] static constexpr double Min(const double a, const double b);

		//! Will return `v`, but at least `min`, and at most `max`
		[[nodiscard]] static constexpr double Clamp(const double v, const double min, const double max);

		//! Will return the linear interpolation between `a` and `b` by `t`
		[[nodiscard]] static constexpr double Lerp(double a, double b, double t);

		//! Will return the absolute value of `a`
		[[nodiscard]] static constexpr double Abs(const double a);

		//! Compares two double values with a given accuracy
		[[nodiscard]] static constexpr bool Similar(const double a, const double b, const double epsilon = 0.00001);
		
		//! Will compute the actual modulo of a fraction. The % operator returns bs for n<0.
		//! May throw division-by-zero std::logic_error
		[[nodiscard]] static int Mod(const int numerator, const int denominator);

		//! Kind of like \f$sin(counter)\f$, but it oscillates over \f$[a,b]\f$ instead of \f$[-1,1]\f$, by a given speed.  
		//! Given that \f$speed = 1\f$, the result will always be `a` if `counter` is even, and `b` if `counter` is uneven.  
		//! If `counter` is a rational, the result will oscillate between `a` and `b`, like `sin()` does.  
		//! If you increase `speed`, the oscillation frequency will increase. Meaning \f$speed = 2\f$ would result in \f$counter=0.5\f$ returning `b`.
		static double Oscillate(const double a, const double b, const double counter, const double speed);

	private:
		// No instanciation! >:(
		Math();
	};



	/*     These are just the inline methods. They have to lie in the header file.     */
	/*     The more sophisticated methods are in the .cpp					           */

	constexpr inline double Math::Max(double a, double b)
	{
		return (a > b) ? a : b;
	}

	constexpr inline double Math::Min(double a, double b)
	{
		return (a < b) ? a : b;
	}

	constexpr inline double Math::Clamp(double v, double min, double max)
	{
		return Max(Min(v, max), min);
	}

	constexpr inline double Math::Lerp(double a, double b, double t)
	{
		const double it = 1.0 - t;
		return (a * it) + (b * t);
	}

	constexpr inline double Math::Abs(const double a)
	{
		return (a > 0.0) ? a : -a;
	}

	constexpr inline bool Math::Similar(const double a, const double b, const double epsilon)
	{
		return Abs(a - b) <= epsilon;
	}
}

/*** ./../Eule/Vector2.h ***/

#pragma once
#include <cstdlib>
#include <sstream>
#include <iostream>

//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
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

namespace Eule
{
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
		Vector2(const Vector2<T>& other) = default;
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

// Good, optimized chad version for doubles
template<>
double Vector2<double>::DotProduct(const Vector2<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components into registers
	__m256 __vector_self = _mm256_set_ps(0,0,0,0,0,0, (float)y, (float)x);
	__m256 __vector_other = _mm256_set_ps(0,0,0,0,0,0, (float)other.y, (float)other.x);

	// Define bitmask, and execute computation
	const int mask = 0x31; // -> 0011 1000 -> use positions 0011 (last 2) of the vectors supplied, and place them in 1000 (first only) element of __dot
	__m256 __dot = _mm256_dp_ps(__vector_self, __vector_other, mask);

	// Retrieve result, and return it
	float result[8];
	_mm256_storeu_ps(result, __dot);

	return result[0];

	#else
	return (x * other.x) +
		   (y * other.y);
	#endif
}

// Slow, lame version for intcels
template<>
double Vector2<int>::DotProduct(const Vector2<int>& other) const
{
	int iDot = (x * other.x) +
			   (y * other.y);

	return (double)iDot;
}



// Good, optimized chad version for doubles
template<>
double Vector2<double>::CrossProduct(const Vector2<double>& other) const
{
	return (x * other.y) -
		   (y * other.x);
}

// Slow, lame version for intcels
template<>
double Vector2<int>::CrossProduct(const Vector2<int>& other) const
{
	int iCross = (x * other.y) -
				 (y * other.x);

	return (double)iCross;
}



// Good, optimized chad version for doubles
template<>
double Vector2<double>::SqrMagnitude() const
{
	// x.DotProduct(x) == x.SqrMagnitude()
	return DotProduct(*this);
}

// Slow, lame version for intcels
template<>
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


template<>
Vector2<double> Vector2<double>::VectorScale(const Vector2<double>& scalar) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Load vectors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_scalar = _mm256_set_pd(0, 0, scalar.y, scalar.x);

	// Multiply them
	__m256d __product = _mm256_mul_pd(__vector_self, __vector_scalar);

	// Retrieve result
	double result[4];
	_mm256_storeu_pd(result, __product);

	// Return value
	return Vector2<double>(
			result[0],
			result[1]
		);

	#else

	return Vector2<double>(
			x * scalar.x,
			y * scalar.y
		);
	#endif
}

template<>
Vector2<int> Vector2<int>::VectorScale(const Vector2<int>& scalar) const
{
	return Vector2<int>(
			x * scalar.x,
			y * scalar.y
	);
}


template<typename T>
Vector2<double> Vector2<T>::Normalize() const
{
	Vector2<double> norm(x, y);
	norm.NormalizeSelf();

	return norm;
}

// Method to normalize a Vector2d
template<>
void Vector2<double>::NormalizeSelf()
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
		#ifndef _EULE_NO_INTRINSICS_

		// Load vector and length into registers
		__m256d __vec = _mm256_set_pd(0, 0, y, x);
		__m256d __len = _mm256_set1_pd(length);

		// Divide
		__m256d __prod = _mm256_div_pd(__vec, __len);

		// Extract and set values
		double prod[4];
		_mm256_storeu_pd(prod, __prod);

		x = prod[0];
		y = prod[1];

		#else

		x /= length;
		y /= length;

		#endif
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
template<>
void Vector2<int>::NormalizeSelf()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;

	return;
}


// Good, optimized chad version for doubles
template<>
void Vector2<double>::LerpSelf(const Vector2<double>& other, double t)
{
	const double it = 1.0 - t; // Inverse t

	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_other = _mm256_set_pd(0, 0, other.y, other.x);
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
	
	#else

	x = it * x + t * other.x;
	y = it * y + t * other.y;

	#endif
	
	return;
}



// Slow, lame version for intcels
template<>
void Vector2<int>::LerpSelf(const Vector2<int>& other, double t)
{
	const double it = 1.0 - t; // Inverse t

	x = (int)(it * (double)x + t * (double)other.x);
	y = (int)(it * (double)y + t * (double)other.y);

	return;
}

template<>
Vector2<double> Vector2<double>::Lerp(const Vector2<double>& other, double t) const
{
	Vector2d copy(*this);
	copy.LerpSelf(other, t);

	return copy;
}

template<>
Vector2<double> Vector2<int>::Lerp(const Vector2<int>& other, double t) const
{
	Vector2d copy(this->ToDouble());
	copy.LerpSelf(other.ToDouble(), t);

	return copy;
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
		(::Eule::Math::Similar(x, other.x, epsilon)) &&
		(::Eule::Math::Similar(y, other.y, epsilon))
	;
}

template<typename T>
Vector2<int> Vector2<T>::ToInt() const
{
	return Vector2<int>((int)x, (int)y);
}

template<typename T>
Vector2<double> Vector2<T>::ToDouble() const
{
	return Vector2<double>((double)x, (double)y);
}

template<>
Vector2<double> Vector2<double>::operator+(const Vector2<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_other = _mm256_set_pd(0, 0, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	return Vector2<double>(
			sum[0],
			sum[1]
		);

	#else

	return Vector2<double>(
		x + other.x,
		y + other.y
	);
	#endif
}

template<typename T>
Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
{
	return Vector2<T>(
			x + other.x,
			y + other.y
		);
}



template<>
void Vector2<double>::operator+=(const Vector2<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_other = _mm256_set_pd(0, 0, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	x = sum[0];
	y = sum[1];

	#else

	x += other.x;
	y += other.y;

	#endif

	return;
}

template<typename T>
void Vector2<T>::operator+=(const Vector2<T>& other)
{
	x += other.x;
	y += other.y;
	return;
}



template<>
Vector2<double> Vector2<double>::operator-(const Vector2<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_other = _mm256_set_pd(0, 0, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	return Vector2<double>(
			diff[0],
			diff[1]
		);

	#else

	return Vector2<double>(
			x - other.x,
			y - other.y
		);
	#endif
}

template<typename T>
Vector2<T> Vector2<T>::operator-(const Vector2<T>& other) const
{
	return Vector2<T>(
		x - other.x,
		y - other.y
	);
}



template<>
void Vector2<double>::operator-=(const Vector2<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __vector_other = _mm256_set_pd(0, 0, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	x = diff[0];
	y = diff[1];

	#else

	x -= other.x;
	y -= other.y;

	#endif

	return;
}

template<typename T>
void Vector2<T>::operator-=(const Vector2<T>& other)
{
	x -= other.x;
	y -= other.y;
	return;
}



template<>
Vector2<double> Vector2<double>::operator*(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector2<double>(
			prod[0],
			prod[1]
		);

	#else

	return Vector2<double>(
			x * scale,
			y * scale
		);

	#endif
}

template<typename T>
Vector2<T> Vector2<T>::operator*(const T scale) const
{
	return Vector2<T>(
		x * scale,
		y * scale
	);
}



template<>
void Vector2<double>::operator*=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];

	#else

	x *= scale;
	y *= scale;

	#endif

	return;
}

template<typename T>
void Vector2<T>::operator*=(const T scale)
{
	x *= scale;
	y *= scale;
	return;
}



template<>
Vector2<double> Vector2<double>::operator/(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector2<double>(
		prod[0],
		prod[1]
	);

	#else

	return Vector2<double>(
			x / scale,
			y / scale
		);

	#endif
}

template<typename T>
Vector2<T> Vector2<T>::operator/(const T scale) const
{
	return Vector2<T>(
			x / scale,
			y / scale
		);
}



template<>
void Vector2<double>::operator/=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, 0, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];

	#else

	x /= scale;
	y /= scale;

	#endif
	return;
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
void Vector2<T>::operator=(Vector2<T>&& other) noexcept
{
	x = std::move(other.x);
	y = std::move(other.y);

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

template<typename T>
Vector2<T> Vector2<T>::operator-() const
{
	return Vector2<T>(
		-x,
		-y
	);
}

template class Vector2<int>;
template class Vector2<double>;

// Some handy predefines
template <typename T>
const Vector2<double> Vector2<T>::up(0, 1);
template <typename T>
const Vector2<double> Vector2<T>::down(0, -1);
template <typename T>
const Vector2<double> Vector2<T>::right(1, 0);
template <typename T>
const Vector2<double> Vector2<T>::left(-1, 0);
template <typename T>
const Vector2<double> Vector2<T>::one(1, 1);
template <typename T>
const Vector2<double> Vector2<T>::zero(0, 0);
}

/*** ./../Eule/Vector3.h ***/

#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>

#include <iostream>

//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
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

namespace Eule
{
	template <typename T> class Vector2;
	template <typename T> class Vector4;

	/** Representation of a 3d vector.
	* Contains a lot of utility methods.
	*/
	template <typename T>
	class Vector3
	{
	public:
		Vector3() : x{ 0 }, y{ 0 }, z{ 0 } {}
		Vector3(T _x, T _y, T _z) : x{ _x }, y{ _y }, z{ _z } {}
		Vector3(const Vector3<T>& other) = default;
		Vector3(Vector3<T>&& other) noexcept = default;

		//! Will compute the dot product to another Vector3
		double DotProduct(const Vector3<T>& other) const;

		//! Will compute the cross product to another Vector3
		Vector3<double> CrossProduct(const Vector3<T>& other) const;

		//! Will compute the square magnitude
		double SqrMagnitude() const;

		//! Will compute the magnitude
		double Magnitude() const;

		//! Will return the normalization of this vector
		[[nodiscard]] Vector3<double> Normalize() const;

		//! Will normalize this vector
		void NormalizeSelf();

		//! Will scale self.n by scalar.n
		[[nodiscard]] Vector3<T> VectorScale(const Vector3<T>& scalar) const;

		//! Will lerp itself towards other by t
		void LerpSelf(const Vector3<T>& other, double t);

		//! Will return a lerp result between this and another vector
		[[nodiscard]] Vector3<double> Lerp(const Vector3<T>& other, double t) const;

		//! Will compare if two vectors are similar to a certain epsilon value
		[[nodiscard]] bool Similar(const Vector3<T>& other, double epsilon = 0.00001) const;

		//! Will convert this vector to a Vector3i
		[[nodiscard]] Vector3<int> ToInt() const;

		//! Will convert this vector to a Vector3d
		[[nodiscard]] Vector3<double> ToDouble() const;

		T& operator[](std::size_t idx);
		const T& operator[](std::size_t idx) const;

		Vector3<T> operator+(const Vector3<T>& other) const;
		void operator+=(const Vector3<T>& other);
		Vector3<T> operator-(const Vector3<T>& other) const;
		void operator-=(const Vector3<T>& other);
		Vector3<T> operator*(const T scale) const;
		void operator*=(const T scale);
		Vector3<T> operator/(const T scale) const;
		void operator/=(const T scale);
		Vector3<T> operator*(const Matrix4x4& mat) const;
		void operator*=(const Matrix4x4& mat);
		Vector3<T> operator-() const;

		operator Vector2<T>() const; //! Conversion method
		operator Vector4<T>() const; //! Conversion method

		void operator=(const Vector3<T>& other);
		void operator=(Vector3<T>&& other) noexcept;

		bool operator==(const Vector3<T>& other) const;
		bool operator!=(const Vector3<T>& other) const;

		friend std::ostream& operator << (std::ostream& os, const Vector3<T>& v)
		{
			return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "]";
		}
		friend std::wostream& operator << (std::wostream& os, const Vector3<T>& v)
		{
			return os << L"[x: " << v.x << L"  y: " << v.y << L"  z: " << v.z << L"]";
		}

		T x;
		T y;
		T z;

		// Some handy predefines
		static const Vector3<double> up;
		static const Vector3<double> down;
		static const Vector3<double> right;
		static const Vector3<double> left;
		static const Vector3<double> forward;
		static const Vector3<double> backward;
		static const Vector3<double> one;
		static const Vector3<double> zero;
	};

	typedef Vector3<int> Vector3i;
	typedef Vector3<double> Vector3d;

	// Good, optimized chad version for doubles
template<>
double Vector3<double>::DotProduct(const Vector3<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components into registers
	__m256 __vector_self  = _mm256_set_ps(0,0,0,0,0, (float)z, (float)y, (float)x);
	__m256 __vector_other = _mm256_set_ps(0,0,0,0,0, (float)other.z, (float)other.y, (float)other.x);

	// Define bitmask, and execute computation
	const int mask = 0x71; // -> 0111 1000 -> use positions 0111 (last 3) of the vectors supplied, and place them in 1000 (first only) element of __dot
	__m256 __dot = _mm256_dp_ps(__vector_self, __vector_other, mask);

	// Retrieve result, and return it
	float result[8];
	_mm256_storeu_ps(result, __dot);

	return result[0];

	#else
	return (x * other.x) +
		   (y * other.y) +
		   (z * other.z);
	#endif
}

// Slow, lame version for intcels
template<>
double Vector3<int>::DotProduct(const Vector3<int>& other) const
{
	int iDot = (x * other.x) + (y * other.y) + (z * other.z);
	return (double)iDot;
}



// Good, optimized chad version for doubles
template<>
Vector3<double> Vector3<double>::CrossProduct(const Vector3<double>& other) const
{
	Vector3<double> cp;
	cp.x = (y * other.z) - (z * other.y);
	cp.y = (z * other.x) - (x * other.z);
	cp.z = (x * other.y) - (y * other.x);

	return cp;
}

// Slow, lame version for intcels
template<>
Vector3<double> Vector3<int>::CrossProduct(const Vector3<int>& other) const
{
	Vector3<double> cp;
	cp.x = ((double)y * (double)other.z) - ((double)z * (double)other.y);
	cp.y = ((double)z * (double)other.x) - ((double)x * (double)other.z);
	cp.z = ((double)x * (double)other.y) - ((double)y * (double)other.x);

	return cp;
}



// Good, optimized chad version for doubles
template<>
double Vector3<double>::SqrMagnitude() const
{
	// x.DotProduct(x) == x.SqrMagnitude()
	return DotProduct(*this);
}

// Slow, lame version for intcels
template<>
double Vector3<int>::SqrMagnitude() const
{
	int iSqrMag = x*x + y*y + z*z;
	return (double)iSqrMag;
}

template <typename T>
double Vector3<T>::Magnitude() const
{
	return sqrt(SqrMagnitude());
}



template<>
Vector3<double> Vector3<double>::VectorScale(const Vector3<double>& scalar) const
{
	#ifndef _EULE_NO_INTRINSICS_
	
	// Load vectors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_scalar = _mm256_set_pd(0, scalar.z, scalar.y, scalar.x);

	// Multiply them
	__m256d __product = _mm256_mul_pd(__vector_self, __vector_scalar);

	// Retrieve result
	double result[4];
	_mm256_storeu_pd(result, __product);

	// Return value
	return Vector3<double>(
			result[0],
			result[1],
			result[2]
		);

	#else

	return Vector3<double>(
			x * scalar.x,
			y * scalar.y,
			z * scalar.z
		);

	#endif
}

template<>
Vector3<int> Vector3<int>::VectorScale(const Vector3<int>& scalar) const
{
	return Vector3<int>(
			x * scalar.x,
			y * scalar.y,
			z * scalar.z
	);
}



template<typename T>
Vector3<double> Vector3<T>::Normalize() const
{
	Vector3<double> norm(x, y, z);
	norm.NormalizeSelf();

	return norm;
}

// Method to normalize a Vector3d
template<>
void Vector3<double>::NormalizeSelf()
{
	const double length = Magnitude();

	// Prevent division by 0
	if (length == 0)
	{
		x = 0;
		y = 0;
		z = 0;
	}
	else
	{
		#ifndef _EULE_NO_INTRINSICS_

		// Load vector and length into registers
		__m256d __vec = _mm256_set_pd(0, z, y, x);
		__m256d __len = _mm256_set1_pd(length);

		// Divide
		__m256d __prod = _mm256_div_pd(__vec, __len);

		// Extract and set values
		double prod[4];
		_mm256_storeu_pd(prod, __prod);

		x = prod[0];
		y = prod[1];
		z = prod[2];
		
		#else
		
		x /= length;
		y /= length;
		z /= length;
		
		#endif
	}

	return;
}

// You can't normalize an int vector, ffs!
// But we need an implementation for T=int
template<>
void Vector3<int>::NormalizeSelf()
{
	std::cerr << "Stop normalizing int-vectors!!" << std::endl;
	x = 0;
	y = 0;
	z = 0;

	return;
}



template<typename T>
bool Vector3<T>::Similar(const Vector3<T>& other, double epsilon) const
{
	return
		(::Eule::Math::Similar(x, other.x, epsilon)) &&
		(::Eule::Math::Similar(y, other.y, epsilon)) &&
		(::Eule::Math::Similar(z, other.z, epsilon))
	;
}

template<typename T>
Vector3<int> Vector3<T>::ToInt() const
{
	return Vector3<int>((int)x, (int)y, (int)z);
}

template<typename T>
Vector3<double> Vector3<T>::ToDouble() const
{
	return Vector3<double>((double)x, (double)y, (double)z);
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



// Good, optimized chad version for doubles
template<>
void Vector3<double>::LerpSelf(const Vector3<double>& other, double t)
{
	const double it = 1.0 - t; // Inverse t

	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_other = _mm256_set_pd(0, other.z, other.y, other.x);
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

	#else

	x = it*x + t*other.x;
	y = it*y + t*other.y;
	z = it*z + t*other.z;

	#endif

	return;
}



// Slow, lame version for intcels
template<>
void Vector3<int>::LerpSelf(const Vector3<int>& other, double t)
{
	const double it = 1.0 - t; // Inverse t

	x = (int)(it * (double)x + t * (double)other.x);
	y = (int)(it * (double)y + t * (double)other.y);
	z = (int)(it * (double)z + t * (double)other.z);

	return;
}

template<>
Vector3<double> Vector3<double>::Lerp(const Vector3<double>& other, double t) const
{
	Vector3d copy(*this);
	copy.LerpSelf(other, t);

	return copy;
}

template<>
Vector3<double> Vector3<int>::Lerp(const Vector3<int>& other, double t) const
{
	Vector3d copy(this->ToDouble());
	copy.LerpSelf(other.ToDouble(), t);

	return copy;
}



template<>
Vector3<double> Vector3<double>::operator+(const Vector3<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_other = _mm256_set_pd(0, other.z, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	return Vector3<double>(
			sum[0],
			sum[1],
			sum[2]
		);

	#else

	return Vector3<double>(
		x + other.x,
		y + other.y,
		z + other.z
	);
	#endif
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



template<>
void Vector3<double>::operator+=(const Vector3<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_other = _mm256_set_pd(0, other.z, other.y, other.x);

	// Add the components
	__m256d __sum = _mm256_add_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	x = sum[0];
	y = sum[1];
	z = sum[2];

	#else

	x += other.x;
	y += other.y;
	z += other.z;

	#endif

	return;
}

template<typename T>
void Vector3<T>::operator+=(const Vector3<T>& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return;
}



template<>
Vector3<double> Vector3<double>::operator-(const Vector3<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_other = _mm256_set_pd(0, other.z, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and return these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	return Vector3<double>(
			diff[0],
			diff[1],
			diff[2]
		);

	#else

	return Vector3<double>(
			x - other.x,
			y - other.y,
			z - other.z
		);
	#endif
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



template<>
void Vector3<double>::operator-=(const Vector3<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __vector_other = _mm256_set_pd(0, other.z, other.y, other.x);

	// Subtract the components
	__m256d __diff = _mm256_sub_pd(__vector_self, __vector_other);

	// Retrieve and apply these values
	double diff[4];
	_mm256_storeu_pd(diff, __diff);

	x = diff[0];
	y = diff[1];
	z = diff[2];

	#else

	x -= other.x;
	y -= other.y;
	z -= other.z;

	#endif

	return;
}

template<typename T>
void Vector3<T>::operator-=(const Vector3<T>& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return;
}



template<>
Vector3<double> Vector3<double>::operator*(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector3<double>(
			prod[0],
			prod[1],
			prod[2]
		);

	#else

	return Vector3<double>(
			x * scale,
			y * scale,
			z * scale
		);

	#endif
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



template<>
void Vector3<double>::operator*=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Multiply the components
	__m256d __prod = _mm256_mul_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];
	z = prod[2];

	#else

	x *= scale;
	y *= scale;
	z *= scale;

	#endif

	return;
}

template<typename T>
void Vector3<T>::operator*=(const T scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return;
}



template<>
Vector3<double> Vector3<double>::operator/(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and return these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	return Vector3<double>(
		prod[0],
		prod[1],
		prod[2]
	);

	#else

	return Vector3<double>(
			x / scale,
			y / scale,
			z / scale
		);

	#endif
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



template<>
void Vector3<double>::operator/=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

	// Move vector components and factors into registers
	__m256d __vector_self = _mm256_set_pd(0, z, y, x);
	__m256d __scalar = _mm256_set1_pd(scale);

	// Divide the components
	__m256d __prod = _mm256_div_pd(__vector_self, __scalar);

	// Retrieve and apply these values
	double prod[4];
	_mm256_storeu_pd(prod, __prod);

	x = prod[0];
	y = prod[1];
	z = prod[2];

	#else

	x /= scale;
	y /= scale;
	z /= scale;

	#endif
	return;
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
template<>
Vector3<double> Vector3<double>::operator*(const Matrix4x4& mat) const
{
	Vector3<double> newVec;

	#ifndef _EULE_NO_INTRINSICS_
	// Store x, y, and z values
	__m256d __vecx = _mm256_set1_pd(x);
	__m256d __vecy = _mm256_set1_pd(y);
	__m256d __vecz = _mm256_set1_pd(z);

	// Store matrix values
	__m256d __mat_row0 = _mm256_set_pd(mat[0][0], mat[1][0], mat[2][0], 0);
	__m256d __mat_row1 = _mm256_set_pd(mat[0][1], mat[1][1], mat[2][1], 0);
	__m256d __mat_row2 = _mm256_set_pd(mat[0][2], mat[1][2], mat[2][2], 0);

	// Multiply x, y, z and matrix values
	__m256d __mul_vecx_row0 = _mm256_mul_pd(__vecx, __mat_row0);
	__m256d __mul_vecy_row1 = _mm256_mul_pd(__vecy, __mat_row1);
	__m256d __mul_vecz_row2 = _mm256_mul_pd(__vecz, __mat_row2);

	// Sum up the products
	__m256d __sum = _mm256_add_pd(__mul_vecx_row0, _mm256_add_pd(__mul_vecy_row1, __mul_vecz_row2));

	// Store translation values
	__m256d __translation = _mm256_set_pd(mat[0][3], mat[1][3], mat[2][3], 0);

	// Add the translation values
	__m256d __final = _mm256_add_pd(__sum, __translation);

	double dfinal[4];

	_mm256_storeu_pd(dfinal, __final);

	newVec.x = dfinal[3];
	newVec.y = dfinal[2];
	newVec.z = dfinal[1];

	#else
	// Rotation, Scaling
	newVec.x = (mat[0][0] * x) + (mat[0][1] * y) + (mat[0][2] * z);
	newVec.y = (mat[1][0] * x) + (mat[1][1] * y) + (mat[1][2] * z);
	newVec.z = (mat[2][0] * x) + (mat[2][1] * y) + (mat[2][2] * z);

	// Translation
	newVec.x += mat[0][3];
	newVec.y += mat[1][3];
	newVec.z += mat[2][3];
	#endif

	return newVec;
}

// Slow, lame version for intcels
template<>
Vector3<int> Vector3<int>::operator*(const Matrix4x4& mat) const
{
	Vector3<double> newVec;

	// Rotation, Scaling
	newVec.x = (mat[0][0] * x) + (mat[0][1] * y) + (mat[0][2] * z);
	newVec.y = (mat[1][0] * x) + (mat[1][1] * y) + (mat[1][2] * z);
	newVec.z = (mat[2][0] * x) + (mat[2][1] * y) + (mat[2][2] * z);

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
template<>
void Vector3<double>::operator*=(const Matrix4x4& mat)
{
	#ifndef _EULE_NO_INTRINSICS_
	// Store x, y, and z values
	__m256d __vecx = _mm256_set1_pd(x);
	__m256d __vecy = _mm256_set1_pd(y);
	__m256d __vecz = _mm256_set1_pd(z);

	// Store matrix values
	__m256d __mat_row0 = _mm256_set_pd(mat[0][0], mat[1][0], mat[2][0], 0);
	__m256d __mat_row1 = _mm256_set_pd(mat[0][1], mat[1][1], mat[2][1], 0);
	__m256d __mat_row2 = _mm256_set_pd(mat[0][2], mat[1][2], mat[2][2], 0);

	// Multiply x, y, z and matrix values
	__m256d __mul_vecx_row0 = _mm256_mul_pd(__vecx, __mat_row0);
	__m256d __mul_vecy_row1 = _mm256_mul_pd(__vecy, __mat_row1);
	__m256d __mul_vecz_row2 = _mm256_mul_pd(__vecz, __mat_row2);

	// Sum up the products
	__m256d __sum = _mm256_add_pd(__mul_vecx_row0, _mm256_add_pd(__mul_vecy_row1, __mul_vecz_row2));

	// Store translation values
	__m256d __translation = _mm256_set_pd(mat[0][3], mat[1][3], mat[2][3], 0);

	// Add the translation values
	__m256d __final = _mm256_add_pd(__sum, __translation);

	double dfinal[4];

	_mm256_storeu_pd(dfinal, __final);

	x = dfinal[3];
	y = dfinal[2];
	z = dfinal[1];

	#else
	Vector3<double> buffer = *this;
	x = (mat[0][0] * buffer.x) + (mat[0][1] * buffer.y) + (mat[0][2] * buffer.z);
	y = (mat[1][0] * buffer.x) + (mat[1][1] * buffer.y) + (mat[1][2] * buffer.z);
	z = (mat[2][0] * buffer.x) + (mat[2][1] * buffer.y) + (mat[2][2] * buffer.z);
	
	// Translation
	x += mat[0][3];
	y += mat[1][3];
	z += mat[2][3];
	#endif

	return;
}

template<typename T>
Vector3<T> Vector3<T>::operator-() const
{
	return Vector3<T>(
		-x,
		-y,
		-z
	);
}

template<typename T>
void Vector3<T>::operator=(const Vector3<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;

	return;
}

template<typename T>
void Vector3<T>::operator=(Vector3<T>&& other) noexcept
{
	x = std::move(other.x);
	y = std::move(other.y);
	z = std::move(other.z);

	return;
}

// Slow, lame version for intcels
template<>
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

// Some handy predefines
template <typename T>
const Vector3<double> Vector3<T>::up(0, 1, 0);
template <typename T>
const Vector3<double> Vector3<T>::down(0, -1, 0);
template <typename T>
const Vector3<double> Vector3<T>::right(1, 0, 0);
template <typename T>
const Vector3<double> Vector3<T>::left(-1, 0, 0);
template <typename T>
const Vector3<double> Vector3<T>::forward(0, 0, 1);
template <typename T>
const Vector3<double> Vector3<T>::backward(0, 0, -1);
template <typename T>
const Vector3<double> Vector3<T>::one(1, 1, 1);
template <typename T>
const Vector3<double> Vector3<T>::zero(0, 0, 0);

}

/*** ./../Eule/Collider.h ***/

#pragma once

namespace Eule
{
	/** Abstract class of a collider domain.
	* Specializations describe a shape in 3d space, and provide implementations of the methods below,
	* for their specific shape. Examples could be a SphereCollider, a BoxCollider, etc...
	*/
	class Collider
	{
	public:
		//! Tests, if this Collider contains a point
		virtual bool Contains(const Vector3d& point) const = 0;
	};
}

/*** ./../Eule/TrapazoidalPrismCollider.h ***/

#pragma once
#include <array>

namespace Eule
{
	/** A collider describing a trapazoidal prism.
	* A trapazoidal prism is basically a box, but each vertex can be manipulated individually, altering
	* the angles between faces.
	* Distorting a 2d face into 3d space will result in undefined behaviour. Each face should stay flat, relative to itself. This shape is based on QUADS!
	*/
	class TrapazoidalPrismCollider : public Collider
	{
	public:
		TrapazoidalPrismCollider();
		TrapazoidalPrismCollider(const TrapazoidalPrismCollider& other) = default;
		TrapazoidalPrismCollider(TrapazoidalPrismCollider&& other) noexcept = default;
		void operator=(const TrapazoidalPrismCollider& other);
		void operator=(TrapazoidalPrismCollider&& other) noexcept;

		//! Will return a specific vertex
		const Vector3d& GetVertex(std::size_t index) const;

		//! Will set the value of a specific vertex
		void SetVertex(std::size_t index, const Vector3d value);

		//! Tests, if this Collider contains a point
		bool Contains(const Vector3d& point) const override;

		/* Vertex identifiers */
		static constexpr std::size_t BACK = 0;
		static constexpr std::size_t FRONT = 4;
		static constexpr std::size_t LEFT = 0;
		static constexpr std::size_t RIGHT = 2;
		static constexpr std::size_t BOTTOM = 0;
		static constexpr std::size_t TOP = 1;

	private:
		enum class FACE_NORMALS : std::size_t;

		//! Will calculate the vertex normals from vertices
		void GenerateNormalsFromVertices();

		//! Returns the dot product of a given point against a specific plane of the bounding box
		double FaceDot(FACE_NORMALS face, const Vector3d& point) const;

		std::array<Vector3d, 8> vertices;


		// Face normals
		enum class FACE_NORMALS : std::size_t
		{
			LEFT = 0,
			RIGHT = 1,
			FRONT = 2,
			BACK = 3,
			TOP = 4,
			BOTTOM = 5
		};
		std::array<Vector3d, 6> faceNormals;
	};
}

/*** ./../Eule/Constants.h ***/

#pragma once

// Pretty sure the compiler will optimize these calculations out...

//! Pi up to 50 decimal places
static constexpr double PI = 3.14159265358979323846264338327950288419716939937510;

//! Pi divided by two
static constexpr double HALF_PI = 1.57079632679489661923132169163975144209858469968755;

//! Factor to convert degrees to radians 
static constexpr double Deg2Rad = 0.0174532925199432957692369076848861271344287188854172222222222222;

//! Factor to convert radians to degrees
static constexpr double Rad2Deg = 57.295779513082320876798154814105170332405472466564427711013084788;

/*** ./../Eule/Vector4.h ***/

#pragma once
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>

#include <iostream>

//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
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
namespace Eule
{
	template <typename T> class Vector2;
	template <typename T> class Vector3;

	/** Representation of a 4d vector.
	* Contains a lot of utility methods.
	*/
	template <typename T>
	class Vector4
	{
	public:
		Vector4() : x{ 0 }, y{ 0 }, z{ 0 }, w{ 0 } {}
		Vector4(T _x, T _y, T _z, T _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w } {}
		Vector4(const Vector4<T>& other) = default;
		Vector4(Vector4<T>&& other) noexcept = default;

		//! Will compute the square magnitude
		double SqrMagnitude() const;

		//! Will compute the magnitude
		double Magnitude() const;

		//! Will return the normalization of this vector
		[[nodiscard]] Vector4<double> Normalize() const;

		//! Will normalize this vector
		void NormalizeSelf();

		//! Will scale self.n by scalar.n
		[[nodiscard]] Vector4<T> VectorScale(const Vector4<T>& scalar) const;

		//! Will lerp itself towards other by t
		void LerpSelf(const Vector4<T>& other, double t);

		//! Will return a lerp result between this and another vector
		[[nodiscard]] Vector4<double> Lerp(const Vector4<T>& other, double t) const;

		//! Will compare if two vectors are similar to a certain epsilon value
		[[nodiscard]] bool Similar(const Vector4<T>& other, double epsilon = 0.00001) const;

		//! Will convert this vector to a Vector4i
		[[nodiscard]] Vector4<int> ToInt() const;

		//! Will convert this vector to a Vector4d
		[[nodiscard]] Vector4<double> ToDouble() const;

		T& operator[](std::size_t idx);
		const T& operator[](std::size_t idx) const;

		Vector4<T> operator+(const Vector4<T>& other) const;
		void operator+=(const Vector4<T>& other);
		Vector4<T> operator-(const Vector4<T>& other) const;
		void operator-=(const Vector4<T>& other);
		Vector4<T> operator*(const T scale) const;
		void operator*=(const T scale);
		Vector4<T> operator/(const T scale) const;
		void operator/=(const T scale);
		Vector4<T> operator*(const Matrix4x4& mat) const;
		void operator*=(const Matrix4x4& mat);
		Vector4<T> operator-() const;

		operator Vector2<T>() const; //! Conversion method
		operator Vector3<T>() const; //! Conversion method

		void operator=(const Vector4<T>& other);
		void operator=(Vector4<T>&& other) noexcept;

		bool operator==(const Vector4<T>& other) const;
		bool operator!=(const Vector4<T>& other) const;

		friend std::ostream& operator << (std::ostream& os, const Vector4<T>& v)
		{
			return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "  w: " << v.w << "]";
		}
		friend std::wostream& operator << (std::wostream& os, const Vector4<T>& v)
		{
			return os << L"[x: " << v.x << L"  y: " << v.y << L"  z: " << v.z << L"  w: " << v.w << L"]";
		}

		T x;
		T y;
		T z;
		T w;

		// Some handy predefines
		static const Vector4<double> up;
		static const Vector4<double> down;
		static const Vector4<double> right;
		static const Vector4<double> left;
		static const Vector4<double> forward;
		static const Vector4<double> backward;
		static const Vector4<double> future;
		static const Vector4<double> past;
		static const Vector4<double> one;
		static const Vector4<double> zero;
	};

	typedef Vector4<int> Vector4i;
	typedef Vector4<double> Vector4d;

	// Good, optimized chad version for doubles
template<>
double Vector4<double>::SqrMagnitude() const
{
	return (x * x) +
		   (y * y) +
		   (z * z) +
		   (w * w);
}

// Slow, lame version for intcels
template<>
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


template<>
Vector4<double> Vector4<double>::VectorScale(const Vector4<double>& scalar) const
{
	#ifndef _EULE_NO_INTRINSICS_

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


template<>
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

// Method to normalize a Vector4d
template<>
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
		#ifndef _EULE_NO_INTRINSICS_

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
template<>
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
		(::Eule::Math::Similar(x, other.x, epsilon)) &&
		(::Eule::Math::Similar(y, other.y, epsilon)) &&
		(::Eule::Math::Similar(z, other.z, epsilon)) &&
		(::Eule::Math::Similar(w, other.w, epsilon))
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
template<>
void Vector4<double>::LerpSelf(const Vector4<double>& other, double t)
{
	const double it = 1.0 - t; // Inverse t

	#ifndef _EULE_NO_INTRINSICS_

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
template<>
void Vector4<int>::LerpSelf(const Vector4<int>& other, double t)
{
	const double it = 1.0 - t;

	x = (int)(it * (double)x + t * (double)other.x);
	y = (int)(it * (double)y + t * (double)other.y);
	z = (int)(it * (double)z + t * (double)other.z);
	w = (int)(it * (double)w + t * (double)other.w);

	return;
}

template<>
Vector4<double> Vector4<double>::Lerp(const Vector4<double>& other, double t) const
{
	Vector4d copy(*this);
	copy.LerpSelf(other, t);

	return copy;
}

template<>
Vector4<double> Vector4<int>::Lerp(const Vector4<int>& other, double t) const
{
	Vector4d copy(this->ToDouble());
	copy.LerpSelf(other.ToDouble(), t);

	return copy;
}



template<>
Vector4<double> Vector4<double>::operator+(const Vector4<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
void Vector4<double>::operator+=(const Vector4<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
Vector4<double> Vector4<double>::operator-(const Vector4<double>& other) const
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
void Vector4<double>::operator-=(const Vector4<double>& other)
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
Vector4<double> Vector4<double>::operator*(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
void Vector4<double>::operator*=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
Vector4<double> Vector4<double>::operator/(const double scale) const
{
	#ifndef _EULE_NO_INTRINSICS_

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



template<>
void Vector4<double>::operator/=(const double scale)
{
	#ifndef _EULE_NO_INTRINSICS_

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
template<>
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
template<>
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
template<>
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
template<>
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

}

/*** ./../Eule/Quaternion.h ***/

#pragma once
#include <mutex>

namespace Eule
{
    /** 3D rotation representation
    */
    class Quaternion
    {
    public:
        Quaternion();

        //! Constructs by these raw values
        explicit Quaternion(const Vector4d values);

        //! Copies this existing Quaternion
        Quaternion(const Quaternion& q);

        //! Creates an quaternion from euler angles
        Quaternion(const Vector3d eulerAngles);

        ~Quaternion();

        //! Copies
        Quaternion operator= (const Quaternion& q);

        //! Multiplies (applies)
        Quaternion operator* (const Quaternion& q) const;

        //! Divides (applies)
        Quaternion operator/ (Quaternion& q) const;

        //! Also multiplies
        Quaternion& operator*= (const Quaternion& q);

        //! Also divides
        Quaternion& operator/= (const Quaternion& q);

        //! Will transform a 3d point around its origin
        Vector3d operator* (const Vector3d& p) const;

        bool operator== (const Quaternion& q) const;
        bool operator!= (const Quaternion& q) const;

        Quaternion Inverse() const;

        Quaternion Conjugate() const;

        Quaternion UnitQuaternion() const;

        //! Will rotate a vector by this quaternion
        Vector3d RotateVector(const Vector3d& vec) const;

        //! Will return euler angles representing this Quaternion's rotation
        Vector3d ToEulerAngles() const;

        //! Will return a rotation matrix representing this Quaternions rotation
        Matrix4x4 ToRotationMatrix() const;

        //! Will return the raw four-dimensional values
        Vector4d GetRawValues() const;

        //! Will return the value between two Quaternion's as another Quaternion
        Quaternion AngleBetween(const Quaternion& other) const;

        //! Will set the raw four-dimensional values
        void SetRawValues(const Vector4d values);

        //! Will return the lerp result between two quaternions
        Quaternion Lerp(const Quaternion& other, double t) const;

        friend std::ostream& operator<< (std::ostream& os, const Quaternion& q);
        friend std::wostream& operator<< (std::wostream& os, const Quaternion& q);

    private:
        //! Scales
        Quaternion operator* (const double scale) const;
        Quaternion& operator*= (const double scale);

        //! Quaternion values
        Vector4d v;

        //! Will force a regenartion of the euler and matrix caches on further converter calls
        void InvalidateCache();

        // Caches for conversions
        mutable bool isCacheUpToDate_euler = false;
        mutable Vector3d cache_euler;

        mutable bool isCacheUpToDate_matrix = false;
        mutable Matrix4x4 cache_matrix;

        mutable bool isCacheUpToDate_inverse = false;
        mutable Vector4d cache_inverse;

        // Mutexes for thread-safe caching
        mutable std::mutex lock_eulerCache;
        mutable std::mutex lock_matrixCache;
        mutable std::mutex lock_inverseCache;
    };
}

/*** ./../Eule/gcccompat.h ***/

#pragma once

/*
* Some intrinsic functions such as _mm_sincos_pd are not available on g++ by default (requires some specific library).
* So let's just "re"define them manually if we're on g++.
* This way the code still works, even with the other intrinsics enabled.
*/

#if (__GNUC__ && __cplusplus)
#include <immintrin.h>
#include <math.h>

inline __m256d _mm256_sincos_pd(__m256d* __cos, __m256d __vec)
{
	double vec[4];

	_mm256_storeu_pd(vec, __vec);

	// Manually calculate cosines
	*__cos = _mm256_set_pd(
		cos(vec[3]),
		cos(vec[2]),
		cos(vec[1]),
		cos(vec[0])
	);

	// Manually calculate sines
	return _mm256_set_pd(
		sin(vec[3]),
		sin(vec[2]),
		sin(vec[1]),
		sin(vec[0])
	);
}
#endif

/*** ./../Eule/Random.h ***/

#pragma once
#include <random>

namespace Eule
{
	/** Extensive random number generator
	*/
	class Random
	{
	public:
		//! Will return a random double between `0` and `1`
		static double RandomFloat();

		//! Will return a random unsigned integer.
		static unsigned int RandomUint();

		//! Will return a random integer
		static unsigned int RandomInt();

		//! Will return a random double within a range  
		//! These bounds are INCLUSIVE!
		static double RandomRange(const double min, const double max);

		//! Will return a random integer within a range. This is faster than `(int)RandomRange(x,y)`  
		//! These bounds are INCLUSIVE!
		static int RandomIntRange(const int max, const int min);

		//! Will 'roll' a dice, returning `true` \f$100 * chance\f$ percent of the time.
		static bool RandomChance(const double chance);

	private:
		//! Will initialize the random number generator
		static void InitRng();

		static std::mt19937 rng;
		static bool isRngInitialized;

		// No instanciation! >:(
		Random();
	};
}
