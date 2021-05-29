#include "CppUnitTest.h"
#include "../Eule/Matrix4x4.h"
#include "../Eule/Vector3.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Matrices
{
	TEST_CLASS(_Matrix4x4)
	{
	private:
		std::mt19937 rng;
	public:
		// Constructor
		_Matrix4x4()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

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

		// Tests that copying via operator= works
		TEST_METHOD(Copy_Operator)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { 1, 0, 0, 5 };
			a[1] = { 2, 0, 0, 6 };
			a[2] = { 3, 0, 0, 7 };
			a[3] = { 4, 0, 0, 8 };

			Matrix4x4 a_toCopy;
			a_toCopy[0] = { 1, 0, 0, 5 };
			a_toCopy[1] = { 2, 0, 0, 6 };
			a_toCopy[2] = { 3, 0, 0, 7 };
			a_toCopy[3] = { 4, 0, 0, 8 };

			// Exercise
			Matrix4x4 b = a_toCopy;

			// Verify
			Assert::IsTrue(a == a_toCopy, L"a got destroyed!");
			Assert::IsTrue(b == a, L"a does not match b!");

			return;
		}

		// Tests that moving via operator= works
		TEST_METHOD(Move_Operator)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { 1, 0, 0, 5 };
			a[1] = { 2, 0, 0, 6 };
			a[2] = { 3, 0, 0, 7 };
			a[3] = { 4, 0, 0, 8 };

			Matrix4x4 a_backup = a;

			// Exercise
			Matrix4x4 b = std::move(a);

			// Verify
			Assert::IsTrue(b == a_backup, L"Values don't match!");

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

			// Get translation component
			Vector3d translation = mat.GetTranslationComponent();

			// Check
			Assert::AreEqual(69.0, translation.x);
			Assert::AreEqual(32.0, translation.y);
			Assert::AreEqual(16.0, translation.z);

			return;
		}

		// Tests if SetTranslationComponent returns the correct values
		TEST_METHOD(SetTranslationComponent)
		{
			// Create and populate mat
			Vector3d translation(69, 32, 16);

			// Set translation component
			Matrix4x4 mat;
			mat.SetTranslationComponent(translation);

			// Check
			Assert::AreEqual(69.0, mat.d);
			Assert::AreEqual(32.0, mat.h);
			Assert::AreEqual(16.0, mat.l);

			return;
		}

		// Tests that transpose3x3 works
		TEST_METHOD(Transpose3x3)
		{
			Matrix4x4 m;
			m[0] = { 0, 0, 0, 0 };
			m[1] = { 3, 0, 4, 0 };
			m[2] = { 0, 0, 2, 5 };
			m[3] = { 9, 0, 6, 0 };

			Matrix4x4 target;
			target[0] = { 0, 3, 0, 0 };
			target[1] = { 0, 0, 0, 0 };
			target[2] = { 0, 4, 2, 5 };
			target[3] = { 9, 0, 6, 0 };

			// Create debug output
			std::wstringstream wss;
			wss << std::endl
				<< "Actual: " << m.Transpose3x3() << std::endl
				<< "Target: " << target << std::endl;

			Assert::IsTrue(target == m.Transpose3x3(), wss.str().c_str());

			return;
		}

		// Tests that transpose4x4 works
		TEST_METHOD(Transpose4x4)
		{
			Matrix4x4 m;
			m[0] = { 0, 0, 0, 0 };
			m[1] = { 3, 0, 4, 0 };
			m[2] = { 0, 0, 2, 5 };
			m[3] = { 9, 0, 6, 0 };

			Matrix4x4 target;
			target[0] = { 0, 3, 0, 9 };
			target[1] = { 0, 0, 0, 0 };
			target[2] = { 0, 4, 2, 6 };
			target[3] = { 0, 0, 5, 0 };

			// Create debug output
			std::wstringstream wss;
			wss << std::endl
				<< "Actual: " << m.Transpose4x4() << std::endl
				<< "Target: " << target << std::endl;

			Assert::IsTrue(target == m.Transpose4x4(), wss.str().c_str());

			return;
		}

		// Tests that IsInvertible3x3 works -> true
		TEST_METHOD(Is_Invertible_3x3_True)
		{
			Matrix4x4 m;
			m[0] = { 0.56601, -0.87207, 0.52783, 488.00000 };
			m[1] = { -0.55281, 0.41590, 0.85470, 500.00000 };
			m[2] = { -1.09497, -0.66076, -0.15866, -155.09390 };
			m[3] = { 0.00000, 0.00000, 0.00000, 0.00000 };

			Assert::IsTrue(m.IsInversible3x3());

			return;
		}

		// Tests that IsInvertible3x3 works -> false
		TEST_METHOD(Is_Invertible_3x3_False)
		{
			Matrix4x4 m;
			m[0] = { 0, 0, 1, 0 };
			m[1] = { 0, 0, 0, 0 };
			m[2] = { 0, 0, 0, 0 };
			m[3] = { 0, 0, 0, 0 };

			Assert::IsFalse(m.IsInversible3x3());

			return;
		}

		// Tests that IsInvertible4x4 works -> true
		TEST_METHOD(Is_Invertible_4x4_True)
		{
			Matrix4x4 m;
			m[0] = { 0.56601, -0.87207, 0.52783, 488.00000 };
			m[1] = { -0.55281, 0.41590, 0.85470, 500.00000 };
			m[2] = { -1.09497, -0.66076, -0.15866, -155.09390 };
			m[3] = { 0.00000, 0.00000, 0.00000, 1.00000 };

			Assert::IsTrue(m.IsInversible4x4());

			return;
		}

		// Tests that IsInvertible4x4 works -> false
		TEST_METHOD(Is_Invertible_4x4_False)
		{
			Matrix4x4 m;
			m[0] = { 0.56601, -0.87207, 0.52783, 488.00000 };
			m[1] = { -0.55281, 0.41590, 0.85470, 500.00000 };
			m[2] = { -1.09497, -0.66076, -0.15866, -155.09390 };
			m[3] = { 0.00000, 0.00000, 0.00000, 0.00000 };

			Assert::IsFalse(m.IsInversible4x4());

			return;
		}

		// Tests that inverting a 3x3 matrix (scale, rotation, translation) works
		TEST_METHOD(Inverse3x3)
		{
			// Invert 50 randomly generated matrices
			for (std::size_t i = 0; i < 50;)
			{
				Matrix4x4 m;
				m[0] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[1] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[2] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[3] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };

				if (m.IsInversible3x3())
				{
					Matrix4x4 inv_m = m.Inverse3x3();
					Matrix4x4 result = m * inv_m;
					
					// Create debug output
					std::wstringstream wss;
					wss << std::endl
						<< "i: " << i << std::endl
						<< "Actual: " << result << std::endl
						<< "Target: " << Matrix4x4() << std::endl;

					Assert::IsTrue(result.Similar(Matrix4x4()), wss.str().c_str()); // Default constructor is identity matrix
					i++;
				}
			}

			return;
		}

		// Tests that inverting a 4x4 matrix works
		TEST_METHOD(Inverse4x4)
		{
			// Invert 50 randomly generated matrices
			for (std::size_t i = 0; i < 50;)
			{
				Matrix4x4 m;
				m[0] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[1] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[2] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };
				m[3] = { LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE };

				if (m.IsInversible4x4())
				{
					Matrix4x4 inv_m = m.Inverse4x4();

					// Create debug output
					std::wstringstream wss;
					wss << std::endl
						<< "i: " << i << std::endl
						<< "Actual: " << m.Multiply4x4(inv_m) << std::endl
						<< "Target: " << Matrix4x4() << std::endl;

					Assert::IsTrue((m.Multiply4x4(inv_m)).Similar(Matrix4x4(), 0.0001), wss.str().c_str()); // Default constructor is identity matrix
					i++;
				}
			}

			return;
		}

		// Tests the Multiply4x4 method, which does an actual 4x4 multiplication
		TEST_METHOD(Multiply4x4)
		{
			Matrix4x4 a;
			a[0] = { 0, 1, 2, 3 };
			a[1] = { 4, 5, 6, 7 };
			a[2] = { 8, 9, 0, 1 };
			a[3] = { 2, 3, 4, 5 };

			Matrix4x4 b;
			b[0] = { 9, 8, 7, 6 };
			b[1] = { 5, 4, 3, 2 };
			b[2] = { 1, 0, 9, 8 };
			b[3] = { 7, 6, 5, 4 };

			Matrix4x4 e; // Expected
			e[0] = {  28,  22,  36, 30 };
			e[1] = { 116,  94, 132, 110 };
			e[2] = { 124, 106,  88, 70 };
			e[3] = {  72,  58,  84, 70 };

			// Create debug output
			std::wstringstream wss;
			wss << std::endl
				<< "Actual: " << a.Multiply4x4(b) << std::endl
				<< "Target: " << e << std::endl;
			
			Assert::IsTrue(a.Multiply4x4(b).Similar(e), wss.str().c_str());
		}

		//! Tests that adding two matrices works as intended
		TEST_METHOD(Operator_Add)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = {  1,  2, 5, 0 };
			a[2] = {  2, -2, 7, 5 };
			a[3] = {  3,  0, 3, 0 };

			Matrix4x4 b;
			b[0] = { 6, 0, 5, 0 };
			b[1] = { 3, 0, 1, 1 };
			b[2] = { 1, 7, 2, 7 };
			b[3] = { 0, 2, 0, 0 };

			Matrix4x4 exp; // Expected
			exp[0] = { -3, 5, 11, 7 };
			exp[1] = {  4, 2, 6, 1 };
			exp[2] = {  3, 5, 9, 12 };
			exp[3] = {  3, 2, 3, 0 };

			// Exercise
			Matrix4x4 result = a + b;

			// Verify
			Assert::IsTrue(exp == result);

			return;
		}

		//! Tests that adding two matrices works as intended
		TEST_METHOD(Operator_AddEquals)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = { 1,  2, 5, 0 };
			a[2] = { 2, -2, 7, 5 };
			a[3] = { 3,  0, 3, 0 };

			Matrix4x4 b;
			b[0] = { 6, 0, 5, 0 };
			b[1] = { 3, 0, 1, 1 };
			b[2] = { 1, 7, 2, 7 };
			b[3] = { 0, 2, 0, 0 };

			Matrix4x4 exp; // Expected
			exp[0] = { -3, 5, 11, 7 };
			exp[1] = { 4, 2, 6, 1 };
			exp[2] = { 3, 5, 9, 12 };
			exp[3] = { 3, 2, 3, 0 };

			// Exercise
			a += b;

			// Verify
			Assert::IsTrue(exp == a);

			return;
		}

		//! Tests that subtracting two matrices works as intended
		TEST_METHOD(Operator_Sub)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = { 1,  2, 5, 0 };
			a[2] = { 2, -2, 7, 5 };
			a[3] = { 3,  0, 3, 0 };

			Matrix4x4 b;
			b[0] = { -6, -0, -5, -0 };
			b[1] = { -3, -0, -1, -1 };
			b[2] = { -1, -7, -2, -7 };
			b[3] = { -0, -2, -0, -0 };

			Matrix4x4 exp; // Expected
			exp[0] = { -3, 5, 11, 7 };
			exp[1] = { 4, 2, 6, 1 };
			exp[2] = { 3, 5, 9, 12 };
			exp[3] = { 3, 2, 3, 0 };

			// Exercise
			Matrix4x4 result = a - b;

			// Verify
			Assert::IsTrue(exp == result);

			return;
		}

		//! Tests that subtracting two matrices works as intended
		TEST_METHOD(Operator_SubEuqals)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = { 1,  2, 5, 0 };
			a[2] = { 2, -2, 7, 5 };
			a[3] = { 3,  0, 3, 0 };

			Matrix4x4 b;
			b[0] = { -6, -0, -5, -0 };
			b[1] = { -3, -0, -1, -1 };
			b[2] = { -1, -7, -2, -7 };
			b[3] = { -0, -2, -0, -0 };

			Matrix4x4 exp; // Expected
			exp[0] = { -3, 5, 11, 7 };
			exp[1] = { 4, 2, 6, 1 };
			exp[2] = { 3, 5, 9, 12 };
			exp[3] = { 3, 2, 3, 0 };

			// Exercise
			a -= b;

			// Verify
			Assert::IsTrue(exp == a);

			return;
		}

		// Tests that the multiplication operator for a double parameter works
		TEST_METHOD(Operator_MultiplyDouble)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = {  1, 2, 5, 0 };
			a[2] = {  2,-2, 7, 5 };
			a[3] = {  3, 0, 3, 0 };

			double s = LARGE_RAND_DOUBLE;

			Matrix4x4 exp; // Expected
			exp[0] = { -9*s, 5*s, 6*s, 7*s };
			exp[1] = {  1*s, 2*s, 5*s, 0*s };
			exp[2] = {  2*s,-2*s, 7*s, 5*s };
			exp[3] = {  3*s, 0*s, 3*s, 0*s };

			// Exercise
			Matrix4x4 result = a * s;

			// Verify
			Assert::IsTrue(exp.Similar(result));

			return;
		}

		// Tests that the multiplication operator for a double parameter works
		TEST_METHOD(Operator_MultiplyEqualsDouble)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = {  1, 2, 5, 0 };
			a[2] = {  2,-2, 7, 5 };
			a[3] = {  3, 0, 3, 0 };

			double s = LARGE_RAND_DOUBLE;

			Matrix4x4 exp; // Expected
			exp[0] = { -9*s, 5*s, 6*s, 7*s };
			exp[1] = {  1*s, 2*s, 5*s, 0*s };
			exp[2] = {  2*s,-2*s, 7*s, 5*s };
			exp[3] = {  3*s, 0*s, 3*s, 0*s };

			// Exercise
			a *= s;

			// Verify
			Assert::IsTrue(exp.Similar(a));

			return;
		}

		// Tests that the division operator for a double parameter works
		TEST_METHOD(Operator_DivideDouble)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = {  1, 2, 5, 0 };
			a[2] = {  2,-2, 7, 5 };
			a[3] = {  3, 0, 3, 0 };

			double s = LARGE_RAND_DOUBLE;

			Matrix4x4 exp; // Expected
			exp[0] = { -9/s, 5/s, 6/s, 7/s };
			exp[1] = {  1/s, 2/s, 5/s, 0/s };
			exp[2] = {  2/s,-2/s, 7/s, 5/s };
			exp[3] = {  3/s, 0/s, 3/s, 0/s };

			// Exercise
			Matrix4x4 result = a / s;

			// Verify
			Assert::IsTrue(exp.Similar(result));

			return;
		}

		// Tests that the division operator for a double parameter works
		TEST_METHOD(Operator_DivideEqualsDouble)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { -9, 5, 6, 7 };
			a[1] = {  1, 2, 5, 0 };
			a[2] = {  2,-2, 7, 5 };
			a[3] = {  3, 0, 3, 0 };

			double s = LARGE_RAND_DOUBLE;

			Matrix4x4 exp; // Expected
			exp[0] = { -9/s, 5/s, 6/s, 7/s };
			exp[1] = {  1/s, 2/s, 5/s, 0/s };
			exp[2] = {  2/s,-2/s, 7/s, 5/s };
			exp[3] = {  3/s, 0/s, 3/s, 0/s };

			// Exercise
			a /= s;

			// Verify
			Assert::IsTrue(exp.Similar(a));

			return;
		}

		// Tests that matrix division (multiplication with inverse) works
		TEST_METHOD(Operator_DivideMatrix)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { 0.0503814, 0.3314391, 0.9421304, 33 };
			a[1] = { 0.4941404, 0.8115034, -0.3119095, 44 };
			a[2] = { -0.8679211, 0.4812591, -0.1228928 , 55 };
			a[3] = { 0, 0, 0, 1 };

			Matrix4x4 b;
			b[0] = { -0.3980391, -0.5301175, -0.7486925, 3 };
			b[1] = { 0.3352839, 0.6756021, -0.6566175, 4 };
			b[2] = { 0.8539026, -0.5123839, -0.0911762  , 5 };
			b[3] = { 0, 0, 0, 1 };

			Matrix4x4 expected = a * b.Inverse3x3();
			// Just to be sure, but should already be set
			expected.SetTranslationComponent(Vector3d(30, 40, 50));

			// Exercise
			Matrix4x4 actual = a / b;

			// Verify
			Assert::IsTrue(expected.Similar(actual));

			return;
		}

		// Tests that matrix division (multiplication with inverse) works
		TEST_METHOD(Operator_DivideEqualsMatrix)
		{
			// Setup
			Matrix4x4 a;
			a[0] = { 0.0503814, 0.3314391, 0.9421304, 33 };
			a[1] = { 0.4941404, 0.8115034, -0.3119095, 44 };
			a[2] = { -0.8679211, 0.4812591, -0.1228928 , 55 };
			a[3] = { 0, 0, 0, 1 };

			Matrix4x4 b;
			b[0] = { -0.3980391, -0.5301175, -0.7486925, 3 };
			b[1] = { 0.3352839, 0.6756021, -0.6566175, 4 };
			b[2] = { 0.8539026, -0.5123839, -0.0911762  , 5 };
			b[3] = { 0, 0, 0, 1 };

			Matrix4x4 expected = a * b.Inverse3x3();
			// Just to be sure, but should already be set
			expected.SetTranslationComponent(Vector3d(30, 40, 50));

			// Exercise
			a /= b;

			// Verify
			Assert::IsTrue(expected.Similar(a));

			return;
		}

		// Tests that Math::Similar() works -> true
		TEST_METHOD(Similar_True)
		{
			Matrix4x4 a;
			a[0] = { 1, 0, 0, 0 };
			a[1] = { 0, 1, 0, 0 };
			a[2] = { 0, 0, 1, 0 };
			a[3] = { 0, 0, 0, 1 };

			Matrix4x4 b;
			b[0] = { 1, -9e-20, 2e-8, 9e-19 };
			b[1] = { 12e-19, 1, -20e-15, -6.9e-29 };
			b[2] = { -69e-25, 13e-23, 1, 4.301e-15 };
			b[3] = { -23e-19, 23e-19, 25e-7, 1 };

			Assert::IsTrue(a.Similar(b));
		}

		// Tests that Math::Similar() works -> false
		TEST_METHOD(Similar_False)
		{
			Matrix4x4 a;
			a[0] = { 1, 0, 0, 0 };
			a[1] = { 0, 1, 0, 0 };
			a[2] = { 0, 0, 1, 0 };
			a[3] = { 0, 0, 0, 1 };

			Matrix4x4 b;
			b[0] = { 1, -9e-20, 2e-8, 9e-19 };
			b[1] = { 12e-19, 1, -20e-15, 0.05 }; // <--
			b[2] = { -69e-25, 13e-23, 1, 4.301e-15 };
			b[3] = { -23e-19, 23e-19, 25e-7, 1 };

			Assert::IsFalse(a.Similar(b));
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
