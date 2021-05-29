#pragma once
#include <cstring>
#include <array>
#include <ostream>

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

	//! Array holding the matrices values
	std::array<std::array<double, 4>, 4> v;

	Matrix4x4 operator*(const Matrix4x4& other) const;
	void operator*=(const Matrix4x4& other);

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

	bool operator==(const Matrix4x4& other);
	bool operator!=(const Matrix4x4& other);

	//! Will return d,h,l as a Vector3d(x,y,z)
	const Vector3d GetTranslationComponent() const;
	//! Will set d,h,l from a Vector3d(x,y,z)
	void SetTranslationComponent(const Vector3d& trans);

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

	friend std::ostream& operator<<(std::ostream& os, const Matrix4x4& m);
	friend std::wostream& operator<<(std::wostream& os, const Matrix4x4& m);

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
