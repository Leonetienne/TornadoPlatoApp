#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Math.h"

namespace Eule {

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

    // Good, optimized chad version for doubles
    template<>
    double Vector2<double>::DotProduct(const Vector2<double>& other) const
    {
        return (x * other.x) +
               (y * other.y);
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
        return Vector2<double>(
            x * scalar.x,
            y * scalar.y
        );
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
            x /= length;
            y /= length;
        }

        return;
    }

    // You can't normalize an int vector, ffs!
    // But we need an implementation for T=int
    template<>
    void Vector2<int>::NormalizeSelf()
    {
        x = 0;
        y = 0;

        return;
    }


    // Good, optimized chad version for doubles
    template<>
    void Vector2<double>::LerpSelf(const Vector2<double>& other, double t)
    {
        const double it = 1.0 - t; // Inverse t

        x = it * x + t * other.x;
        y = it * y + t * other.y;

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

    template<typename T>
    Vector2<T> Vector2<T>::operator+(const Vector2<T>& other) const
    {
        return Vector2<T>(
            x + other.x,
            y + other.y
        );
    }


    template<typename T>
    void Vector2<T>::operator+=(const Vector2<T>& other)
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
    template <>
    const Vector2<double> Vector2<double>::up(0, 1);
    template <>
    const Vector2<double> Vector2<double>::down(0, -1);
    template <>
    const Vector2<double> Vector2<double>::right(1, 0);
    template <>
    const Vector2<double> Vector2<double>::left(-1, 0);
    template <>
    const Vector2<double> Vector2<double>::one(1, 1);
    template <>
    const Vector2<double> Vector2<double>::zero(0, 0);
}
