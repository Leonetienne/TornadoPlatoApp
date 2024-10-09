#pragma once
#include "Matrix4x4.h"
#include <cstdlib>
#include <iomanip>
#include <ostream>
#include <sstream>

namespace Eule {
    template<typename T>
    class Vector2;

    template<typename T>
    class Vector4;

    /** Representation of a 3d vector.
    * Contains a lot of utility methods.
    */
    template<typename T>
    class Vector3 {
    public:
        Vector3() : x{0}, y{0}, z{0} {}

        Vector3(T _x, T _y, T _z) : x{_x}, y{_y}, z{_z} {}

        Vector3(const Vector3<T> &other) = default;

        Vector3(Vector3<T> &&other) noexcept = default;

        //! Will compute the dot product to another Vector3
        double DotProduct(const Vector3<T> &other) const;

        //! Will compute the cross product to another Vector3
        Vector3<double> CrossProduct(const Vector3<T> &other) const;

        //! Will compute the square magnitude
        double SqrMagnitude() const;

        //! Will compute the magnitude
        double Magnitude() const;

        //! Will return the normalization of this vector
        [[nodiscard]] Vector3<double> Normalize() const;

        //! Will normalize this vector
        void NormalizeSelf();

        //! Will scale self.n by scalar.n
        [[nodiscard]] Vector3<T> VectorScale(const Vector3<T> &scalar) const;

        //! Will lerp itself towards other by t
        void LerpSelf(const Vector3<T> &other, double t);

        //! Will return a lerp result between this and another vector
        [[nodiscard]] Vector3<double> Lerp(const Vector3<T> &other, double t) const;

        //! Will compare if two vectors are similar to a certain epsilon value
        [[nodiscard]] bool Similar(const Vector3<T> &other, double epsilon = 0.00001) const;

        //! Will convert this vector to a Vector3i
        [[nodiscard]] Vector3<int> ToInt() const;

        //! Will convert this vector to a Vector3d
        [[nodiscard]] Vector3<double> ToDouble() const;

        T &operator[](std::size_t idx);

        const T &operator[](std::size_t idx) const;

        Vector3<T> operator+(const Vector3<T> &other) const;

        void operator+=(const Vector3<T> &other);

        Vector3<T> operator-(const Vector3<T> &other) const;

        void operator-=(const Vector3<T> &other);

        Vector3<T> operator*(const T scale) const;

        void operator*=(const T scale);

        Vector3<T> operator/(const T scale) const;

        void operator/=(const T scale);

        Vector3<T> operator*(const Matrix4x4 &mat) const;

        void operator*=(const Matrix4x4 &mat);

        Vector3<T> operator-() const;

        operator Vector2<T>() const; //! Conversion method
        operator Vector4<T>() const; //! Conversion method

        void operator=(const Vector3<T> &other);

        void operator=(Vector3<T> &&other) noexcept;

        bool operator==(const Vector3<T> &other) const;

        bool operator!=(const Vector3<T> &other) const;

        friend std::ostream &operator<<(std::ostream &os, const Vector3<T> &v) {
            return os << "[x: " << v.x << "  y: " << v.y << "  z: " << v.z << "]";
        }

        friend std::wostream &operator<<(std::wostream &os, const Vector3<T> &v) {
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
}
