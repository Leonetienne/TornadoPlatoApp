#pragma once
#include <cstring>
#include <array>

/*
* myMatrix[y][x] = 3
*
*  X ==============>
* Y
* |  # # # # # # # # # # #
* |  #   a   b   c   d   #
* |  #		             #
* |  #   e   f   g   h   #
* |  #                   #
* V  #   i   j   k   l   #
*    #                   #
*    #   m   n   o   p   #
*    # # # # # # # # # # #
*/

template <class T>
class Vector3;
typedef Vector3<double> Vector3d;

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const Matrix4x4& other);

	std::array<std::array<double, 4>, 4> v;

	Matrix4x4 operator*(const Matrix4x4& other) const;
	void operator*=(const Matrix4x4& other);

	std::array<double, 4>& operator[](std::size_t y);
	const std::array<double, 4>& operator[](std::size_t y) const;

	void operator=(const Matrix4x4& other);

	bool operator==(const Matrix4x4& other);
	bool operator!=(const Matrix4x4& other);

	const Vector3d GetTranslationComponent();

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
