#include "Vector4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Math.h"

namespace Eule {

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

    template<typename T>
    Vector4<T> Vector4<T>::VectorScale(const Vector4<T>& scalar) const
    {
        return Vector4<T>(
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
            x /= length;
            y /= length;
            z /= length;
            w /= length;
        }

        return;
    }

    // You can't normalize an int vector, ffs!
    // But we need an implementation for T=int
    template<>
    void Vector4<int>::NormalizeSelf()
    {
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
        x = it * x + t * other.x;
        y = it * y + t * other.y;
        z = it * z + t * other.z;
        w = it * w + t * other.w;

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

    template<typename T>
    void Vector4<T>::operator+=(const Vector4<T>& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return;
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

    template<typename T>
    void Vector4<T>::operator-=(const Vector4<T>& other)
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
    template <>
    const Vector4<double> Vector4<double>::up(0, 1, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::down(0, -1, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::right(1, 0, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::left(-1, 0, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::forward(1, 0, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::backward(-1, 0, 0, 0);
    template <>
    const Vector4<double> Vector4<double>::future(0, 0, 0, 1);
    template <>
    const Vector4<double> Vector4<double>::past(0, 0, 0, -1);
    template <>
    const Vector4<double> Vector4<double>::one(1, 1, 1, 1);
    template <>
    const Vector4<double> Vector4<double>::zero(0, 0, 0, 0);
}

