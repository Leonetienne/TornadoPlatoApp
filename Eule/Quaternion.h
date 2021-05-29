#pragma once
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

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

};
