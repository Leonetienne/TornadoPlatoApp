#include "Matrix4x4.h"
#include "Vector3.h"
#include "Math.h"

using namespace Eule;

Matrix4x4::Matrix4x4()
{
	// Create identity matrix
	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			v[i][j] = double(i == j);

	return;
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
{
	v = other.v;
	return;
}

Matrix4x4::Matrix4x4(Matrix4x4&& other) noexcept
{
	v = std::move(other.v);
	return;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	Matrix4x4 newMatrix;
	newMatrix.p = 1;

	// Rotation, Scaling
	newMatrix[0][0] = (v[0][0] * other[0][0]) + (v[0][1] * other[1][0]) + (v[0][2] * other[2][0]);
	newMatrix[0][1] = (v[0][0] * other[0][1]) + (v[0][1] * other[1][1]) + (v[0][2] * other[2][1]);
	newMatrix[0][2] = (v[0][0] * other[0][2]) + (v[0][1] * other[1][2]) + (v[0][2] * other[2][2]);
	
	newMatrix[1][0] = (v[1][0] * other[0][0]) + (v[1][1] * other[1][0]) + (v[1][2] * other[2][0]);
	newMatrix[1][1] = (v[1][0] * other[0][1]) + (v[1][1] * other[1][1]) + (v[1][2] * other[2][1]);
	newMatrix[1][2] = (v[1][0] * other[0][2]) + (v[1][1] * other[1][2]) + (v[1][2] * other[2][2]);
	
	newMatrix[2][0] = (v[2][0] * other[0][0]) + (v[2][1] * other[1][0]) + (v[2][2] * other[2][0]);
	newMatrix[2][1] = (v[2][0] * other[0][1]) + (v[2][1] * other[1][1]) + (v[2][2] * other[2][1]);
	newMatrix[2][2] = (v[2][0] * other[0][2]) + (v[2][1] * other[1][2]) + (v[2][2] * other[2][2]);
	
	// Translation
	newMatrix[0][3] = v[0][3] + other[0][3];
	newMatrix[1][3] = v[1][3] + other[1][3];
	newMatrix[2][3] = v[2][3] + other[2][3];

	return newMatrix;
}

void Matrix4x4::operator*=(const Matrix4x4& other)
{
	*this = *this * other;
	return;
}

Matrix4x4 Matrix4x4::operator/(const Matrix4x4& other) const
{
	return *this * other.Inverse3x3();
}

void Matrix4x4::operator/=(const Matrix4x4& other)
{
	*this = *this * other.Inverse3x3();
	return;
}

Matrix4x4 Matrix4x4::operator*(const double scalar) const
{
	Matrix4x4 m;

	for (std::size_t x = 0; x < 4; x++) {
	    for (std::size_t y = 0; y < 4; y++) {
		    m[x][y] = v[x][y] * scalar;
        }
    }

	return m;
}

void Matrix4x4::operator*=(const double scalar)
{
	*this = *this * scalar;
	return;
}

Matrix4x4 Matrix4x4::operator/(const double denominator) const
{
	const double precomputeDivision = 1.0 / denominator;

	return *this * precomputeDivision;
}

void Matrix4x4::operator/=(const double denominator)
{
	*this = *this / denominator;
	return;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& other) const
{
	Matrix4x4 m;

	for (std::size_t x = 0; x < 4; x++) {
        for (std::size_t y = 0; y < 4; y++) {
            m[x][y] = v[x][y] + other[x][y];
        }
    }

	return m;
}

void Matrix4x4::operator+=(const Matrix4x4& other)
{
	*this = *this + other;
	return;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
	Matrix4x4 m;

	for (std::size_t x = 0; x < 4; x++) {
        for (std::size_t y = 0; y < 4; y++) {
            m[x][y] = v[x][y] - other[x][y];
        }
    }

	return m;
}

void Matrix4x4::operator-=(const Matrix4x4& other)
{
	* this = *this - other;
	return;
}

std::array<double, 4>& Matrix4x4::operator[](std::size_t y)
{
	return v[y];
}

const std::array<double, 4>& Matrix4x4::operator[](std::size_t y) const
{
	return v[y];
}

void Matrix4x4::operator=(const Matrix4x4& other)
{
	v = other.v;
	return;
}

void Matrix4x4::operator=(Matrix4x4&& other) noexcept
{
	v = std::move(other.v);
	return;
}

bool Matrix4x4::operator==(const Matrix4x4& other)
{
	return v == other.v;
}

bool Matrix4x4::operator!=(const Matrix4x4& other)
{
	return !operator==(other);
}

bool Matrix4x4::operator==(const Matrix4x4& other) const
{
	return v == other.v;
}

bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
	return !operator==(other);
}

const Vector3d Matrix4x4::GetTranslationComponent() const
{
	return Vector3d(d, h, l);
}

void Matrix4x4::SetTranslationComponent(const Vector3d& trans)
{
	d = trans.x;
	h = trans.y;
	l = trans.z;
	return;
}

Matrix4x4 Matrix4x4::DropTranslationComponents() const
{
	Matrix4x4 m(*this);
	m.d = 0;
	m.h = 0;
	m.l = 0;
	return m;
}

Matrix4x4 Matrix4x4::Transpose3x3() const
{
	Matrix4x4 trans(*this); // Keep other cells

	for (std::size_t i = 0; i < 3; i++) {
        for (std::size_t j = 0; j < 3; j++) {
            trans[j][i] = v[i][j];
        }
    }

	return trans;
}

Matrix4x4 Matrix4x4::Transpose4x4() const
{
	Matrix4x4 trans;

	for (std::size_t i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < 4; j++) {
            trans[j][i] = v[i][j];
        }
    }

	return trans;
}

Matrix4x4 Matrix4x4::Multiply4x4(const Matrix4x4& o) const
{
	Matrix4x4 m;

	m[0][0] = (v[0][0]*o[0][0]) + (v[0][1]*o[1][0]) + (v[0][2]*o[2][0]) + (v[0][3]*o[3][0]);
	m[0][1] = (v[0][0]*o[0][1]) + (v[0][1]*o[1][1]) + (v[0][2]*o[2][1]) + (v[0][3]*o[3][1]);
	m[0][2] = (v[0][0]*o[0][2]) + (v[0][1]*o[1][2]) + (v[0][2]*o[2][2]) + (v[0][3]*o[3][2]);
	m[0][3] = (v[0][0]*o[0][3]) + (v[0][1]*o[1][3]) + (v[0][2]*o[2][3]) + (v[0][3]*o[3][3]);

	m[1][0] = (v[1][0]*o[0][0]) + (v[1][1]*o[1][0]) + (v[1][2]*o[2][0]) + (v[1][3]*o[3][0]);
	m[1][1] = (v[1][0]*o[0][1]) + (v[1][1]*o[1][1]) + (v[1][2]*o[2][1]) + (v[1][3]*o[3][1]);
	m[1][2] = (v[1][0]*o[0][2]) + (v[1][1]*o[1][2]) + (v[1][2]*o[2][2]) + (v[1][3]*o[3][2]);
	m[1][3] = (v[1][0]*o[0][3]) + (v[1][1]*o[1][3]) + (v[1][2]*o[2][3]) + (v[1][3]*o[3][3]);

	m[2][0] = (v[2][0]*o[0][0]) + (v[2][1]*o[1][0]) + (v[2][2]*o[2][0]) + (v[2][3]*o[3][0]);
	m[2][1] = (v[2][0]*o[0][1]) + (v[2][1]*o[1][1]) + (v[2][2]*o[2][1]) + (v[2][3]*o[3][1]);
	m[2][2] = (v[2][0]*o[0][2]) + (v[2][1]*o[1][2]) + (v[2][2]*o[2][2]) + (v[2][3]*o[3][2]);
	m[2][3] = (v[2][0]*o[0][3]) + (v[2][1]*o[1][3]) + (v[2][2]*o[2][3]) + (v[2][3]*o[3][3]);

	m[3][0] = (v[3][0]*o[0][0]) + (v[3][1]*o[1][0]) + (v[3][2]*o[2][0]) + (v[3][3]*o[3][0]);
	m[3][1] = (v[3][0]*o[0][1]) + (v[3][1]*o[1][1]) + (v[3][2]*o[2][1]) + (v[3][3]*o[3][1]);
	m[3][2] = (v[3][0]*o[0][2]) + (v[3][1]*o[1][2]) + (v[3][2]*o[2][2]) + (v[3][3]*o[3][2]);
	m[3][3] = (v[3][0]*o[0][3]) + (v[3][1]*o[1][3]) + (v[3][2]*o[2][3]) + (v[3][3]*o[3][3]);

	return m;
}

Matrix4x4 Matrix4x4::GetCofactors(std::size_t p, std::size_t q, std::size_t n) const
{
	if (n > 4)
		throw std::runtime_error("Dimension out of range! 0 <= n <= 4");

	Matrix4x4 cofs;

	std::size_t i = 0;
	std::size_t j = 0;

	for (std::size_t y = 0; y < n; y++) {
		for (std::size_t x = 0; x < n; x++) {
			if ((y != p) && (x != q)) {
				cofs[i][j] = v[y][x];
				j++;
			}

			if (j == n - 1) {
				j = 0;
				i++;
			}
		}
    }

	return cofs;
}

/*
* BEGIN_REF
* https://www.geeksforgeeks.org/adjoint-inverse-matrix/
*/
double Matrix4x4::Determinant(std::size_t n) const
{
	if (n > 4) {
		throw std::runtime_error("Dimension out of range! 0 <= n <= 4");
    }

	double d = 0;
	double sign = 1;

	if (n == 1) {
		return v[0][0];
    }

	for (std::size_t x = 0; x < n; x++) {
		Matrix4x4 cofs = GetCofactors(0, x, n);

		d += sign * v[0][x] * cofs.Determinant(n - 1);
		sign = -sign;
	}

	return d;
}

Matrix4x4 Matrix4x4::Adjoint(std::size_t n) const
{
	if (n > 4) {
		throw std::runtime_error("Dimension out of range! 0 <= n <= 4");
    }

	Matrix4x4 adj;
	double sign = 1;

	for (std::size_t i = 0; i < n; i++) {
		for (std::size_t j = 0; j < n; j++) {
			Matrix4x4 cofs = GetCofactors(i, j, n);

			// sign of adj[j][i] positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj[j][i] = sign * (cofs.Determinant(n - 1));
		}
    }

	return adj;
}

Matrix4x4 Matrix4x4::Inverse3x3() const
{
	Matrix4x4 inv;

	double det = Determinant(3);

	if (det == 0.0) {
		throw std::runtime_error("Matrix3x3 not inversible!");
    }

	Matrix4x4 adj = Adjoint(3);

	for (std::size_t i = 0; i < 3; i++) {
        for (std::size_t j = 0; j < 3; j++) {
            inv[i][j] = adj[i][j] / det;
        }
    }

	inv.SetTranslationComponent(-GetTranslationComponent());

	return inv;
}

Matrix4x4 Matrix4x4::Inverse4x4() const
{
	Matrix4x4 inv;

	double det = Determinant(4);

	if (det == 0.0) {
		throw std::runtime_error("Matrix4x4 not inversible!");
    }

	Matrix4x4 adj = Adjoint(4);

	for (std::size_t i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < 4; j++) {
            inv[i][j] = adj[i][j] / det;
        }
    }

	return inv;
}

/*
* END REF
*/

bool Matrix4x4::IsInversible3x3() const
{
	return (Determinant(3) != 0);
}

bool Matrix4x4::IsInversible4x4() const
{
	return (Determinant(4) != 0);
}

bool Matrix4x4::Similar(const Matrix4x4& other, double epsilon) const
{
	for (std::size_t i = 0; i < 4; i++) {
        for (std::size_t j = 0; j < 4; j++) {
			if (!Math::Similar(v[i][j], other[i][j], epsilon)) {
				return false;
            }
        }
    }

	return true;
}

namespace Eule
{
	std::ostream& operator<< (std::ostream& os, const Matrix4x4& m)
	{
		os << std::endl;

		for (std::size_t y = 0; y < 4; y++)
		{
			for (std::size_t x = 0; x < 4; x++)
				os << " | " << m[y][x];

			os << " |" << std::endl;
		}

		return os;
	}

	std::wostream& operator<< (std::wostream& os, const Matrix4x4& m)
	{
		os << std::endl;

		for (std::size_t y = 0; y < 4; y++)
		{
			for (std::size_t x = 0; x < 4; x++)
				os << L" | " << m[y][x];

			os << L" |" << std::endl;
		}

		return os;
	}
}
