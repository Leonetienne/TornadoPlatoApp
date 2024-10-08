#include "Eule.h"

/*** ./../Eule/Collider.cpp ***/



/*** ./../Eule/Math.cpp ***/

#include <array>

using namespace Eule;

int Math::Mod(const int numerator, const int denominator)
{
	if (denominator == 0)
		throw std::logic_error("Division by zero");

	// Quick optimizations:

	// -> 0/n is always 0
	if (numerator == 0)
		return 0;

	// -> operator% works for a > 0 && b > 0
	if (denominator > 0 && numerator > 0)
		return numerator % denominator;

	// Else: generalized formula
	return (denominator + (numerator % denominator)) % denominator;
}

double Math::Oscillate(const double a, const double b, const double counter, const double speed)
{
	return (sin(counter * speed * PI - HALF_PI) * 0.5 + 0.5) * (b - a) + a;
}


/*** ./../Eule/Matrix4x4.cpp ***/


//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
#include <immintrin.h>
#endif

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

	#ifndef _EULE_NO_INTRINSICS_


	/*     <=  Matrix3x3 multiplication =>     */

	// Load matrix components
	__m256d __va1 = _mm256_set_pd(v[0][0], v[0][0], v[0][0], v[1][0]);
	__m256d __va2 = _mm256_set_pd(v[1][0], v[1][0], v[2][0], v[2][0]);

	__m256d __oa1 = _mm256_set_pd(other[0][0], other[0][1], other[0][2], other[0][0]);
	__m256d __oa2 = _mm256_set_pd(other[0][1], other[0][2], other[0][0], other[0][1]);

	__m256d __vb1 = _mm256_set_pd(v[0][1], v[0][1], v[0][1], v[1][1]);
	__m256d __vb2 = _mm256_set_pd(v[1][1], v[1][1], v[2][1], v[2][1]);

	__m256d __ob1 = _mm256_set_pd(other[1][0], other[1][1], other[1][2], other[1][0]);
	__m256d __ob2 = _mm256_set_pd(other[1][1], other[1][2], other[1][0], other[1][1]);

	__m256d __vc1 = _mm256_set_pd(v[0][2], v[0][2], v[0][2], v[1][2]);
	__m256d __vc2 = _mm256_set_pd(v[1][2], v[1][2], v[2][2], v[2][2]);

	__m256d __oc1 = _mm256_set_pd(other[2][0], other[2][1], other[2][2], other[2][0]);
	__m256d __oc2 = _mm256_set_pd(other[2][1], other[2][2], other[2][0], other[2][1]);

	// Initialize sums
	__m256d __sum1 = _mm256_set1_pd(0);
	__m256d __sum2 = _mm256_set1_pd(0);

	// Let's multiply-add them together
	// First, the first block
	__sum1 = _mm256_fmadd_pd(__va1, __oa1, __sum1);
	__sum1 = _mm256_fmadd_pd(__vb1, __ob1, __sum1);
	__sum1 = _mm256_fmadd_pd(__vc1, __oc1, __sum1);

	// Then the second block
	__sum2 = _mm256_fmadd_pd(__va2, __oa2, __sum2);
	__sum2 = _mm256_fmadd_pd(__vb2, __ob2, __sum2);
	__sum2 = _mm256_fmadd_pd(__vc2, __oc2, __sum2);

	// Retrieve results
	double sum1[4];
	double sum2[4];
	
	_mm256_storeu_pd(sum1, __sum1);
	_mm256_storeu_pd(sum2, __sum2);

	// Apply results
	// Block 1
	newMatrix[0][0] = sum1[3];
	newMatrix[0][1] = sum1[2];
	newMatrix[0][2] = sum1[1];
	newMatrix[1][0] = sum1[0];
	
	// Block 2
	newMatrix[1][1] = sum2[3];
	newMatrix[1][2] = sum2[2];
	newMatrix[2][0] = sum2[1];
	newMatrix[2][1] = sum2[0];

	// Does not fit in the intrinsic calculation. Might just calculate 'by hand'.
	newMatrix[2][2] = (v[2][0] * other[0][2]) + (v[2][1] * other[1][2]) + (v[2][2] * other[2][2]);


	/*     <=  Translation component =>     */

	// Load translation components into registers
	__m256d __transSelf = _mm256_set_pd(0, l, h, d);
	__m256d __transOther = _mm256_set_pd(0, other.l, other.h, other.d);

	// Let's add them
	__m256d __sum = _mm256_add_pd(__transSelf, __transOther);

	// Retrieve results
	double sum[4];
	_mm256_storeu_pd(sum, __sum);

	// Apply them
	newMatrix.d = sum[0];
	newMatrix.h = sum[1];
	newMatrix.l = sum[2];

	#else


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

	#endif

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

	#ifndef _EULE_NO_INTRINSICS_

	// Load matrix rows
	__m256d __row0 = _mm256_set_pd(v[0][3], v[0][2], v[0][1], v[0][0]);
	__m256d __row1 = _mm256_set_pd(v[1][3], v[1][2], v[1][1], v[1][0]);
	__m256d __row2 = _mm256_set_pd(v[2][3], v[2][2], v[2][1], v[2][0]);
	__m256d __row3 = _mm256_set_pd(v[3][3], v[3][2], v[3][1], v[3][0]);

	// Load scalar
	__m256d __scalar = _mm256_set1_pd(scalar);

	// Scale values
	__m256d __sr0 = _mm256_mul_pd(__row0, __scalar);
	__m256d __sr1 = _mm256_mul_pd(__row1, __scalar);
	__m256d __sr2 = _mm256_mul_pd(__row2, __scalar);
	__m256d __sr3 = _mm256_mul_pd(__row3, __scalar);

	// Extract results
	_mm256_storeu_pd(m.v[0].data(), __sr0);
	_mm256_storeu_pd(m.v[1].data(), __sr1);
	_mm256_storeu_pd(m.v[2].data(), __sr2);
	_mm256_storeu_pd(m.v[3].data(), __sr3);

	#else

	for (std::size_t x = 0; x < 4; x++)
	for (std::size_t y = 0; y < 4; y++)
		m[x][y] = v[x][y] * scalar;

	#endif

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

	#ifndef _EULE_NO_INTRINSICS_

	// Load matrix rows
	__m256d __row0a = _mm256_set_pd(v[0][3], v[0][2], v[0][1], v[0][0]);
	__m256d __row1a = _mm256_set_pd(v[1][3], v[1][2], v[1][1], v[1][0]);
	__m256d __row2a = _mm256_set_pd(v[2][3], v[2][2], v[2][1], v[2][0]);
	__m256d __row3a = _mm256_set_pd(v[3][3], v[3][2], v[3][1], v[3][0]);

	__m256d __row0b = _mm256_set_pd(other[0][3], other[0][2], other[0][1], other[0][0]);
	__m256d __row1b = _mm256_set_pd(other[1][3], other[1][2], other[1][1], other[1][0]);
	__m256d __row2b = _mm256_set_pd(other[2][3], other[2][2], other[2][1], other[2][0]);
	__m256d __row3b = _mm256_set_pd(other[3][3], other[3][2], other[3][1], other[3][0]);

	// Add rows
	__m256d __sr0 = _mm256_add_pd(__row0a, __row0b);
	__m256d __sr1 = _mm256_add_pd(__row1a, __row1b);
	__m256d __sr2 = _mm256_add_pd(__row2a, __row2b);
	__m256d __sr3 = _mm256_add_pd(__row3a, __row3b);

	// Extract results
	_mm256_storeu_pd(m.v[0].data(), __sr0);
	_mm256_storeu_pd(m.v[1].data(), __sr1);
	_mm256_storeu_pd(m.v[2].data(), __sr2);
	_mm256_storeu_pd(m.v[3].data(), __sr3);

	#else

	for (std::size_t x = 0; x < 4; x++)
	for (std::size_t y = 0; y < 4; y++)
		m[x][y] = v[x][y] + other[x][y];

	#endif

	return m;
}

void Matrix4x4::operator+=(const Matrix4x4& other)
{
	#ifndef _EULE_NO_INTRINSICS_
	// Doing it again is a tad directer, and thus faster. We avoid an intermittent Matrix4x4 instance

	// Load matrix rows
	__m256d __row0a = _mm256_set_pd(v[0][3], v[0][2], v[0][1], v[0][0]);
	__m256d __row1a = _mm256_set_pd(v[1][3], v[1][2], v[1][1], v[1][0]);
	__m256d __row2a = _mm256_set_pd(v[2][3], v[2][2], v[2][1], v[2][0]);
	__m256d __row3a = _mm256_set_pd(v[3][3], v[3][2], v[3][1], v[3][0]);

	__m256d __row0b = _mm256_set_pd(other[0][3], other[0][2], other[0][1], other[0][0]);
	__m256d __row1b = _mm256_set_pd(other[1][3], other[1][2], other[1][1], other[1][0]);
	__m256d __row2b = _mm256_set_pd(other[2][3], other[2][2], other[2][1], other[2][0]);
	__m256d __row3b = _mm256_set_pd(other[3][3], other[3][2], other[3][1], other[3][0]);

	// Add rows
	__m256d __sr0 = _mm256_add_pd(__row0a, __row0b);
	__m256d __sr1 = _mm256_add_pd(__row1a, __row1b);
	__m256d __sr2 = _mm256_add_pd(__row2a, __row2b);
	__m256d __sr3 = _mm256_add_pd(__row3a, __row3b);

	// Extract results
	_mm256_storeu_pd(v[0].data(), __sr0);
	_mm256_storeu_pd(v[1].data(), __sr1);
	_mm256_storeu_pd(v[2].data(), __sr2);
	_mm256_storeu_pd(v[3].data(), __sr3);

	#else
	
	*this = *this + other;
	
	#endif

	return;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& other) const
{
	Matrix4x4 m;

	#ifndef _EULE_NO_INTRINSICS_

	// Load matrix rows
	__m256d __row0a = _mm256_set_pd(v[0][3], v[0][2], v[0][1], v[0][0]);
	__m256d __row1a = _mm256_set_pd(v[1][3], v[1][2], v[1][1], v[1][0]);
	__m256d __row2a = _mm256_set_pd(v[2][3], v[2][2], v[2][1], v[2][0]);
	__m256d __row3a = _mm256_set_pd(v[3][3], v[3][2], v[3][1], v[3][0]);

	__m256d __row0b = _mm256_set_pd(other[0][3], other[0][2], other[0][1], other[0][0]);
	__m256d __row1b = _mm256_set_pd(other[1][3], other[1][2], other[1][1], other[1][0]);
	__m256d __row2b = _mm256_set_pd(other[2][3], other[2][2], other[2][1], other[2][0]);
	__m256d __row3b = _mm256_set_pd(other[3][3], other[3][2], other[3][1], other[3][0]);

	// Subtract rows
	__m256d __sr0 = _mm256_sub_pd(__row0a, __row0b);
	__m256d __sr1 = _mm256_sub_pd(__row1a, __row1b);
	__m256d __sr2 = _mm256_sub_pd(__row2a, __row2b);
	__m256d __sr3 = _mm256_sub_pd(__row3a, __row3b);

	// Extract results
	_mm256_storeu_pd(m.v[0].data(), __sr0);
	_mm256_storeu_pd(m.v[1].data(), __sr1);
	_mm256_storeu_pd(m.v[2].data(), __sr2);
	_mm256_storeu_pd(m.v[3].data(), __sr3);

	#else

	for (std::size_t x = 0; x < 4; x++)
		for (std::size_t y = 0; y < 4; y++)
			m[x][y] = v[x][y] - other[x][y];

	#endif

	return m;
}

void Matrix4x4::operator-=(const Matrix4x4& other)
{
	#ifndef _EULE_NO_INTRINSICS_
	// Doing it again is a tad directer, and thus faster. We avoid an intermittent Matrix4x4 instance

	// Load matrix rows
	__m256d __row0a = _mm256_set_pd(v[0][3], v[0][2], v[0][1], v[0][0]);
	__m256d __row1a = _mm256_set_pd(v[1][3], v[1][2], v[1][1], v[1][0]);
	__m256d __row2a = _mm256_set_pd(v[2][3], v[2][2], v[2][1], v[2][0]);
	__m256d __row3a = _mm256_set_pd(v[3][3], v[3][2], v[3][1], v[3][0]);

	__m256d __row0b = _mm256_set_pd(other[0][3], other[0][2], other[0][1], other[0][0]);
	__m256d __row1b = _mm256_set_pd(other[1][3], other[1][2], other[1][1], other[1][0]);
	__m256d __row2b = _mm256_set_pd(other[2][3], other[2][2], other[2][1], other[2][0]);
	__m256d __row3b = _mm256_set_pd(other[3][3], other[3][2], other[3][1], other[3][0]);

	// Subtract rows
	__m256d __sr0 = _mm256_sub_pd(__row0a, __row0b);
	__m256d __sr1 = _mm256_sub_pd(__row1a, __row1b);
	__m256d __sr2 = _mm256_sub_pd(__row2a, __row2b);
	__m256d __sr3 = _mm256_sub_pd(__row3a, __row3b);

	// Extract results
	_mm256_storeu_pd(v[0].data(), __sr0);
	_mm256_storeu_pd(v[1].data(), __sr1);
	_mm256_storeu_pd(v[2].data(), __sr2);
	_mm256_storeu_pd(v[3].data(), __sr3);

	#else

	* this = *this - other;

	#endif

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

	for (std::size_t i = 0; i < 3; i++)
		for (std::size_t j = 0; j < 3; j++)
			trans[j][i] = v[i][j];

	return trans;
}

Matrix4x4 Matrix4x4::Transpose4x4() const
{
	Matrix4x4 trans;

	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			trans[j][i] = v[i][j];

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

	for (std::size_t y = 0; y < n; y++)
		for (std::size_t x = 0; x < n; x++)
		{
			if ((y != p) && (x != q))
			{
				cofs[i][j] = v[y][x];
				j++;
			}

			if (j == n - 1)
			{
				j = 0;
				i++;
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
	if (n > 4)
		throw std::runtime_error("Dimension out of range! 0 <= n <= 4");

	double d = 0;
	double sign = 1;

	if (n == 1)
		return v[0][0];

	for (std::size_t x = 0; x < n; x++)
	{
		Matrix4x4 cofs = GetCofactors(0, x, n);

		d += sign * v[0][x] * cofs.Determinant(n - 1);
		sign = -sign;
	}

	return d;
}

Matrix4x4 Matrix4x4::Adjoint(std::size_t n) const
{
	if (n > 4)
		throw std::runtime_error("Dimension out of range! 0 <= n <= 4");

	Matrix4x4 adj;
	double sign = 1;

	for (std::size_t i = 0; i < n; i++)
		for (std::size_t j = 0; j < n; j++)
		{
			Matrix4x4 cofs = GetCofactors(i, j, n);

			// sign of adj[j][i] positive if sum of row
			// and column indexes is even.
			sign = ((i + j) % 2 == 0) ? 1 : -1;

			// Interchanging rows and columns to get the
			// transpose of the cofactor matrix
			adj[j][i] = sign * (cofs.Determinant(n - 1));
		}

	return adj;
}

Matrix4x4 Matrix4x4::Inverse3x3() const
{
	Matrix4x4 inv;

	double det = Determinant(3);
	if (det == 0.0)
		throw std::runtime_error("Matrix3x3 not inversible!");

	Matrix4x4 adj = Adjoint(3);

	for (std::size_t i = 0; i < 3; i++)
		for (std::size_t j = 0; j < 3; j++)
			inv[i][j] = adj[i][j] / det;

	inv.SetTranslationComponent(-GetTranslationComponent());

	return inv;
}

Matrix4x4 Matrix4x4::Inverse4x4() const
{
	Matrix4x4 inv;

	double det = Determinant(4);
	if (det == 0.0)
		throw std::runtime_error("Matrix4x4 not inversible!");

	Matrix4x4 adj = Adjoint(4);

	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			inv[i][j] = adj[i][j] / det;

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
	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			if (!Math::Similar(v[i][j], other[i][j], epsilon))
				return false;

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


/*** ./../Eule/Quaternion.cpp ***/

#include <algorithm>
#include <functional>
#include <cmath>

//#define _EULE_NO_INTRINSICS_
#ifndef _EULE_NO_INTRINSICS_
#include <immintrin.h>
#endif

using namespace Eule;

Quaternion::Quaternion()
{
	v = Vector4d(0, 0, 0, 1);
	return;
}

Quaternion::Quaternion(const Vector4d values)
{
	v = values;
	return;
}

Quaternion::Quaternion(const Quaternion& q)
{
	v = q.v;
	return;
}

Quaternion::Quaternion(const Vector3d eulerAngles)
{
	Vector3d eulerRad = eulerAngles * Deg2Rad;

	#ifndef _EULE_NO_INTRINSICS_

	// Calculate sine and cos values
	__m256d __vec = _mm256_set_pd(0, eulerRad.z, eulerRad.y, eulerRad.x);
	__vec = _mm256_mul_pd(__vec, _mm256_set1_pd(0.5));
	__m256d __cos;
	__m256d __sin = _mm256_sincos_pd(&__cos, __vec);

	// Create multiplication vectors
	double sin[4];
	double cos[4];

	_mm256_storeu_pd(sin, __sin);
	_mm256_storeu_pd(cos, __cos);

	__m256d __a = _mm256_set_pd(cos[0], cos[0], sin[0], cos[0]);
	__m256d __b = _mm256_set_pd(cos[1], sin[1], cos[1], cos[1]);
	__m256d __c = _mm256_set_pd(sin[2], cos[2], cos[2], cos[2]);

	__m256d __d = _mm256_set_pd(sin[0], sin[0], cos[0], sin[0]);
	__m256d __e = _mm256_set_pd(sin[1], cos[1], sin[1], sin[1]);
	__m256d __f = _mm256_set_pd(cos[2], sin[2], sin[2], sin[2]);

	// Multiply them
	__m256d __abc;
	__abc = _mm256_mul_pd(__a, __b);
	__abc = _mm256_mul_pd(__abc, __c);

	__m256d __def;
	__def = _mm256_mul_pd(__d, __e);
	__def = _mm256_mul_pd(__def, __f);

	// Extract results
	double abc[4];
	double def[4];

	_mm256_storeu_pd(abc, __abc);
	_mm256_storeu_pd(def, __def);

	// Sum them up
	v.w = abc[0] + def[0];
	v.x = abc[1] - def[1];
	v.y = abc[2] + def[2];
	v.z = abc[3] - def[3];

	#else

	const double cy = cos(eulerRad.z * 0.5);
	const double sy = sin(eulerRad.z * 0.5);
	const double cp = cos(eulerRad.y * 0.5);
	const double sp = sin(eulerRad.y * 0.5);
	const double cr = cos(eulerRad.x * 0.5);
	const double sr = sin(eulerRad.x * 0.5);

	v.w = cr * cp * cy + sr * sp * sy;
	v.x = sr * cp * cy - cr * sp * sy;
	v.y = cr * sp * cy + sr * cp * sy;
	v.z = cr * cp * sy - sr * sp * cy;

	#endif

	return;
}

Quaternion::~Quaternion()
{
	return;
}

Quaternion Quaternion::operator= (const Quaternion& q)
{
	InvalidateCache();

	v = q.v;

	return (*this);
}

Quaternion Quaternion::operator* (const Quaternion& q) const
{
	return Quaternion(Vector4d(
		v.w * q.v.x + v.x * q.v.w + v.y * q.v.z - v.z * q.v.y,
		v.w * q.v.y + v.y * q.v.w + v.z * q.v.x - v.x * q.v.z,
		v.w * q.v.z + v.z * q.v.w + v.x * q.v.y - v.y * q.v.x,
		v.w * q.v.w - v.x * q.v.x - v.y * q.v.y - v.z * q.v.z
	));
}

Quaternion Quaternion::operator*(const double scale) const
{
	return Quaternion(v * scale);
}

Quaternion Quaternion::operator/ (Quaternion& q) const
{
	return ((*this) * (q.Inverse()));
}

Quaternion& Quaternion::operator*= (const Quaternion& q)
{
	InvalidateCache();

	Vector4d bufr = v;
	v.x = bufr.w * q.v.x + bufr.x * q.v.w + bufr.y * q.v.z - bufr.z * q.v.y; // x
	v.y = bufr.w * q.v.y + bufr.y * q.v.w + bufr.z * q.v.x - bufr.x * q.v.z; // y
	v.z = bufr.w * q.v.z + bufr.z * q.v.w + bufr.x * q.v.y - bufr.y * q.v.x; // z
	v.w = bufr.w * q.v.w - bufr.x * q.v.x - bufr.y * q.v.y - bufr.z * q.v.z; // w

	return (*this);
}

Quaternion& Quaternion::operator*=(const double scale)
{
	InvalidateCache();

	v *= scale;
	return (*this);
}

Quaternion& Quaternion::operator/= (const Quaternion& q)
{
	InvalidateCache();

	(*this) = (*this) * q.Inverse();
	return (*this);
}

Vector3d Quaternion::operator*(const Vector3d& p) const
{
	return RotateVector(p);
}

bool Quaternion::operator== (const Quaternion& q) const
{
	return (v.Similar(q.v)) || (v.Similar(q.v * -1));
}

bool Quaternion::operator!= (const Quaternion& q) const
{
	return (!v.Similar(q.v)) && (!v.Similar(q.v * -1));
}

Quaternion Quaternion::Inverse() const
{
	const std::lock_guard<std::mutex> lock(lock_inverseCache);

	if (!isCacheUpToDate_inverse)
	{
		cache_inverse = (Conjugate() * (1.0 / v.SqrMagnitude())).v;

		isCacheUpToDate_inverse = true;
	}

	return Quaternion(cache_inverse);
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(Vector4d(-v.x, -v.y, -v.z, v.w));
}

Quaternion Quaternion::UnitQuaternion() const
{
	return (*this) * (1.0 / v.Magnitude());
}

Vector3d Quaternion::RotateVector(const Vector3d& vec) const
{
	Quaternion pure(Vector4d(vec.x, vec.y, vec.z, 0));

	//Quaternion f = Conjugate() * pure * (*this);
	//Quaternion f = Inverse().Conjugate() * pure * (this->Inverse());
	
	
	Quaternion f = Inverse() * pure * (*this);

	Vector3d toRet;
	toRet.x = f.v.x;
	toRet.y = f.v.y;
	toRet.z = f.v.z;

	return toRet;
}

Vector3d Quaternion::ToEulerAngles() const
{
	const std::lock_guard<std::mutex> lock(lock_eulerCache);

	if (!isCacheUpToDate_euler)
	{
		Vector3d euler;
		// roll (x-axis rotation)
		double sinr_cosp = 2.0 * (v.w * v.x + v.y * v.z);
		double cosr_cosp = 1.0 - 2.0 * (v.x * v.x + v.y * v.y);
		euler.x = std::atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		double sinp = 2.0 * (v.w * v.y - v.z * v.x);
		if (std::abs(sinp) >= 1)
			euler.y = std::copysign(PI / 2, sinp); // use 90 degrees if out of range
		else
			euler.y = std::asin(sinp);

		// yaw (z-axis rotation)
		double siny_cosp = 2.0 * (v.w * v.z + v.x * v.y);
		double cosy_cosp = 1.0 - 2.0 * (v.y * v.y + v.z * v.z);
		euler.z = std::atan2(siny_cosp, cosy_cosp);

		euler *= Rad2Deg;

		cache_euler = euler;
		isCacheUpToDate_matrix = true;
	}

	return cache_euler;
}

Matrix4x4 Quaternion::ToRotationMatrix() const
{
	const std::lock_guard<std::mutex> lock(lock_matrixCache);

	if (!isCacheUpToDate_matrix)
	{
		Matrix4x4 m;

		const double sqx = v.x * v.x;
		const double sqy = v.y * v.y;
		const double sqz = v.z * v.z;
		const double sqw = v.w * v.w;
		const double x = v.x;
		const double y = v.y;
		const double z = v.z;
		const double w = v.w;

		// invs (inverse square length) is only required if quaternion is not already normalised
		double invs = 1.0 / (sqx + sqy + sqz + sqw);
		
		// since sqw + sqx + sqy + sqz =1/invs*invs

		// yaw (y)
		m.c = ((2 * x * z) - (2 * w * y)) * invs;
		m.f = (1 - (2 * sqx) - (2 * sqz)) * invs;
		m.i = ((2 * x * z) + (2 * w * y)) * invs;

		// pitch (x)
		m.a = (1 - (2 * sqy) - (2 * sqz)) * invs;
		m.g = ((2 * y * z) + (2 * w * x)) * invs;
		m.j = ((2 * y * z) - (2 * w * x)) * invs;

		// roll (z)
		m.b = ((2 * x * v.y) + (2 * w * z)) * invs;
		m.e = ((2 * x * v.y) - (2 * w * z)) * invs;
		m.k = (1 - (2 * sqx) - (2 * sqy)) * invs;

		m.p = 1;
		
		cache_matrix = m;
		isCacheUpToDate_matrix = true;
	}

	return cache_matrix;
}

Vector4d Quaternion::GetRawValues() const
{
	return v;
}

Quaternion Quaternion::AngleBetween(const Quaternion& other) const
{
	return other * Conjugate();
}

void Quaternion::SetRawValues(const Vector4d values)
{
	InvalidateCache();

	v = values;

	return;
}

Quaternion Quaternion::Lerp(const Quaternion& other, double t) const
{
	return Quaternion(v.Lerp(other.v, t)).UnitQuaternion();
}

void Quaternion::InvalidateCache()
{
	isCacheUpToDate_euler = false;
	isCacheUpToDate_matrix = false;
	isCacheUpToDate_inverse = false;

	return;
}

namespace Eule
{
	std::ostream& operator<< (std::ostream& os, const Quaternion& q)
	{
		os << "[" << q.v << "]";
		return os;
	}

	std::wostream& operator<< (std::wostream& os, const Quaternion& q)
	{
		os << L"[" << q.v << L"]";
		return os;
	}
}


/*** ./../Eule/Random.cpp ***/

#include <array>

using namespace Eule;

// Checks if the random number generator is initialized. Does nothing if it is, initializes if it isn't.
#define MAKE_SURE_RNG_IS_INITIALIZED if (!isRngInitialized) InitRng();

void Random::InitRng()
{
	// Create truly random source (from hardware events)
	std::random_device randomSource;

	// Generate enough truly random values to populate the entire state of the mersenne twister
	std::array<int, std::mt19937::state_size> seedValues;
	std::generate_n(seedValues.data(), seedValues.size(), std::ref(randomSource));
	std::seed_seq seedSequence(seedValues.begin(), seedValues.end());

	// Seed the mersenne twister with these values
	rng = std::mt19937(seedSequence);

	isRngInitialized = true;

	return;
}

// Will return a random double between 0 and 1
double Random::RandomFloat()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return (rng() % 694206942069ll) / 694206942069.0;
}

// Will return a random unsigned integer.
unsigned int Random::RandomUint()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return rng();
}

// Will return a random integer
unsigned int Random::RandomInt()
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	// Since this is supposed to return a random value anyways,
	// we can let the random uint overflow without any problems.
	return (int)rng();
}

// Will return a random double within a range  
// These bounds are INCLUSIVE!
double Random::RandomRange(double min, double max)
{
	return (RandomFloat() * (max - min)) + min;
}

// Will return a random integer within a range. This is faster than '(int)RandomRange(x,y)'
// These bounds are INCLUSIVE!
int Random::RandomIntRange(int min, int max)
{
	MAKE_SURE_RNG_IS_INITIALIZED;

	return (rng() % (max + 1 - min)) + min;
}

bool Random::RandomChance(const double chance)
{
	return RandomFloat() <= chance;
}

std::mt19937 Random::rng;
bool Random::isRngInitialized = false;


/*** ./../Eule/TrapazoidalPrismCollider.cpp ***/


using namespace Eule;

TrapazoidalPrismCollider::TrapazoidalPrismCollider()
{
	return;
}

void TrapazoidalPrismCollider::operator=(const TrapazoidalPrismCollider& other)
{
	vertices = other.vertices;
	faceNormals = other.faceNormals;

	return;
}

void TrapazoidalPrismCollider::operator=(TrapazoidalPrismCollider&& other) noexcept
{
	vertices = std::move(other.vertices);
	faceNormals = std::move(other.faceNormals);

	return;
}

const Vector3d& TrapazoidalPrismCollider::GetVertex(std::size_t index) const
{
	return vertices[index];
}

void TrapazoidalPrismCollider::SetVertex(std::size_t index, const Vector3d value)
{
	vertices[index] = value;
	GenerateNormalsFromVertices();
	return;
}

void TrapazoidalPrismCollider::GenerateNormalsFromVertices()
{
	faceNormals[(std::size_t)FACE_NORMALS::LEFT] =
		(vertices[BACK|LEFT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[FRONT|LEFT|TOP] - vertices[FRONT|LEFT|BOTTOM]);
	
	faceNormals[(std::size_t)FACE_NORMALS::RIGHT] =
		(vertices[FRONT|RIGHT|TOP] - vertices[FRONT|RIGHT|BOTTOM])
		.CrossProduct(vertices[BACK|RIGHT|BOTTOM] - vertices[FRONT|RIGHT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::FRONT] =
		(vertices[FRONT|LEFT|TOP] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[FRONT|RIGHT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::BACK] =
		(vertices[BACK|RIGHT|BOTTOM] - vertices[BACK|LEFT|BOTTOM])
		.CrossProduct(vertices[BACK|LEFT|TOP] - vertices[BACK|LEFT|BOTTOM]);

	faceNormals[(std::size_t)FACE_NORMALS::TOP] =
		(vertices[BACK|LEFT|TOP] - vertices[FRONT|LEFT|TOP])
		.CrossProduct(vertices[FRONT|RIGHT|TOP] - vertices[FRONT|LEFT|TOP]);

	faceNormals[(std::size_t)FACE_NORMALS::BOTTOM] =
		(vertices[FRONT|RIGHT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM])
		.CrossProduct(vertices[BACK|LEFT|BOTTOM] - vertices[FRONT|LEFT|BOTTOM]);

	return;
}

double TrapazoidalPrismCollider::FaceDot(FACE_NORMALS face, const Vector3d& point) const
{
	// This vertex is the one being used twice to calculate the normals
	std::size_t coreVertexIdx;
	switch (face)
	{
	case FACE_NORMALS::LEFT:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;

	case FACE_NORMALS::RIGHT:
		coreVertexIdx = FRONT|RIGHT|BOTTOM;
		break;
	
	case FACE_NORMALS::FRONT:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;
	
	case FACE_NORMALS::BACK:
		coreVertexIdx = BACK|LEFT|BOTTOM;
		break;
	
	case FACE_NORMALS::TOP:
		coreVertexIdx = FRONT|LEFT|TOP;
		break;
	
	case FACE_NORMALS::BOTTOM:
		coreVertexIdx = FRONT|LEFT|BOTTOM;
		break;
	}

	if ((std::size_t)face < 6)
		return faceNormals[(std::size_t)face].DotProduct(point - vertices[coreVertexIdx]);
	return 1;
}

bool TrapazoidalPrismCollider::Contains(const Vector3d& point) const
{
	for (std::size_t i = 0; i < 6; i++)
		if (FaceDot((FACE_NORMALS)i, point) < 0)
			return false;

	return true;
}


/*** ./../Eule/Vector2.cpp ***/


template<typename T>
Eule::Vector2<T>::operator Eule::Vector3<T>() const
{
	return Vector3<T>(x, y, 0);
}

template<typename T>
Eule::Vector2<T>::operator Eule::Vector4<T>() const
{
	return Vector4<T>(x, y, 0, 0);
}


/*** ./../Eule/Vector3.cpp ***/





template<typename T>
Eule::Vector3<T>::operator Eule::Vector2<T>() const
{
	return Vector2<T>(x, y);
}

template<typename T>
Eule::Vector3<T>::operator Eule::Vector4<T>() const
{
	return Vector4<T>(x, y, z, 0);
}


/*** ./../Eule/Vector4.cpp ***/


template<typename T>
Eule::Vector4<T>::operator Eule::Vector2<T>() const
{
	return Vector2<T>(x, y);
}

template<typename T>
Eule::Vector4<T>::operator Eule::Vector3<T>() const
{
	return Vector3<T>(x, y, z);
}

