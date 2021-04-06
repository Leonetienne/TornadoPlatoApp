#pragma once
#include "../Tornado/Vector3.h"
#include "../Tornado/Vector4.h"
#include "../Tornado/Matrix4x4.h"

class Quaternion
{
public:
    Quaternion();

    // Constructs after these values
    Quaternion(const Vector4d values);

    Quaternion(const Quaternion& q);

    // Creates quaternion from euler angles
    Quaternion(const Vector3d eulerAngles);

    // Creates quaternion from rotation matrix
    Quaternion(const Matrix4x4 rotMatrix);

    ~Quaternion();

    // Copies
    Quaternion operator= (const Quaternion& q);

    // Multiplies (applies)
    Quaternion operator* (const Quaternion& q) const;

    // Divides (applies)
    Quaternion operator/ (Quaternion& q) const;

    // Also multiplies
    Quaternion& operator*= (const Quaternion& q);

    // Also divides
    Quaternion& operator/= (const Quaternion& q);

    // Will transform a 3d point around its origin
    Vector3d operator* (const Vector3d& p);

    bool operator== (const Quaternion& q) const;

    bool operator!= (const Quaternion& q) const;

    Quaternion Inverse() const;

    Quaternion Conjugate() const;

    Quaternion UnitQuaternion() const;

    Vector3d RotateVector(const Vector3d vec) const;

    Vector3d ToEulerAngles() const;

    Matrix4x4 ToRotationMatrix() const;

    Vector4d GetRawValues() const;

    Quaternion AngleBetween(const Quaternion& other) const;

    void SetRawValues(const Vector4d values);

    friend std::ostream& operator<< (std::ostream& os, const Quaternion& q);
    friend std::wostream& operator<< (std::wostream& os, const Quaternion& q);

    static Quaternion FromMatrix(const Matrix4x4 mat);
    static Quaternion FromEuler(const Vector3d euler);

private:
    // Scales
    Quaternion operator* (const double scale) const;
    Quaternion& operator*= (const double scale);

    // Quaternion values
    Vector4d v;

    // Will force a regenartion of the euler and matrix caches on further converter calls
    void InvalidateCache();

    // Caches for conversions
    mutable bool isCacheUpToDate_euler = false;
    mutable Vector3d eulerCache;

    mutable bool isCacheUpToDate_matrix = false;
    mutable Matrix4x4 matrixCache;

};
