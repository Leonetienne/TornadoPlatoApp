#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"
#include "Math.h"

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
        return (x * other.x) +
               (y * other.y) +
               (z * other.z);
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
        return Vector3<double>(
            x * scalar.x,
            y * scalar.y,
            z * scalar.z
        );
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
            x /= length;
            y /= length;
            z /= length;
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

        x = it*x + t*other.x;
        y = it*y + t*other.y;
        z = it*z + t*other.z;

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
    void Vector3<T>::operator+=(const Vector3<T>& other)
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
    template<>
    Vector3<double> Vector3<double>::operator*(const Matrix4x4& mat) const
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

    template<typename T>
    bool Vector3<T>::operator==(const Vector3<T>& other) const
    {
        return (x == other.x) &&
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

