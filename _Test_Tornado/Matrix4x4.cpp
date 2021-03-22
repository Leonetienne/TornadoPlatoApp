#include "CppUnitTest.h"
#include "../Tornado/Matrix4x4.h"
#include "../Tornado/Vector3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrices
{
	TEST_CLASS(_Matrix4x4)
	{
	public:

		// Tests if all values are 0 after initialization via default constructor
		TEST_METHOD(New_Matrix_All_0)
		{
			Matrix4x4 mat;

			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual(0.0, mat[i][j]);

			return;
		}

		// Test if setting values via array descriptors works
		TEST_METHOD(Can_Set_Values_ArrayDescriptor)
		{
			Matrix4x4 mat;
			mat[0][0] = 1;
			mat[0][1] = 2;
			mat[0][2] = 3;
			mat[0][3] = 4;
			mat[1][0] = 5;
			mat[1][1] = 6;
			mat[1][2] = 7;
			mat[1][3] = 8;
			mat[2][0] = 9;
			mat[2][1] = 10;
			mat[2][2] = 11;
			mat[2][3] = 12;
			mat[3][0] = 13;
			mat[3][1] = 14;
			mat[3][2] = 15;
			mat[3][3] = 16;

			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j + 1), mat[i][j]);

			return;
		}

		// Tests if setting values via letters works
		TEST_METHOD(Can_Set_Values_Letters)
		{
			Matrix4x4 mat;
			mat.a = 1;
			mat.b = 2;
			mat.c = 3;
			mat.d = 4;
			mat.e = 5;
			mat.f = 6;
			mat.g = 7;
			mat.h = 8;
			mat.i = 9;
			mat.j = 10;
			mat.k = 11;
			mat.l = 12;
			mat.m = 13;
			mat.n = 14;
			mat.o = 15;
			mat.p = 16;

			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j + 1), mat[j][i]); // <-- i and j are swapped because mat[x][y]

			return;
		}

		// Tests if values can be read correctly from the reference variables
		TEST_METHOD(Can_Read_Letters)
		{
			Matrix4x4 mat;

			// Populate matrix
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat[j][i] = (double)(i * 4 + j + 1);

			// Check if values can be read
			Assert::AreEqual(1.0, mat.a);
			Assert::AreEqual(2.0, mat.b);
			Assert::AreEqual(3.0, mat.c);
			Assert::AreEqual(4.0, mat.d);
			Assert::AreEqual(5.0, mat.e);
			Assert::AreEqual(6.0, mat.f);
			Assert::AreEqual(7.0, mat.g);
			Assert::AreEqual(8.0, mat.h);
			Assert::AreEqual(9.0, mat.i);
			Assert::AreEqual(10.0, mat.j);
			Assert::AreEqual(11.0, mat.k);
			Assert::AreEqual(12.0, mat.l);
			Assert::AreEqual(13.0, mat.m);
			Assert::AreEqual(14.0, mat.n);
			Assert::AreEqual(15.0, mat.o);
			Assert::AreEqual(16.0, mat.p);

			return;
		}

		// Tests if the copy constructor results in the same values as the reference given
		TEST_METHOD(CopyConstructor_Equal_Values)
		{
			Matrix4x4 mat1;

			// Fill with values
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat1[i][j] = i * 4.0 + j;

			// Copy
			Matrix4x4 mat2(mat1);

			// Both equal?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual(mat1[i][j], mat2[i][j]);

			return;
		}

		// Tests if the equals operator results in the same values as the reference given
		TEST_METHOD(Copy_Via_Equals_Operator)
		{
			Matrix4x4 mat1;

			// Fill with values
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat1[i][j] = i * 4.0 + j;

			// Copy
			Matrix4x4 mat2 = mat1;

			// Both equal?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual(mat1[i][j], mat2[i][j]);

			return;
		}

		// Tests if the values of a matrix constructed via a copy constructor can be changed without modifying the object copied from
		TEST_METHOD(Copy_Is_Independent_CopyConstructor)
		{
			Matrix4x4 mat1;

			// Fill with values
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat1[i][j] = i * 4.0 + j;

			// Copy
			Matrix4x4 mat2(mat1);

			// Change values in mat2
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat2[i][j] *= -99;

			// Is mat1 untouched?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j), mat1[i][j]);

			// Are the values of mat2 correct?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j) * -99, mat2[i][j]);

			return;
		}

		// Tests if the values of a matrix constructed copied via the equals operator can be changed without modifying the object copied from
		TEST_METHOD(Copy_Is_Independent_EqualOperator)
		{
			Matrix4x4 mat1;

			// Fill with values
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat1[i][j] = i * 4.0 + j;

			// Copy
			Matrix4x4 mat2 = mat1;

			// Change values in mat2
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat2[i][j] *= -99;

			// Is mat1 untouched?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j), mat1[i][j]);

			// Are the values of mat2 correct?
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j) * -99, mat2[i][j]);

			return;
		}

		// Tests if the multiply-equals (*=) operator works as intended
		TEST_METHOD(Multiplication_Equals)
		{
			// Populate 1
			Matrix4x4 mat1;
			mat1.a = 12;
			mat1.b = 33;
			mat1.c = 43;
			mat1.e = 0;
			mat1.f = 4;
			mat1.g = 3;
			mat1.i = 76;
			mat1.j = 5;
			mat1.k = 42;

			mat1.d = 34;
			mat1.h = 11;
			mat1.l = 4;

			// Populate 2
			Matrix4x4 mat2;
			mat2.a = 32;
			mat2.b = 11;
			mat2.c = 23;
			mat2.e = 54;
			mat2.f = 23;
			mat2.g = 64;
			mat2.i = 64;
			mat2.j = 43;
			mat2.k = 12;

			mat2.d = 6;
			mat2.h = 9;
			mat2.l = 16;

			// Multiply
			mat1 *= mat2;

			// Check
			// Rotation, Scale, Skew
			Assert::AreEqual(4918.0, mat1.a);
			Assert::AreEqual(2740.0, mat1.b);
			Assert::AreEqual(2904.0, mat1.c);
			Assert::AreEqual( 408.0, mat1.e);
			Assert::AreEqual( 221.0, mat1.f);
			Assert::AreEqual( 292.0, mat1.g);
			Assert::AreEqual(5390.0, mat1.i);
			Assert::AreEqual(2757.0, mat1.j);
			Assert::AreEqual(2572.0, mat1.k);

			// Position
			Assert::AreEqual(40.0, mat1.d);
			Assert::AreEqual(20.0, mat1.h);
			Assert::AreEqual(20.0, mat1.l);

			// Always zero
			Assert::AreEqual(0.0, mat1.m);
			Assert::AreEqual(0.0, mat1.n);
			Assert::AreEqual(0.0, mat1.o);
			Assert::AreEqual(0.0, mat1.p);
			
			return;
		}

		// Tests if the multiplication operator works as intended
		TEST_METHOD(Multiplication)
		{
			// Populate 1
			Matrix4x4 mat1;
			mat1.a = 12;
			mat1.b = 65;
			mat1.c = 34;
			mat1.e = 43;
			mat1.f = 57;
			mat1.g = 76;
			mat1.i = 23;
			mat1.j = 34;
			mat1.k = 54;

			mat1.d = 99;
			mat1.h = 50;
			mat1.l = 0;

			// Populate 2
			Matrix4x4 mat2;
			mat2.a = 76;
			mat2.b = 74;
			mat2.c = 54;
			mat2.e = 34;
			mat2.f = 45;
			mat2.g = 45;
			mat2.i = 32;
			mat2.j = 76;
			mat2.k = 65;

			mat2.d = 1;
			mat2.h = 150;
			mat2.l = 2;

			// Multiply
			Matrix4x4 mat3 = mat1 * mat2;

			// Check
			// Rotation, Scale, Skew
			Assert::AreEqual(4210.0,  mat3.a);
			Assert::AreEqual(6397.0,  mat3.b);
			Assert::AreEqual(5783.0,  mat3.c);
			Assert::AreEqual(7638.0,  mat3.e);
			Assert::AreEqual(11523.0, mat3.f);
			Assert::AreEqual(9827.0,  mat3.g);
			Assert::AreEqual(4632.0,  mat3.i);
			Assert::AreEqual(7336.0,  mat3.j);
			Assert::AreEqual(6282.0,  mat3.k);

			// Position
			Assert::AreEqual(100.0, mat3.d);
			Assert::AreEqual(200.0, mat3.h);
			Assert::AreEqual(  2.0, mat3.l);

			// Always zero
			Assert::AreEqual(0.0, mat3.m);
			Assert::AreEqual(0.0, mat3.n);
			Assert::AreEqual(0.0, mat3.o);
			Assert::AreEqual(0.0, mat3.p);
		}

		// Tests if GetTranslationComponent returns the correct values
		TEST_METHOD(GetTranslationComponent)
		{
			// Create and populate mat
			Matrix4x4 mat;
			mat.d = 69;
			mat.h = 32;
			mat.l = 16;

			// Get translation components
			Vector3d translation = mat.GetTranslationComponent();

			// Check
			Assert::AreEqual(69.0, translation.x);
			Assert::AreEqual(32.0, translation.y);
			Assert::AreEqual(16.0, translation.z);

			return;
		}
	};
}
