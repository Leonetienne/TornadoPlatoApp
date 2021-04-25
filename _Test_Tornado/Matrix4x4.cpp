#include "CppUnitTest.h"
#include "../Tornado/Matrix4x4.h"
#include "../Tornado/Vector3.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrices
{
	TEST_CLASS(_Matrix4x4)
	{
	public:

		// Tests that a freshly created matrix is an identity matrix
		TEST_METHOD(New_Matrix_Is_Identity)
		{
			Matrix4x4 mat;

			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					if (i == j)
						Assert::AreEqual(1.0, mat[i][j]);
					else
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
					Assert::AreEqual((double)(i * 4 + j + 1), mat[i][j]);

			return;
		}

		// Tests if setting values via multiple initializer lists works
		TEST_METHOD(Can_Set_Values_Multiple_Initializer_Lists)
		{
			Matrix4x4 mat;
			mat[0] = {  1,  2,  3,  4 };
			mat[1] = {  5,  6,  7,  8 };
			mat[2] = {  9, 10, 11, 12 };
			mat[3] = { 13, 14, 15, 16 };

			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					Assert::AreEqual((double)(i * 4 + j + 1), mat[i][j]);

			return;
		}

		// Tests if values can be read correctly from the reference variables
		TEST_METHOD(Can_Read_Letters)
		{
			Matrix4x4 mat;

			// Populate matrix
			for (std::size_t i = 0; i < 4; i++)
				for (std::size_t j = 0; j < 4; j++)
					mat[i][j] = (double)(i * 4 + j + 1);

			// Check if values can be read
			Assert::AreEqual( 1.0, mat.a);
			Assert::AreEqual( 2.0, mat.b);
			Assert::AreEqual( 3.0, mat.c);
			Assert::AreEqual( 4.0, mat.d);
			Assert::AreEqual( 5.0, mat.e);
			Assert::AreEqual( 6.0, mat.f);
			Assert::AreEqual( 7.0, mat.g);
			Assert::AreEqual( 8.0, mat.h);
			Assert::AreEqual( 9.0, mat.i);
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
			mat1[0] = { 12, 33, 43, 34 };
			mat1[1] = {  0,  4,  3, 11 };
			mat1[2] = { 76,  5, 42,  4 };
			mat1[3] = {  0,  0,  0,  1 };

			// Populate 2
			Matrix4x4 mat2;
			mat2[0] = { 32, 11, 23,  6 };
			mat2[1] = { 54, 23, 64,  9 };
			mat2[2] = { 64, 43, 12, 16 };
			mat2[3] = {  0,  0,  0,  1 };


			// Multiply
			mat1 *= mat2;

			// Check
			Matrix4x4 expected;
			expected[0] = { 4918.0, 2740.0, 2904.0, 40 };
			expected[1] = {  408.0,  221.0,  292.0, 20 };
			expected[2] = { 5390.0, 2757.0, 2572.0, 20 };
			expected[3] = {      0,      0,      0,  1 };

			Assert::IsTrue(mat1.v == expected.v);
			
			return;
		}

		// Tests if the multiplication operator works as intended
		TEST_METHOD(Multiplication)
		{
			// Populate 1
			Matrix4x4 mat1;
			mat1[0] = { 12, 33, 43, 34 };
			mat1[1] = { 0,  4,  3, 11 };
			mat1[2] = { 76,  5, 42,  4 };
			mat1[3] = { 0,  0,  0,  1 };

			// Populate 2
			Matrix4x4 mat2;
			mat2[0] = { 32, 11, 23,  6 };
			mat2[1] = { 54, 23, 64,  9 };
			mat2[2] = { 64, 43, 12, 16 };
			mat2[3] = { 0,  0,  0,  1 };

			// Multiply
			Matrix4x4 mat3 = mat1 * mat2;

			// Check
			Matrix4x4 expected;
			expected[0] = { 4918.0, 2740.0, 2904.0, 40 };
			expected[1] = { 408.0,  221.0,  292.0, 20 };
			expected[2] = { 5390.0, 2757.0, 2572.0, 20 };
			expected[3] = { 0,      0,      0,  1 };

			Assert::IsTrue(mat3.v == expected.v);

			return;
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

		// Tests if the equal operator (==) and not-equals operator (!=) work (equal: false)
		TEST_METHOD(Operator_Equals_NotEquals_False)
		{
			Matrix4x4 a;
			a[0] = { 0x0, 0x1, 0x2, 0x3 };
			a[1] = { 0x4, 0x5, 0x6, 0x7 };
			a[2] = { 0x8, 0x9, 0xA, 0xB };
			a[3] = { 0xC, 0xD, 0xE, 0xF };

			Matrix4x4 b;
			b[3] = { 0xF ,0xD, 0xE, 0xC };
			b[2] = { 0xB ,0x9, 0xA, 0x8 };
			b[1] = { 0x7 ,0x5, 0x6, 0x4 };
			b[0] = { 0x3 ,0x1, 0x2, 0x0 };

			Assert::IsFalse(a == b);
			Assert::IsTrue(a != b);
			return;
		}

		// Tests if the equal operator (==) and not-equals operator (!=) work (equal: true)
		TEST_METHOD(Operator_Equals_False)
		{
			Matrix4x4 a;
			a[0] = { 0x0, 0x1, 0x2, 0x3 };
			a[1] = { 0x4, 0x5, 0x6, 0x7 };
			a[2] = { 0x8, 0x9, 0xA, 0xB };
			a[3] = { 0xC, 0xD, 0xE, 0xF };

			Matrix4x4 b;
			b[0] = { 0x0, 0x1, 0x2, 0x3 };
			b[1] = { 0x4, 0x5, 0x6, 0x7 };
			b[2] = { 0x8, 0x9, 0xA, 0xB };
			b[3] = { 0xC, 0xD, 0xE, 0xF };

			Assert::IsTrue(a == b);
			Assert::IsFalse(a != b);
			return;
		}
	};
}
