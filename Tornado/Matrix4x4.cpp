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
	//newMatrix[0][0] = ((*this)[0][0] * other[0][0]) + ((*this)[1][0] * other[0][1]) + ((*this)[2][0] * other[0][2]);
	//newMatrix[1][0] = ((*this)[0][0] * other[1][0]) + ((*this)[1][0] * other[1][1]) + ((*this)[2][0] * other[1][2]);
	//newMatrix[2][0] = ((*this)[0][0] * other[2][0]) + ((*this)[1][0] * other[2][1]) + ((*this)[2][0] * other[2][2]);
	//
	//newMatrix[0][1] = ((*this)[0][1] * other[0][0]) + ((*this)[1][1] * other[0][1]) + ((*this)[2][1] * other[0][2]);
	//newMatrix[1][1] = ((*this)[0][1] * other[1][0]) + ((*this)[1][1] * other[1][1]) + ((*this)[2][1] * other[1][2]);
	//newMatrix[2][1] = ((*this)[0][1] * other[2][0]) + ((*this)[1][1] * other[2][1]) + ((*this)[2][1] * other[2][2]);
	//
	//newMatrix[0][2] = ((*this)[0][2] * other[0][0]) + ((*this)[1][2] * other[0][1]) + ((*this)[2][2] * other[0][2]);
	//newMatrix[1][2] = ((*this)[0][2] * other[1][0]) + ((*this)[1][2] * other[1][1]) + ((*this)[2][2] * other[1][2]);
	//newMatrix[2][2] = ((*this)[0][2] * other[2][0]) + ((*this)[1][2] * other[2][1]) + ((*this)[2][2] * other[2][2]);
	//
	//// Translation
	//newMatrix[3][0] = (*this)[3][0] + other[3][0];
	//newMatrix[3][1] = (*this)[3][1] + other[3][1];
	//newMatrix[3][2] = (*this)[3][2] + other[3][2];

	// Rotation, Scaling
	newMatrix[0][0] = ((*this)[0][0] * other[0][0]) + ((*this)[0][1] * other[1][0]) + ((*this)[0][2] * other[2][0]);
	newMatrix[0][1] = ((*this)[0][0] * other[0][1]) + ((*this)[0][1] * other[1][1]) + ((*this)[0][2] * other[2][1]);
	newMatrix[0][2] = ((*this)[0][0] * other[0][2]) + ((*this)[0][1] * other[1][2]) + ((*this)[0][2] * other[2][2]);
	
	newMatrix[1][0] = ((*this)[1][0] * other[0][0]) + ((*this)[1][1] * other[1][0]) + ((*this)[1][2] * other[2][0]);
	newMatrix[1][1] = ((*this)[1][0] * other[0][1]) + ((*this)[1][1] * other[1][1]) + ((*this)[1][2] * other[2][1]);
	newMatrix[1][2] = ((*this)[1][0] * other[0][2]) + ((*this)[1][1] * other[1][2]) + ((*this)[1][2] * other[2][2]);
	
	newMatrix[2][0] = ((*this)[2][0] * other[0][0]) + ((*this)[2][1] * other[1][0]) + ((*this)[2][2] * other[2][0]);
	newMatrix[2][1] = ((*this)[2][0] * other[0][1]) + ((*this)[2][1] * other[1][1]) + ((*this)[2][2] * other[2][1]);
	newMatrix[2][2] = ((*this)[2][0] * other[0][2]) + ((*this)[2][1] * other[1][2]) + ((*this)[2][2] * other[2][2]);

	// Translation
	newMatrix[0][3] = (*this)[0][3] + other[0][3];
	newMatrix[1][3] = (*this)[1][3] + other[1][3];
	newMatrix[2][3] = (*this)[2][3] + other[2][3];

	return newMatrix;
}

void Matrix4x4::operator*=(const Matrix4x4& other)
{
	Matrix4x4 buffer = *this;

	//// Rotation
	//(*this)[0][0] = (buffer[0][0] * other[0][0]) + (buffer[1][0] * other[0][1]) + (buffer[2][0] * other[0][2]);
	//(*this)[1][0] = (buffer[0][0] * other[1][0]) + (buffer[1][0] * other[1][1]) + (buffer[2][0] * other[1][2]);
	//(*this)[2][0] = (buffer[0][0] * other[2][0]) + (buffer[1][0] * other[2][1]) + (buffer[2][0] * other[2][2]);
	//
	//(*this)[0][1] = (buffer[0][1] * other[0][0]) + (buffer[1][1] * other[0][1]) + (buffer[2][1] * other[0][2]);
	//(*this)[1][1] = (buffer[0][1] * other[1][0]) + (buffer[1][1] * other[1][1]) + (buffer[2][1] * other[1][2]);
	//(*this)[2][1] = (buffer[0][1] * other[2][0]) + (buffer[1][1] * other[2][1]) + (buffer[2][1] * other[2][2]);
	//
	//(*this)[0][2] = (buffer[0][2] * other[0][0]) + (buffer[1][2] * other[0][1]) + (buffer[2][2] * other[0][2]);
	//(*this)[1][2] = (buffer[0][2] * other[1][0]) + (buffer[1][2] * other[1][1]) + (buffer[2][2] * other[1][2]);
	//(*this)[2][2] = (buffer[0][2] * other[2][0]) + (buffer[1][2] * other[2][1]) + (buffer[2][2] * other[2][2]);
	//
	//// Translation
	//(*this)[3][0] = buffer[3][0] + other[3][0];
	//(*this)[3][1] = buffer[3][1] + other[3][1];
	//(*this)[3][2] = buffer[3][2] + other[3][2];

	// Rotation
	(*this)[0][0] = (buffer[0][0] * other[0][0]) + (buffer[0][1] * other[1][0]) + (buffer[0][2] * other[2][0]);
	(*this)[0][1] = (buffer[0][0] * other[0][1]) + (buffer[0][1] * other[1][1]) + (buffer[0][2] * other[2][1]);
	(*this)[0][2] = (buffer[0][0] * other[0][2]) + (buffer[0][1] * other[1][2]) + (buffer[0][2] * other[2][2]);
	
	(*this)[1][0] = (buffer[1][0] * other[0][0]) + (buffer[1][1] * other[1][0]) + (buffer[1][2] * other[2][0]);
	(*this)[1][1] = (buffer[1][0] * other[0][1]) + (buffer[1][1] * other[1][1]) + (buffer[1][2] * other[2][1]);
	(*this)[1][2] = (buffer[1][0] * other[0][2]) + (buffer[1][1] * other[1][2]) + (buffer[1][2] * other[2][2]);
	
	(*this)[2][0] = (buffer[2][0] * other[0][0]) + (buffer[2][1] * other[1][0]) + (buffer[2][2] * other[2][0]);
	(*this)[2][1] = (buffer[2][0] * other[0][1]) + (buffer[2][1] * other[1][1]) + (buffer[2][2] * other[2][1]);
	(*this)[2][2] = (buffer[2][0] * other[0][2]) + (buffer[2][1] * other[1][2]) + (buffer[2][2] * other[2][2]);

	// Translation
	(*this)[0][3] = buffer[0][3] + other[0][3];
	(*this)[1][3] = buffer[1][3] + other[1][3];
	(*this)[2][3] = buffer[2][3] + other[2][3];

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

const Vector3d Matrix4x4::GetTranslationComponent()
{
	return Vector3d(d, h, l);
}
