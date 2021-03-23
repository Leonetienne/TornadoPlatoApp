#include "CppUnitTest.h"
#include "../Tornado/Vector4.h"
#include "../Tornado/Matrix4x4.h"
#include <random>
#include <sstream>
#include "Similar.h"
#include "HandyMacros.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vectors
{
	TEST_CLASS(_Vector4)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Vector4()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if all values are 0 after initialization via default constructor
		TEST_METHOD(New_Vector_All_0)
		{
			Vector4d v4;

			Assert::AreEqual(0.0, v4.x);
			Assert::AreEqual(0.0, v4.y);
			Assert::AreEqual(0.0, v4.z);
			Assert::AreEqual(0.0, v4.w);

			return;
		}

		// Tests if values can be set via the constructor
		TEST_METHOD(Can_Set_Values_Constructor)
		{
			Vector4d v4(69, 32, 16, 10);

			Assert::AreEqual(69.0, v4.x);
			Assert::AreEqual(32.0, v4.y);
			Assert::AreEqual(16.0, v4.z);
			Assert::AreEqual(10.0, v4.w);

			return;
		}

		// Tests if values can be set via letters
		TEST_METHOD(Can_Set_Values_Letters)
		{
			Vector4d v4;
			v4.x = 69;
			v4.y = 32;
			v4.z = 16;
			v4.w = 10;

			Assert::AreEqual(69.0, v4.x);
			Assert::AreEqual(32.0, v4.y);
			Assert::AreEqual(16.0, v4.z);
			Assert::AreEqual(10.0, v4.w);

			return;
		}

		// Tests if values can be set via array descriptors
		TEST_METHOD(Can_Set_Values_ArrayDescriptor)
		{
			Vector4d v4;
			v4[0] = 69;
			v4[1] = 32;
			v4[2] = 16;
			v4[3] = 10;

			Assert::AreEqual(69.0, v4.x);
			Assert::AreEqual(32.0, v4.y);
			Assert::AreEqual(16.0, v4.z);
			Assert::AreEqual(10.0, v4.w);

			return;
		}

		// Tests if values can be set via an initializer list
		TEST_METHOD(Can_Set_Values_InitializerList)
		{
			Vector4d v4 = { 69, 32, 16, 10 };

			Assert::AreEqual(69.0, v4.x);
			Assert::AreEqual(32.0, v4.y);
			Assert::AreEqual(16.0, v4.z);
			Assert::AreEqual(10.0, v4.w);

			return;
		}

		// Tests for vectors copied via the copy constructor to have the same values
		TEST_METHOD(Copy_Constructor_Same_Values)
		{
			Vector4d a(69, 32, 16, 10);
			Vector4d b(a);

			Assert::AreEqual(69.0, b.x);
			Assert::AreEqual(32.0, b.y);
			Assert::AreEqual(16.0, b.z);
			Assert::AreEqual(10.0, b.w);

			return;
		}

		// Tests for vectors copied via the equals operator to have the same values
		TEST_METHOD(Operator_Equals_Same_Values)
		{
			Vector4d a(69, 32, 16, 10);
			Vector4d b = a;

			Assert::AreEqual(69.0, b.x);
			Assert::AreEqual(32.0, b.y);
			Assert::AreEqual(16.0, b.z);
			Assert::AreEqual(10.0, b.w);

			return;
		}

		// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
		TEST_METHOD(Copy_Constructor_Independent)
		{
			Vector4d a(69, 32, 16, 10);
			Vector4d b(a);

			b.x = 169;
			b.y = 132;
			b.z = 116;
			b.w = 110;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);
			Assert::AreEqual(16.0, a.z);
			Assert::AreEqual(10.0, a.w);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);
			Assert::AreEqual(116.0, b.z);
			Assert::AreEqual(110.0, b.w);

			return;
		}

		// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
		TEST_METHOD(Operator_Equals_Independent)
		{
			Vector4d a(69, 32, 16, 10);
			Vector4d b = a;

			b.x = 169;
			b.y = 132;
			b.z = 116;
			b.w = 110;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);
			Assert::AreEqual(16.0, a.z);
			Assert::AreEqual(10.0, a.w);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);
			Assert::AreEqual(116.0, b.z);
			Assert::AreEqual(110.0, b.w);

			return;
		}

		// Tests the SqrMagnitude method to work as expected with random numbers
		TEST_METHOD(SqrMagnitude)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				double expected = x*x + y*y + z*z + w*w;

				Assert::AreEqual(expected, Vector4d(x, y, z, w).SqrMagnitude());
			}

			return;
		}

		// Tests for the length of the vector (0,0,0,0) being 0
		TEST_METHOD(Magnitude_Is_0_On_Vec0)
		{
			Assert::AreEqual(0.0, Vector4d(0, 0, 0, 0).Magnitude());
			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_X)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				Vector4d vec(x, 0, 0, 0);
				Assert::AreEqual(abs(x), vec.Magnitude());
			}

			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_Y)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double y = LARGE_RAND_DOUBLE;
				Vector4d vec(0, y, 0, 0);
				Assert::AreEqual(abs(y), vec.Magnitude());
			}

			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_Z)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double z = LARGE_RAND_DOUBLE;
				Vector4d vec(0, 0, z, 0);
				Assert::AreEqual(abs(z), vec.Magnitude());
			}

			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_W)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double w = LARGE_RAND_DOUBLE;
				Vector4d vec(0, 0, 0, w);
				Assert::AreEqual(abs(w), vec.Magnitude());
			}

			return;
		}

		// Tests for a known result
		TEST_METHOD(Magnitude)
		{
			// Ya'll got more of 'dem digits?
			Assert::AreEqual(78.5746530377322045524124405346810817718505859375, Vector4d(-23.76, 15.82, 66.75, 30.06).Magnitude());
			return;
		}

		// Tests for expected lerp result 0.00
		TEST_METHOD(Lerp_000)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = a.Lerp(b, 0.00);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(a == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.25
		TEST_METHOD(Lerp_025)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = a.Lerp(b, 0.25);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector4d(125, 1750, 32.5, -100) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.50
		TEST_METHOD(Lerp_050)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = a.Lerp(b, 0.50);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector4d(150, 2500, 55, 0) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.75
		TEST_METHOD(Lerp_075)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = a.Lerp(b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector4d(175, 3250, 77.5, 100) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 1.00
		TEST_METHOD(Lerp_100)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = a.Lerp(b, 1.00);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(b == res, wss.str().c_str());
			return;
		}

		// Tests for any normalized vector to be of length 1
		TEST_METHOD(Normalize_Length_Is_1)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				Vector4d vec(x, y, z, w);

				vec.Normalize();

				std::wstringstream wss;
				wss << vec;
				Assert::IsTrue(Similar(vec.Magnitude(), 1.0), wss.str().c_str()); // Account for floating point inaccuracy
			}

			return;
		}

		// Kinda dumb method, but ok lol
		// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
		TEST_METHOD(Normalized_Int_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				int x = LARGE_RAND_DOUBLE;
				int y = LARGE_RAND_DOUBLE;
				int z = LARGE_RAND_DOUBLE;
				int w = LARGE_RAND_DOUBLE;

				Vector4i vec(x, y, z, w);

				vec.Normalize();

				std::wstringstream wss;
				wss << vec;
				Assert::AreEqual(0.0, vec.Magnitude(), wss.str().c_str());
			}
		}

		// Tests for operator+ to work as expected
		TEST_METHOD(Operator_Add)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				Vector4d b(bx, by, bz, bw);

				Assert::IsTrue(Vector4d(ax + bx, ay + by, az + bz, aw + bw) == a + b);
			}

			return;
		}

		// Tests for operator+= to work as expected
		TEST_METHOD(Operator_Add_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				a += Vector4d(bx, by, bz, bw);

				Assert::IsTrue(Vector4d(ax + bx, ay + by, az + bz, aw + bw) == a);
			}

			return;
		}

		// Tests for operator- to work as expected
		TEST_METHOD(Operator_Sub)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				Vector4d b(bx, by, bz, bw);

				Assert::IsTrue(Vector4d(ax - bx, ay - by, az - bz, aw - bw) == a - b);
			}

			return;
		}

		// Tests for operator-= to work as expected
		TEST_METHOD(Operator_Sub_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				a -= Vector4d(bx, by, bz, bw);

				Assert::IsTrue(Vector4d(ax - bx, ay - by, az - bz, aw - bw) == a);
			}

			return;
		}

		// Tests for operator* to work as expected
		TEST_METHOD(Operator_Mult)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

				Vector4d a(x, y, z, w);

				Assert::IsTrue(Vector4d(x * scalar, y * scalar, z * scalar, w * scalar) == a * scalar);
			}

			return;
		}

		// Tests for operator*= to work as expected
		TEST_METHOD(Operator_Mult_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

				Vector4d a(x, y, z, w);
				a *= scalar;

				Assert::IsTrue(Vector4d(x * scalar, y * scalar, z * scalar, w * scalar) == a);
			}

			return;
		}

		// Tests for operator/ to work as expected
		TEST_METHOD(Operator_Div)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

				Vector4d a(x, y, z, w);

				Assert::IsTrue(Vector4d(x / scalar, y / scalar, z / scalar, w / scalar) == a / scalar);
			}

			return;
		}

		// Tests for operator/= to work as expected
		TEST_METHOD(Operator_Div_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				double w = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

				Vector4d a(x, y, z, w);
				a /= scalar;

				Assert::IsTrue(Vector4d(x / scalar, y / scalar, z / scalar, w / scalar) == a);
			}

			return;
		}

		// Tests for operator== to work as expected
		TEST_METHOD(Operator_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				Vector4d b(bx, by, bz, bw);

				Assert::IsTrue(
					((ax == bx) && (ay == by) && (az == bz) && (aw == bw)) ==
					(a == b)
				);
			}

			return;
		}

		// Tests for operator!= to work as expected
		TEST_METHOD(Operator_Not_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double az = LARGE_RAND_DOUBLE;
				double aw = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;
				double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				Vector4d b(bx, by, bz, bw);

				Assert::IsTrue(
					((ax != bx) || (ay != by) || (az != bz) || (aw != bw)) ==
					(a != b)
				);
			}

			return;
		}

		// Tests matrix multiplication with the multiplication operator (*) with a known result
		TEST_METHOD(MatrixMult)
		{
			Vector4d vec(117, 12, -36, 500);

			Matrix4x4 mat;
			mat[0] = { -43.7,   83,   96,     86 };
			mat[1] = {    12, 34.3,   43,    -47 };
			mat[2] = {    36,   67, 48.9,    -32 };
			mat[3] = {   -69,   47,   21,  89.01 };

			vec = vec * mat;

			Assert::IsTrue(Vector4d(35427.1, -23232.4, -12744.4, 36240) == vec);

			return;
		}

		// Tests matrix multiplication with the multiplication equals operator (*=) with a known result
		TEST_METHOD(MatrixMult_Equals)
		{
			Vector4d vec(117, 12, -36, 500);

			Matrix4x4 mat;
			mat[0] = { -43.7,   83,   96,     86 };
			mat[1] = {    12, 34.3,   43,    -47 };
			mat[2] = {    36,   67, 48.9,    -32 };
			mat[3] = {   -69,   47,   21,  89.01 };

			vec *= mat;

			Assert::IsTrue(Vector4d(35427.1, -23232.4, -12744.4, 36240) == vec);

			return;
		}

		// Tests matrix multiplication with the multiplication operator (*) with a known result, but with an int-vector
		TEST_METHOD(MatrixMult_Int)
		{
			Vector4i vec(112, -420, 80085, 1);

			Matrix4x4 mat;
			mat[0] = {  12,  83, 96,  86 };
			mat[1] = {  12, -57, 43, -47 };
			mat[2] = {  36,  67, 61, -32 };
			mat[3] = { -69,  47, 21,  99 };

			vec = vec * mat;

			Assert::IsTrue(Vector4i(7654730, 3468892, 4861045, 1654416) == vec);

			return;
		}

		// Tests matrix multiplication with the multiplication equals operator (*=) with a known result, but with an int - vector
		TEST_METHOD(MatrixMult_Equals_Int)
		{
			Vector4i vec(112, -420, 80085, 1);

			Matrix4x4 mat;
			mat[0] = {  12,  83, 96,  86 };
			mat[1] = {  12, -57, 43, -47 };
			mat[2] = {  36,  67, 61, -32 };
			mat[3] = { -69,  47, 21,  99 };

			vec *= mat;

			Assert::IsTrue(Vector4i(7654730, 3468892, 4861045, 1654416) == vec);

			return;
		}
	};
}
