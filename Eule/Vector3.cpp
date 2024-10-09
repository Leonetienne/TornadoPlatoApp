#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

#include "Math.h"

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

namespace Eule {
    template<typename T>
    Vector3<T>::operator Vector2<T>() const
    {
        return Vector2<T>(x, y);
    }

    template<typename T>
    Vector3<T>::operator Vector4<T>() const
    {
        return Vector4<T>(x, y, z, 0);
    }

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
    template <>
    const Vector3<double> Vector3<double>::up(0, 1, 0);
    template <>
    const Vector3<double> Vector3<double>::down(0, -1, 0);
    template <>
    const Vector3<double> Vector3<double>::right(1, 0, 0);
    template <>
    const Vector3<double> Vector3<double>::left(-1, 0, 0);
    template <>
    const Vector3<double> Vector3<double>::forward(0, 0, 1);
    template <>
    const Vector3<double> Vector3<double>::backward(0, 0, -1);
    template <>
    const Vector3<double> Vector3<double>::one(1, 1, 1);
    template <>
    const Vector3<double> Vector3<double>::zero(0, 0, 0);
}
