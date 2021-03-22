#include "Matrix4x4.h"
#include "Vector3.h"

Matrix4x4::Matrix4x4()
{
	// Zero it all
	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			v[i][j] = 0;

	return;
}

Matrix4x4::Matrix4x4(const Matrix4x4& other)
{
	// Clone
	for (std::size_t i = 0; i < 4; i++)
		for (std::size_t j = 0; j < 4; j++)
			v[i][j] = other[i][j];

	return;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const
{
	Matrix4x4 newMatrix;

	// Rotation, Scaling
	newMatrix.a = (a * other.a) + (b * other.e) + (c * other.i);
	newMatrix.b = (a * other.b) + (b * other.f) + (c * other.j);
	newMatrix.c = (a * other.c) + (b * other.g) + (c * other.k);

	newMatrix.e = (e * other.a) + (f * other.e) + (g * other.i);
	newMatrix.f = (e * other.b) + (f * other.f) + (g * other.j);
	newMatrix.g = (e * other.c) + (f * other.g) + (g * other.k);

	newMatrix.i = (i * other.a) + (j * other.e) + (k * other.i);
	newMatrix.j = (i * other.b) + (j * other.f) + (k * other.j);
	newMatrix.k = (i * other.c) + (j * other.g) + (k * other.k);

	// Translation
	newMatrix.d = d + other.d;
	newMatrix.h = h + other.h;
	newMatrix.l = l + other.l;

	return newMatrix;
}

void Matrix4x4::operator*=(const Matrix4x4& other)
{
	Matrix4x4 buffer = *this;

	// Rotation
	a = (buffer.a * other.a) + (buffer.b * other.e) + (buffer.c * other.i);
	b = (buffer.a * other.b) + (buffer.b * other.f) + (buffer.c * other.j);
	c = (buffer.a * other.c) + (buffer.b * other.g) + (buffer.c * other.k);

	e = (buffer.e * other.a) + (buffer.f * other.e) + (buffer.g * other.i);
	f = (buffer.e * other.b) + (buffer.f * other.f) + (buffer.g * other.j);
	g = (buffer.e * other.c) + (buffer.f * other.g) + (buffer.g * other.k);

	i = (buffer.i * other.a) + (buffer.j * other.e) + (buffer.k * other.i);
	j = (buffer.i * other.b) + (buffer.j * other.f) + (buffer.k * other.j);
	k = (buffer.i * other.c) + (buffer.j * other.g) + (buffer.k * other.k);

	// Translation
	d = buffer.d + other.d;
	h = buffer.h + other.h;
	l = buffer.l + other.l;

	return;
}

std::array<double, 4>& Matrix4x4::operator[](std::size_t x)
{
	return v[x];
}

const std::array<double, 4>& Matrix4x4::operator[](std::size_t x) const
{
	return v[x];
}

const Vector3d Matrix4x4::GetTranslationComponent()
{
	return Vector3d(d, h, l);
}
