#include "CppUnitTest.h"
#include "../Tornado/Vector3.h"
#include "Similar.h"
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

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);
			Assert::AreEqual(a.z, b.z);

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

		// Tests for the cross product between the same vector being 0
		TEST_METHOD(CrossProduct_Same_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;

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
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;

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

		// Tests the SqrMagnitude method to work as expected with random numbers
		TEST_METHOD(SqrMagnitude)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				double expected = x*x + y*y + z*z;

				Assert::AreEqual(expected, Vector3d(x, y, z).SqrMagnitude());
			}

			return;
		}

		// Tests for the length of the vector (0,0) being 0
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
				double x = (rng() % 6969) - 3500;
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
				double y = (rng() % 6969) - 3500;
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
				double z = (rng() % 6969) - 3500;
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

		// Tests for any normalized vector to be of length 1
		TEST_METHOD(Normalize_Length_Is_1)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				Vector3d vec(x, y, z);

				vec.Normalize();

				std::wstringstream wss;
				wss << vec;
				Assert::IsTrue(Similar(vec.Magnitude(), 1.0), wss.str().c_str()); // Account for floating point inaccuracy

			}

			return;
		}

		// Tests for a normalized vector to still point in the exact same direction
		TEST_METHOD(Normalize_Direction_Stays_Unaffected)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				if (x == 0) x++;
				if (y == 0) y++;
				if (z == 0) z++;
		
				Vector3d vec(x, y, z);
		
				Vector3d vec_n(x, y, z);
				vec_n.Normalize();
		
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

		// Tests for operator+ to work as expected
		TEST_METHOD(Operator_Add)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

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
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

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
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

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
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

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
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				double scalar = (rng() % 6969) - 3500;

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
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				double scalar = (rng() % 6969) - 3500;

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
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				double scalar = (rng() % 6969) - 3500;

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
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				double z = (rng() % 6969) - 3500;
				double scalar = (rng() % 6969) - 3500;

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
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

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
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = (rng() % 6969) - 3500;
				double ay = (rng() % 6969) - 3500;
				double az = (rng() % 6969) - 3500;
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;
				double bz = (rng() % 6969) - 3500;

				Vector3d a(ax, ay, az);
				Vector3d b(bx, by, bz);

				Assert::IsTrue(
					((ax != bx) || (ay != by) || (az == bz)) ==
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

			// Create 90deg yaw rotation matrix
			Matrix4x4 mat;
			mat[2][0] = 1;
			mat[1][1] = 1;
			mat[0][2] = -1;

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

			// Rotate by 90 deg a last time, having completed a 360� rotation.
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

			// Create 90deg yaw rotation matrix
			Matrix4x4 mat;
			//mat[1][0] = -1;
			//mat[0][1] = 1;
			//mat[2][2] = 1;
			mat[0][1] = -1;
			mat[1][0] = 1;
			mat[2][2] = 1;

			// Rotate vector

			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << L"Rot #1 " << vec;
			Assert::IsTrue(Vector3d(32, -69, 16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #2 " << vec;
			Assert::IsTrue(Vector3d(-69, -32, 16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #3 " << vec;
			Assert::IsTrue(Vector3d(-32, 69, 16) == vec, wss.str().c_str());

			// Rotate by 90 deg a last time, having completed a 360� rotation.
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

			// Create 90deg yaw rotation matrix
			Matrix4x4 mat;
			//mat[0][0] = 1;
			//mat[2][1] = -1;
			//mat[1][2] = 1;
			mat[0][0] = 1;
			mat[1][2] = -1;
			mat[2][1] = 1;

			// Rotate vector

			vec *= mat;

			// Did we succeed?
			std::wstringstream wss;
			wss << L"Rot #1 " << vec;
			Assert::IsTrue(Vector3d(69, 16, -32) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #2 " << vec;
			Assert::IsTrue(Vector3d(69, -32, -16) == vec, wss.str().c_str());

			// Rotate again!
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #3 " << vec;
			Assert::IsTrue(Vector3d(69, -16, 32) == vec, wss.str().c_str());

			// Rotate by 90 deg a last time, having completed a 360� rotation.
			vec *= mat;
			wss.str(L"");
			wss << L"Rot #4 " << vec;
			Assert::IsTrue(originalVec == vec, wss.str().c_str());

			return;
		}
	};
}