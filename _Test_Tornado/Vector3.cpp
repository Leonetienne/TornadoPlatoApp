#include "CppUnitTest.h"
#include "../Tornado/Vector3.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vectors
{
	TEST_CLASS(_Vector3)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Vector3()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if all values are 0 after initialization via default constructor
		TEST_METHOD(New_Vector_All_0)
		{
			Vector3d v3;

			Assert::AreEqual(0.0, v3.x);
			Assert::AreEqual(0.0, v3.y);
			Assert::AreEqual(0.0, v3.z);

			return;
		}

		// Tests if values can be set via the constructor
		TEST_METHOD(Can_Set_Values_Constructor)
		{
			Vector3d v3(69, 32, 16);

			Assert::AreEqual(69.0, v3.x);
			Assert::AreEqual(32.0, v3.y);
			Assert::AreEqual(16.0, v3.z);

			return;
		}

		// Tests if values can be set via letters
		TEST_METHOD(Can_Set_Values_Letters)
		{
			Vector3d v3;
			v3.x = 69;
			v3.y = 32;
			v3.z = 16;

			Assert::AreEqual(69.0, v3.x);
			Assert::AreEqual(32.0, v3.y);
			Assert::AreEqual(16.0, v3.z);

			return;
		}

		// Tests if values can be set via array descriptors
		TEST_METHOD(Can_Set_Values_ArrayDescriptor)
		{
			Vector3d v3;
			v3[0] = 69;
			v3[1] = 32;
			v3[2] = 16;

			Assert::AreEqual(69.0, v3.x);
			Assert::AreEqual(32.0, v3.y);
			Assert::AreEqual(16.0, v3.z);

			return;
		}

		// Tests if values can be set via an initializer list
		TEST_METHOD(Can_Set_Values_InitializerList)
		{
			Vector3d v3 = { 69, 32, 16 };

			Assert::AreEqual(69.0, v3.x);
			Assert::AreEqual(32.0, v3.y);
			Assert::AreEqual(16.0, v3.z);

			return;
		}

		// Tests for vectors copied via the copy constructor to have the same values
		TEST_METHOD(Copy_Constructor_Same_Values)
		{
			Vector3d a(69, 32, 16);
			Vector3d b(a);

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);
			Assert::AreEqual(a.z, b.z);

			return;
		}

		// Tests for vectors copied via the equals operator to have the same values
		TEST_METHOD(Operator_Equals_Same_Values)
		{
			Vector3d a(69, 32, 16);
			Vector3d b = a;

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);
			Assert::AreEqual(a.z, b.z);

			return;
		}

		// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
		TEST_METHOD(Copy_Constructor_Independent)
		{
			Vector3d a(69, 32, 16);
			Vector3d b(a);

			b.x = 169;
			b.y = 132;
			b.z = 116;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);
			Assert::AreEqual(16.0, a.z);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);
			Assert::AreEqual(116.0, b.z);

			return;
		}

		// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
		TEST_METHOD(Operator_Equals_Independent)
		{
			Vector3d a(69, 32, 16);
			Vector3d b = a;

			b.x = 169;
			b.y = 132;
			b.z = 116;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);
			Assert::AreEqual(16.0, a.z);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);
			Assert::AreEqual(116.0, b.z);

			return;
		}

		// Tests if the dot product between two vectors angled 90 degrees from one another is 0. It should by definition be 0!
		// Dot products are commutative, so we'll check both directions.
		// This test tests all possible 90 degree setups with 1000x random lengths
		TEST_METHOD(DotProduct_90deg)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 100; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector3d a = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
				Vector3d b = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
		
				std::wstringstream wss;
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

				wss.str(L"");
				a = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
				b = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

				wss.str(L"");
				a = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
				b = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

				wss.str(L"");
				a = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
				b = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

				wss.str(L"");
				a = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
				b = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

				wss.str(L"");
				a = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
				b = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
				Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());
			}
		
			return;
		}

		// Test if the dot product is positive for two vectors angled less than 90 degrees from another
		// Dot products are commutative, so we'll check both directions.
		TEST_METHOD(DotProduct_LessThan90deg)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector3d a = Vector3d(1, 1.0 / (rng() % 100), 69) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector3d b = Vector3d(1.0 / (rng() % 100), 1, 69) * (rng() % 6969 + 1.0);
		
		
				std::wstringstream wss;
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::IsTrue(a.DotProduct(b) > 0, wss.str().c_str());
				Assert::IsTrue(b.DotProduct(a) > 0, wss.str().c_str());
			}
		
			return;
		}

		// Test if the dot product is negative for two vectors angled greater than 90 degrees from another
		// Dot products are commutative, so we'll check both directions.
		TEST_METHOD(DotProduct_GreaterThan90deg)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector3d a = Vector3d(1, -1.0 / (rng() % 100), 69) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector3d b = Vector3d(-1.0 / (rng() % 100), 1, -69) * (rng() % 6969 + 1.0);
		
				std::wstringstream wss;
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::IsTrue(a.DotProduct(b) < 0, wss.str().c_str());
				Assert::IsTrue(b.DotProduct(a) < 0, wss.str().c_str());
			}
		
			return;
		}

		// Quick and dirty check if the useless int-method is working
		TEST_METHOD(DotProduct_Dirty_Int)
		{
			Vector3i a;
			Vector3i b;
			std::wstringstream wss;

			// 90 deg
			a = { 0, 10, 0 };
			b = { 10, 0, 0 };
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
			Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

			// < 90 deg
			a = { 7, 10, 10 };
			b = { 10, 1, 10 };
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::IsTrue(a.DotProduct(b) > 0.0, wss.str().c_str());
			Assert::IsTrue(b.DotProduct(a) > 0.0, wss.str().c_str());

			// > 90 deg
			a = { -3, 10, -10 };
			b = { 10, -4,  10 };
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::IsTrue(a.DotProduct(b) < 0.0, wss.str().c_str());
			Assert::IsTrue(b.DotProduct(a) < 0.0, wss.str().c_str());

			return;
		}

		// Tests for the cross product between the same vector being 0
		TEST_METHOD(CrossProduct_Same_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);

				std::wstringstream wss;
				wss << a << L" CROSS " << a << L" = " << a.CrossProduct(a) << std::endl;
				Assert::IsTrue(Vector3d(0,0,0) == a.CrossProduct(a), wss.str().c_str());
			}

			return;
		}

		// Tests for the cross product between opposite vectors being 0
		TEST_METHOD(CrossProduct_Opposite_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);
				Vector3d b(-x, -y, -z);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(Vector3d(0, 0, 0) == a.CrossProduct(b), wss.str().c_str());
			}

			return;
		}

		// Tests for known values
		TEST_METHOD(CrossProduct_KnownValues)
		{
			Vector3d a;
			Vector3d b;

			std::wstringstream wss;

			wss.str(L"");
			a = Vector3d(1, 0, 0);
			b = Vector3d(0, 0, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(-1, 0, 0);
			b = Vector3d(0, 0, -1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(1, 0, 0);
			b = Vector3d(0, 0, -1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, 1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(1, 0, 0);
			b = Vector3d(0, 1, 0);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, 0, 1) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(1, 0, 0);
			b = Vector3d(1, 0, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(1, 0, 0);
			b = Vector3d(1, 1, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 1) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3d(3, -1, -3);
			b = Vector3d(-1, 1, 3);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -6, 2) == a.CrossProduct(b), wss.str().c_str());

			return;
		}

		// Tests for known values, but with int vectors
		TEST_METHOD(CrossProduct_KnownValues_Int)
		{
			Vector3i a;
			Vector3i b;

			std::wstringstream wss;

			wss.str(L"");
			a = Vector3i(1, 0, 0);
			b = Vector3i(0, 0, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(-1, 0, 0);
			b = Vector3i(0, 0, -1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(1, 0, 0);
			b = Vector3i(0, 0, -1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, 1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(1, 0, 0);
			b = Vector3i(0, 1, 0);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, 0, 1) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(1, 0, 0);
			b = Vector3i(1, 0, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 0) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(1, 0, 0);
			b = Vector3i(1, 1, 1);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -1, 1) == a.CrossProduct(b), wss.str().c_str());

			wss.str(L"");
			a = Vector3i(3, -1, -3);
			b = Vector3i(-1, 1, 3);
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(Vector3d(0, -6, 2) == a.CrossProduct(b), wss.str().c_str());

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
				double expected = x*x + y*y + z*z;

				Assert::AreEqual(expected, Vector3d(x, y, z).SqrMagnitude());
			}

			return;
		}

		// Tests the SqrMagnitude method to work as expected with random numbers, but with an int-vector
		TEST_METHOD(SqrMagnitude_Int)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				int x = LARGE_RAND_INT;
				int y = LARGE_RAND_INT;
				int z = LARGE_RAND_INT;
				int expected = x*x + y*y + z*z;

				Assert::AreEqual((double)expected, Vector3i(x, y, z).SqrMagnitude());
			}

			return;
		}

		// Tests for the length of the vector (0,0,0) being 0
		TEST_METHOD(Magnitude_Is_0_On_Vec0)
		{
			Assert::AreEqual(0.0, Vector3d(0, 0, 0).Magnitude());
			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_X)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				Vector3d vec(x, 0, 0);
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
				Vector3d vec(0, y, 0);
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
				Vector3d vec(0, 0, z);
				Assert::AreEqual(abs(z), vec.Magnitude());
			}

			return;
		}

		// Tests for a known result
		TEST_METHOD(Magnitude)
		{
			// Ya'll got more of 'dem digits?
			Assert::AreEqual(426.14786166306174663986894302070140838623046875, Vector3d(69, -420, 21).Magnitude());
			return;
		}

		// Tests for expected lerp result 0.00
		TEST_METHOD(Lerp_000)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = a.Lerp(b, 0.00);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(a == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.25
		TEST_METHOD(Lerp_025)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = a.Lerp(b, 0.25);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector3d(125, 1750, 32.5) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.50
		TEST_METHOD(Lerp_050)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = a.Lerp(b, 0.50);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector3d(150, 2500, 55) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.75
		TEST_METHOD(Lerp_075)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = a.Lerp(b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector3d(175, 3250, 77.5) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 1.00
		TEST_METHOD(Lerp_100)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = a.Lerp(b, 1.00);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(b == res, wss.str().c_str());
			return;
		}

		// Tests lerp as a static function
		TEST_METHOD(Lerp_Static)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);
			Vector3d res = Vector3d::Lerp(a, b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector3d(175, 3250, 77.5) == res, wss.str().c_str());
			return;
		}

		// Tests lerpself
		TEST_METHOD(LerpSelf)
		{
			Vector3d a(100, 1000, 10);
			Vector3d b(200, 4000, 100);

			a.LerpSelf(b, 0.75);

			std::wstringstream wss;
			wss << a;
			Assert::IsTrue(Vector3d(175, 3250, 77.5) == a, wss.str().c_str());
			return;
		}

		// Tests if an input vector of length 0 is handled correctly by the normalize method
		TEST_METHOD(Normalize_Length_Before_Is_0)
		{
			Vector3d vec(0, 0, 0);
			vec.Normalize();
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

				Vector3d vec(x, y, z);

				// Prevent a vector of length 0 going in
				if (vec.SqrMagnitude() == 0)
					vec.x++;

				std::wstringstream wss;
				wss << vec;
				Assert::IsTrue(Similar(vec.Normalize().Magnitude(), 1.0), wss.str().c_str()); // Account for floating point inaccuracy
			}

			return;
		}

		// Tests for a normalized vector to still point in the exact same direction
		TEST_METHOD(Normalize_Direction_Stays_Unaffected)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double z = LARGE_RAND_DOUBLE;
				if (x == 0) x++;
				if (y == 0) y++;
				if (z == 0) z++;
		
				Vector3d vec(x, y, z);
		
				Vector3d vec_n(x, y, z);
				vec_n = vec_n.Normalize();
		
				std::wstringstream wss;
				wss << vec << L" | " << vec_n;
		
				// Both vectors should still point in the same direction!
				Assert::IsTrue(
					(vec.DotProduct(vec_n) > 0) && // Roughly same direction
					(Similar(vec_n.CrossProduct(vec).Magnitude(), 0.0)), // Both vectors align
					wss.str().c_str());
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
				int x = LARGE_RAND_INT;
				int y = LARGE_RAND_INT;
				int z = LARGE_RAND_INT;

				Vector3i vec(x, y, z);

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
				Vector3d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);

				Vector3d nVec = vec.Normalize();
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
				const double bx = LARGE_RAND_DOUBLE;
				const double by = LARGE_RAND_DOUBLE;
				const double bz = LARGE_RAND_DOUBLE;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Vector3d target(
					ax * bx,
					ay * by,
					az * bz
				);

				Assert::IsTrue(a.VectorScale(b) == target);
			}

			return;
		}

		// Tests for operator- (unary) to work
		TEST_METHOD(Operator_Unary_Negative)
		{
			Vector3d v(29, -5, 35);

			Assert::IsTrue(Vector3d(-29, 5, -35) == -v);

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
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Assert::IsTrue(Vector3d(ax + bx, ay + by, az + bz) == a + b);
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
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;

				Vector3d a(ax, ay, az);
				a += Vector3d(bx, by, bz);

				Assert::IsTrue(Vector3d(ax + bx, ay + by, az + bz) == a);
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
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Assert::IsTrue(Vector3d(ax - bx, ay - by, az - bz) == a - b);
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
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;
				double bz = LARGE_RAND_DOUBLE;

				Vector3d a(ax, ay, az);
				a -= Vector3d(bx, by, bz);

				Assert::IsTrue(Vector3d(ax - bx, ay - by, az - bz) == a);
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
				double scalar = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);

				Assert::IsTrue(Vector3d(x * scalar, y * scalar, z * scalar) == a * scalar);
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
				double scalar = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);
				a *= scalar;

				Assert::IsTrue(Vector3d(x * scalar, y * scalar, z * scalar) == a);
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
				double scalar = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);

				Assert::IsTrue(Vector3d(x / scalar, y / scalar, z / scalar) == a / scalar);
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
				double scalar = LARGE_RAND_DOUBLE;

				Vector3d a(x, y, z);
				a /= scalar;

				Assert::IsTrue(Vector3d(x / scalar, y / scalar, z / scalar) == a);
			}

			return;
		}

		// Tests for operator== to work as expected
		TEST_METHOD(Operator_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 10000; i++)
			{
				double ax = (rng() % 10) - 5;
				double ay = (rng() % 10) - 5;
				double az = (rng() % 10) - 5;
				double bx = (rng() % 10) - 5;
				double by = (rng() % 10) - 5;
				double bz = (rng() % 10) - 5;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Assert::IsTrue(
					((ax == bx) && (ay == by) && (az == bz)) ==
					(a == b)
				);
			}

			return;
		}

		// Tests for operator!= to work as expected
		TEST_METHOD(Operator_Not_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 10000; i++)
			{
				double ax = (rng() % 10) - 5;
				double ay = (rng() % 10) - 5;
				double az = (rng() % 10) - 5;
				double bx = (rng() % 10) - 5;
				double by = (rng() % 10) - 5;
				double bz = (rng() % 10) - 5;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Assert::IsTrue(
					((ax != bx) || (ay != by) || (az != bz)) ==
					(a != b)
				);
			}

			return;
		}

		// Tests for matrix multiplication working regarding rotation
		TEST_METHOD(MatrixMult_Rotate_Yaw)
		{
			// Create vector
			Vector3d vec(69, 32, 16);
			Vector3d originalVec = vec;

			// Create 90deg yaw rotation matrix (Y)
			Matrix4x4 mat;
			mat[0] = {  0, 0, 1, 0 };
			mat[1] = {  0, 1, 0, 0 };
			mat[2] = { -1, 0, 0, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << L"Rot #1 " << vec;
			Assert::IsTrue(Vector3d(16, 32, -69) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #2 " << vec;
			Assert::IsTrue(Vector3d(-69, 32, -16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #3 " << vec;
			Assert::IsTrue(Vector3d(-16, 32, 69) == vec, wss.str().c_str());

			// Rotate by 90 deg a last time, having completed a 360° rotation.
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #4 " << vec;
			Assert::IsTrue(originalVec == vec, wss.str().c_str());

			return;
		}

		// Tests for matrix multiplication working regarding rotation
		TEST_METHOD(MatrixMult_Rotate_Roll)
		{
			// Create vector
			Vector3d vec(69, 32, 16);
			Vector3d originalVec = vec;

			// Create 90deg roll rotation matrix (Z)
			Matrix4x4 mat;
			mat[0] = { 0, -1, 0, 0 };
			mat[1] = { 1,  0, 0, 0 };
			mat[2] = { 0,  0, 1, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << L"Rot #1 " << vec;
			Assert::IsTrue(Vector3d(-32, 69, 16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #2 " << vec;
			Assert::IsTrue(Vector3d(-69, -32, 16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #3 " << vec;
			Assert::IsTrue(Vector3d(32, -69, 16) == vec, wss.str().c_str());

			// Rotate by 90 deg a last time, having completed a 360° rotation.
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #4 " << vec;
			Assert::IsTrue(originalVec == vec, wss.str().c_str());

			return;
		}

		// Tests for matrix multiplication working regarding rotation
		TEST_METHOD(MatrixMult_Rotate_Pitch)
		{
			// Create vector
			Vector3d vec(69, 32, 16);
			Vector3d originalVec = vec;

			// Create 90deg pitch rotation matrix (X)
			Matrix4x4 mat;
			mat[0] = { 1, 0,  0, 0 };
			mat[1] = { 0, 0, -1, 0 };
			mat[2] = { 0, 1,  0, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << L"Rot #1 " << vec;
			Assert::IsTrue(Vector3d(69, -16, 32) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #2 " << vec;
			Assert::IsTrue(Vector3d(69, -32, -16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #3 " << vec;
			Assert::IsTrue(Vector3d(69, 16, -32) == vec, wss.str().c_str());

			// Rotate by 90 deg a last time, having completed a 360° rotation.
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #4 " << vec;
			Assert::IsTrue(originalVec == vec, wss.str().c_str());

			return;
		}

		// Tests if rotating a vector (1,1,1) by (45,45,45) eulers works
		TEST_METHOD(MatrixMult_Rotate_Unit_Combined)
		{
			// Create vector
			Vector3d vec(1, 1, 1);

			// Create rotation matrix
			Matrix4x4 mat;
			mat[0] = {     0.5, -0.1465,  0.8535, 0 };
			mat[1] = {     0.5,  0.8535, -0.1465, 0 };
			mat[2] = { -0.7072,     0.5,     0.5, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(
				Similar(vec.x, 1.207, 0.001) &&
				Similar(vec.y, 1.207, 0.001) &&
				Similar(vec.z, 0.2928, 0.001),
				wss.str().c_str());

			return;
		}

		// Tests if rotating a vector (69,32,16) by (45,45,45) eulers works
		TEST_METHOD(MatrixMult_Rotate_HalfUnit_Combined)
		{
			// Create vector
			Vector3d vec(69, 32, 16);

			// Create rotation matrix
			Matrix4x4 mat;
			mat[0] = {     0.5, -0.1465,  0.8535, 0 };
			mat[1] = {     0.5,  0.8535, -0.1465, 0 };
			mat[2] = { -0.7072,     0.5,     0.5, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(
				Similar(vec.x, 43.468, 0.001) &&
				Similar(vec.y, 59.468, 0.001) &&
				Similar(vec.z, -24.7968, 0.001),
				wss.str().c_str());

			return;
		}

		// Tests if rotating a vector (69,32,16) by (45,45,45) eulers works
		TEST_METHOD(MatrixMult_Rotate_Combined)
		{
			// Create vector
			Vector3d vec(69, 32, 16);

			// Create rotation matrix
			Matrix4x4 mat;
			mat[0] = { -0.1639, -0.9837, -0.0755, 0 };
			mat[1] = {  0.128,   -0.097,   0.987, 0 };
			mat[2] = { -0.9782,   0.152,  0.1417, 0 };

			// Rotate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(
				Similar(vec.x, -43.9955, 0.001) &&
				Similar(vec.y, 21.52, 0.001) &&
				Similar(vec.z, -60.3646, 0.001),
				wss.str().c_str());

			return;
		}

		// Tests if matrix scaling works ( x axis only )
		TEST_METHOD(MatrixMult_Scale_X)
		{
			// Create vector
			Vector3d vec(5, 6, 7);

			// Create scaling matrix
			Matrix4x4 mat;
			mat[0] = { 3, 0, 0, 0 };
			mat[1] = { 0, 1, 0, 0 };
			mat[2] = { 0, 0, 1, 0 };

			// Scale vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3d(15, 6, 7) == vec, wss.str().c_str());

			return;
		}

		// Tests if matrix scaling works ( y axis only )
		TEST_METHOD(MatrixMult_Scale_Y)
		{
			// Create vector
			Vector3d vec(5, 6, 7);

			// Create scaling matrix
			Matrix4x4 mat;
			mat[0] = { 1, 0, 0, 0 };
			mat[1] = { 0, 3, 0, 0 };
			mat[2] = { 0, 0, 1, 0 };

			// Scale vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3d(5, 18, 7) == vec, wss.str().c_str());

			return;
		}

		// Tests if matrix scaling works ( z axis only )
		TEST_METHOD(MatrixMult_Scale_Z)
		{
			// Create vector
			Vector3d vec(5, 6, 7);

			// Create scaling matrix
			Matrix4x4 mat;
			mat[0] = { 1, 0, 0, 0 };
			mat[1] = { 0, 1, 0, 0 };
			mat[2] = { 0, 0, 3, 0 };

			// Scale vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3d(5, 6, 21) == vec, wss.str().c_str());

			return;
		}

		// Tests if matrix scaling works ( all axes )
		TEST_METHOD(MatrixMult_Scale_Combined)
		{
			// Create vector
			Vector3d vec(5, 6, 7);

			// Create scaling matrix
			Matrix4x4 mat;
			mat[0] = { 4, 0, 0, 0 };
			mat[1] = { 0, 5, 0, 0 };
			mat[2] = { 0, 0, 8, 0 };

			// Scale vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3d(20, 30, 56) == vec, wss.str().c_str());

			return;
		}

		// Tests if translation via matrix multiplication works
		TEST_METHOD(MatrixMult_Translation)
		{
			// Create vector
			Vector3d vec(5, 6, 7);

			// Create scaling matrix
			Matrix4x4 mat;
			mat[0] = { 1, 0, 0, 155 };
			mat[1] = { 0, 1, 0, -23 };
			mat[2] = { 0, 0, 1, 333 };

			// Translate vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3d(160, -17, 340) == vec, wss.str().c_str());

			return;
		}

		// Tests the multiplication operator (*) with a simple matrix. All other tests used the * operator (without the '=')
		TEST_METHOD(MatrixMult_Not_Using_MultEqualsOperator)
		{
			// Create vector
			Vector3d vec(5.1, 6.4, 7.99);

			// Create scaling and translation matrix
			Matrix4x4 mat;
			mat[0] = { 3.8, 0, 0, -5.1 };
			mat[1] = { 0, -1.5, 0, 15.2 };
			mat[2] = { 0, 0, 3.01, 19.9 };

			// Transform vector
			vec = vec * mat;

			// Did we succeed?
			Vector3d expected(
				5.1 * 3.8 - 5.1,
				6.4 * -1.5 + 15.2,
				7.99 * 3.01 + 19.9
			);

			std::wstringstream wss;
			wss << std::endl;
			wss << "Expected: " << expected << std::endl;
			wss << "Actual: " << vec;

			Assert::IsTrue(expected == vec, wss.str().c_str());

			return;
		}

		// A simple matrix multiplication tested on an int vector
		TEST_METHOD(MatrixMult_Dirty_Int_Check)
		{
			// Create vector
			Vector3i vec(5, 6, 7);

			// Create scaling and translation matrix
			Matrix4x4 mat;
			mat[0] = { 3,  0, 0, -5 };
			mat[1] = { 0, -1, 0, 15 };
			mat[2] = { 0,  0, 3, 20 };

			// Transform vector
			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3i(
				5*3 + -5,
				6*-1 + 15,
				7*3 + 20
			) == vec, wss.str().c_str());

			return;
		}

		// Tests the multiplication operator (*) with a simple matrix. All other tests used the * operator (without the '=')
		TEST_METHOD(MatrixMult_Dirty_Int_Check_Not_Using_MultEqualsOperator)
		{
			// Create vector
			Vector3i vec(5, 6, 7);

			// Create scaling and translation matrix
			Matrix4x4 mat;
			mat[0] = { 3,  0, 0, -5 };
			mat[1] = { 0, -1, 0, 15 };
			mat[2] = { 0,  0, 3, 20 };

			// Scale vector
			vec = vec * mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << vec;
			Assert::IsTrue(Vector3i(
				5 * 3 + -5,
				6 * -1 + 15,
				7 * 3 + 20
			) == vec, wss.str().c_str());

			return;
		}

		//This tests the multiplication equals operator (*=) procedurally
		TEST_METHOD(MatrixMult_Equals_Procedural)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Generate parameters
				double initialX = LARGE_RAND_DOUBLE;
				double initialY = LARGE_RAND_DOUBLE;
				double initialZ = LARGE_RAND_DOUBLE;
				double scaleX   = LARGE_RAND_DOUBLE;
				double scaleY   = LARGE_RAND_DOUBLE;
				double scaleZ   = LARGE_RAND_DOUBLE;
				double transX   = LARGE_RAND_DOUBLE;
				double transY   = LARGE_RAND_DOUBLE;
				double transZ   = LARGE_RAND_DOUBLE;

				// Create vector
				Vector3d vec(initialX, initialY, initialZ);

				// Create matrix
				Matrix4x4 mat;
				mat[0] = { scaleX,      0,      0, transX };
				mat[1] = {      0, scaleY,      0, transY };
				mat[2] = {      0,      0, scaleZ, transZ };
				mat[3] = {      0,      0,      0,      0 };

				// Create expected vector
				Vector3d expected(
					initialX * scaleX + transX,
					initialY * scaleY + transY,
					initialZ * scaleZ + transZ
				);

				// Transform vector
				vec *= mat;

				// Compare
				Assert::IsTrue(vec == expected);
			}

			return;
		}

		//This tests the multiplication operator (*) procedurally
		TEST_METHOD(MatrixMult_Procedural)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// Generate parameters
				double initialX = LARGE_RAND_DOUBLE;
				double initialY = LARGE_RAND_DOUBLE;
				double initialZ = LARGE_RAND_DOUBLE;
				double scaleX   = LARGE_RAND_DOUBLE;
				double scaleY   = LARGE_RAND_DOUBLE;
				double scaleZ   = LARGE_RAND_DOUBLE;
				double transX   = LARGE_RAND_DOUBLE;
				double transY   = LARGE_RAND_DOUBLE;
				double transZ   = LARGE_RAND_DOUBLE;

				// Create vector
				Vector3d vec(initialX, initialY, initialZ);

				// Create matrix
				Matrix4x4 mat;
				mat[0] = { scaleX,      0,      0, transX };
				mat[1] = {      0, scaleY,      0, transY };
				mat[2] = {      0,      0, scaleZ, transZ };
				mat[3] = {      0,      0,      0,      0 };

				// Create expected vector
				Vector3d expected(
					initialX * scaleX + transX,
					initialY * scaleY + transY,
					initialZ * scaleZ + transZ
				);

				// Transform vector
				vec = vec * mat;

				// Compare
				Assert::IsTrue(vec == expected);
			}

			return;
		}

		// Tests loose comparison via Vector3d::Similar -> true
		TEST_METHOD(Similar_True)
		{
			Assert::IsTrue(
				Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999).Similar(
					Vector3d(0, -6.666666667, 10)
			));
			return;
		}

		// Tests loose comparison via Vector3d::Similar -> false
		TEST_METHOD(Similar_False)
		{
			Assert::IsFalse(
				Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999).Similar(
					Vector3d(0.1, -6.7, 10.1)
			));
			return;
		}
	};
}
