#include "CppUnitTest.h"
#include "../Tornado/Vector4.h"
#include "../Tornado/Matrix4x4.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

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

		// Tests lerp as a static function
		TEST_METHOD(Lerp_Static)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);
			Vector4d res = Vector4d::Lerp(a, b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector4d(175, 3250, 77.5, 100) == res, wss.str().c_str());
			return;
		}

		// Tests lerpself
		TEST_METHOD(LerpSelf)
		{
			Vector4d a(100, 1000, 10, -200);
			Vector4d b(200, 4000, 100, 200);

			a.LerpSelf(b, 0.75);

			std::wstringstream wss;
			wss << a;
			Assert::IsTrue(Vector4d(175, 3250, 77.5, 100) == a, wss.str().c_str());
			return;
		}

		// Tests if an input vector of length 0 is handled correctly by the normalize method
		TEST_METHOD(Normalize_Length_Before_Is_0)
		{
			Vector4d vec(0, 0, 0, 0);
			vec.NormalizeSelf();
			Assert::AreEqual(0.0, vec.Magnitude());
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

				// Prevent a vector of length 0 going in
				if (vec.SqrMagnitude() == 0)
					vec.x++;

				std::wstringstream wss;
				wss << vec;
				Assert::IsTrue(Similar(vec.Normalize().Magnitude(), 1.0), wss.str().c_str()); // Account for floating point inaccuracy
			}

			return;
		}

		// Tests the normalize method with known values
		TEST_METHOD(Normalize_Oracle)
		{
			// Setup
			Vector4d v(3.2, -5.3, 9.88, 69.420);

			// Exercise
			v.NormalizeSelf();

			// Verify
			Vector4d expected(0.0454594951, -0.07529228877, 0.14035619114, 0.98618692201);
			Assert::IsTrue(v.Similar(expected));
		}

		// Kinda dumb method, but ok lol
		// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
		TEST_METHOD(Normalized_Int_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				int x = LARGE_RAND_INT;
				int y = LARGE_RAND_INT;
				int z = LARGE_RAND_INT;
				int w = LARGE_RAND_INT;

				Vector4i vec(x, y, z, w);

				vec.NormalizeSelf();

				std::wstringstream wss;
				wss << vec;
				Assert::AreEqual(0.0, vec.Magnitude(), wss.str().c_str());
			}
		}

		// Tests that NormalizeSelf() results in the same as Normalize()
		TEST_METHOD(NormalizeSelf_IsSameAs_Normalize)
		{
			// Run test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				Vector4d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);

				Vector4d nVec = vec.Normalize();
				vec.NormalizeSelf();

				Assert::IsTrue(nVec == vec);
			}

			return;
		}

		// Tests for the VectorScale() method to work
		TEST_METHOD(VectorScale)
		{
			// Run test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				const double ax = LARGE_RAND_DOUBLE;
				const double ay = LARGE_RAND_DOUBLE;
				const double az = LARGE_RAND_DOUBLE;
				const double aw = LARGE_RAND_DOUBLE;
				const double bx = LARGE_RAND_DOUBLE;
				const double by = LARGE_RAND_DOUBLE;
				const double bz = LARGE_RAND_DOUBLE;
				const double bw = LARGE_RAND_DOUBLE;

				Vector4d a(ax, ay, az, aw);
				Vector4d b(bx, by, bz, bw);

				Vector4d target(
					ax * bx,
					ay * by,
					az * bz,
					aw * bw
				);

				Assert::IsTrue(a.VectorScale(b) == target);
			}

			return;
		}

		// Tests for operator- (unary) to work
		TEST_METHOD(Operator_Unary_Negative)
		{
			Vector4d v(29, -5, 35, -69);

			Assert::IsTrue(Vector4d(-29, 5, -35, 69) == -v);

			return;
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

		// Tests loose comparison via Vector4d::Similar -> true
		TEST_METHOD(Loose_Comparison_True_Vector4d)
		{
			Assert::IsTrue(
				Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333).Similar(
					Vector4d(0, -6.666666667, 10, -3.33333333333333)
			));
			return;
		}

		// Tests loose comparison via Vector4d::Similar -> false
		TEST_METHOD(Loose_Comparison_False_Vector4d)
		{
			Assert::IsFalse(
				Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333).Similar(
					Vector4d(0.1, -6.7, 10.1, -3.333)
			));
			return;
		}
	};
}
