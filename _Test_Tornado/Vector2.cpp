#include "CppUnitTest.h"
#include "../Tornado/Vector2.h"
#include "Similar.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vectors
{
	TEST_CLASS(_Vector2)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_Vector2()
		{
			rng = std::mt19937((std::random_device())());
			return;
		}

		// Tests if all values are 0 after initialization via default constructor
		TEST_METHOD(New_Vector_All_0)
		{
			Vector2d v2;

			Assert::AreEqual(0.0, v2.x);
			Assert::AreEqual(0.0, v2.y);

			return;
		}

		// Tests if values can be set via the constructor
		TEST_METHOD(Can_Set_Values_Constructor)
		{
			Vector2d v2(69, 32);

			Assert::AreEqual(69.0, v2.x);
			Assert::AreEqual(32.0, v2.y);

			return;
		}

		// Tests if values can be set via letters
		TEST_METHOD(Can_Set_Values_Letters)
		{
			Vector2d v2;
			v2.x = 69;
			v2.y = 32;

			Assert::AreEqual(69.0, v2.x);
			Assert::AreEqual(32.0, v2.y);

			return;
		}

		// Tests if values can be set via array descriptors
		TEST_METHOD(Can_Set_Values_ArrayDescriptor)
		{
			Vector2d v2;
			v2[0] = 69;
			v2[1] = 32;

			Assert::AreEqual(69.0, v2.x);
			Assert::AreEqual(32.0, v2.y);

			return;
		}

		// Tests if values can be set via an initializer list
		TEST_METHOD(Can_Set_Values_InitializerList)
		{
			Vector2d v2 = {69, 32};

			Assert::AreEqual(69.0, v2.x);
			Assert::AreEqual(32.0, v2.y);

			return;
		}

		// Tests for vectors copied via the copy constructor to have the same values
		TEST_METHOD(Copy_Constructor_Same_Values)
		{
			Vector2d a(69, 32);
			Vector2d b(a);

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);

			return;
		}

		// Tests for vectors copied via the equals operator to have the same values
		TEST_METHOD(Operator_Equals_Same_Values)
		{
			Vector2d a(69, 32);
			Vector2d b = a;

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);

			return;
		}

		// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
		TEST_METHOD(Copy_Constructor_Independent)
		{
			Vector2d a(69, 32);
			Vector2d b(a);

			b.x = 169;
			b.y = 132;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);

			return;
		}

		// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
		TEST_METHOD(Operator_Equals_Independent)
		{
			Vector2d a(69, 32);
			Vector2d b = a;

			Assert::AreEqual(a.x, b.x);
			Assert::AreEqual(a.y, b.y);

			return;
		}

		// Tests if the dot product between two vectors angled 90 degrees from one another is 0. It should by definition be 0!
		// Dot products are commutative, so we'll check both directions.
		TEST_METHOD(DotProduct_90deg)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 100; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector2d a = Vector2d(1, 0) * (rng() % 6969 + 1.0);
				Vector2d b = Vector2d(0, 1) * (rng() % 6969 + 1.0);

				std::wstringstream wss;
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
				Vector2d a = Vector2d(1, 1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector2d b = Vector2d(1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);


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
				Vector2d a = Vector2d(1, -1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector2d b = Vector2d(-1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);

				std::wstringstream wss;
				wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
				Assert::IsTrue(a.DotProduct(b) < 0, wss.str().c_str());
				Assert::IsTrue(b.DotProduct(a) < 0, wss.str().c_str());
			}

			return;
		}

		// Tests if the cross product of two vectors of the exact opposite direction is 0
		TEST_METHOD(CrossProduct_Opposite_Direction)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;

				// Vector length should not matter, so randomize it
				// In this case, they are allowed to be of length 0
				Vector2d a = Vector2d(x, y) * (rng() % 6969);
				Vector2d b = Vector2d(-x, -y) * (rng() % 6969);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.CrossProduct(b), wss.str().c_str());
			}

			return;
		}

		// Tests if the cross product of two vectors of the exact same direction is 0
		TEST_METHOD(CrossProduct_Same_Direction)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;

				// Vector length should not matter, so randomize it
				// In this case, they are allowed to be of length 0
				Vector2d a = Vector2d(x, y) * (rng() % 6969);
				Vector2d b = Vector2d(x, y) * (rng() % 6969);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::AreEqual(0.0, a.CrossProduct(b), wss.str().c_str());
			}

			return;
		}

		// Tests for the cross product to be positive, if vector b is to the left of a
		TEST_METHOD(CrossProduct_BToTheLeft)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				if (x == 0) x++;
				if (y == 0) y++;

				// Vector length should not matter, so randomize it
				Vector2d a = Vector2d(x, y) * (rng() % 6969 + 1.0);
				Vector2d b = Vector2d(x - (rng() % 6969 + 1.0), y) * (rng() % 6969 + 1.0);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(a.CrossProduct(b) > 0, wss.str().c_str());
			}

			return;
		}

		// Tests for the cross product to be negative, if vector b is to the left of a
		TEST_METHOD(CrossProduct_BToTheRight)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				double y = (rng() % 6969) - 3500;
				if (x == 0) x++;
				if (y == 0) y++;

				// Vector length should not matter, so randomize it
				Vector2d a = Vector2d(x, y) * (rng() % 6969 + 1.0);
				Vector2d b = Vector2d(x + (rng() % 6969 + 1.0), y) * (rng() % 6969 + 1.0);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(a.CrossProduct(b) < 0, wss.str().c_str());
			}

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
				double expected = x*x + y*y;

				Assert::AreEqual(expected, Vector2d(x, y).SqrMagnitude());
			}

			return;
		}

		// Tests for the length of the vector (0,0) being 0
		TEST_METHOD(Magnitude_Is_0_On_Vec0)
		{
			Assert::AreEqual(0.0, Vector2d(0, 0).Magnitude());
			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_X)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (rng() % 6969) - 3500;
				Vector2d vec(x, 0);
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
				Vector2d vec(0, y);
				Assert::AreEqual(abs(y), vec.Magnitude());
			}

			return;
		}

		// Tests for a known result
		TEST_METHOD(Magnitude)
		{
			// Ya'll got more of 'dem digits?
			Assert::AreEqual(204.02205763103165736538358032703399658203125, Vector2d(192, -69).Magnitude());
			return;
		}

		// Tests for expected lerp result 0.00
		TEST_METHOD(Lerp_000)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = a.Lerp(b, 0.00);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(a == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.25
		TEST_METHOD(Lerp_025)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = a.Lerp(b, 0.25);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector2d(125, 1750) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.50
		TEST_METHOD(Lerp_050)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = a.Lerp(b, 0.50);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector2d(150, 2500) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 0.75
		TEST_METHOD(Lerp_075)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = a.Lerp(b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector2d(175, 3250) == res, wss.str().c_str());
			return;
		}

		// Tests for expected lerp result 1.00
		TEST_METHOD(Lerp_100)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = a.Lerp(b, 1.00);

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
				Vector2d vec(x, y);

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
				if (x == 0) x++;
				if (y == 0) y++;

				Vector2d vec(x, y);

				Vector2d vec_n(x, y);
				vec_n.Normalize();

				std::wstringstream wss;
				wss << vec << L" | " << vec_n;

				// Both vectors should still point in the same direction!
				Assert::IsTrue(
					(vec.DotProduct(vec_n) > 0) && // Roughly same direction
					(Similar(vec_n.CrossProduct(vec), 0.0)), // Both vectors align
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
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;

				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Assert::IsTrue(Vector2d(ax+bx, ay+by) == a+b);
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
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;

				Vector2d a(ax, ay);
				a += Vector2d(bx, by);

				Assert::IsTrue(Vector2d(ax + bx, ay + by) == a);
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
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;

				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Assert::IsTrue(Vector2d(ax - bx, ay - by) == a - b);
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
				double bx = (rng() % 6969) - 3500;
				double by = (rng() % 6969) - 3500;

				Vector2d a(ax, ay);
				a -= Vector2d(bx, by);

				Assert::IsTrue(Vector2d(ax - bx, ay - by) == a);
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
				double scalar = (rng() % 6969) - 3500;

				Vector2d a(x, y);

				Assert::IsTrue(Vector2d(x * scalar, y * scalar) == a * scalar);
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
				double scalar = (rng() % 6969) - 3500;

				Vector2d a(x, y);
				a *= scalar;

				Assert::IsTrue(Vector2d(x * scalar, y * scalar) == a);
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
				double scalar = (rng() % 6969) - 3500;

				Vector2d a(x, y);

				Assert::IsTrue(Vector2d(x / scalar, y / scalar) == a / scalar);
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
				double scalar = (rng() % 6969) - 3500;

				Vector2d a(x, y);
				a /= scalar;

				Assert::IsTrue(Vector2d(x / scalar, y / scalar) == a);
			}

			return;
		}

		// Tests for operator== to work as expected
		TEST_METHOD(Operator_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = (rng() % 20) - 10;
				double ay = (rng() % 20) - 10;
				double bx = (rng() % 20) - 10;
				double by = (rng() % 20) - 10;

				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Assert::IsTrue(
					((ax == bx) && (ay == by)) ==
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
				double ax = (rng() % 20) - 10;
				double ay = (rng() % 20) - 10;
				double bx = (rng() % 20) - 10;
				double by = (rng() % 20) - 10;

				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Assert::IsTrue(
					((ax != bx) || (ay != by)) ==
					(a != b)
				);
			}

			return;
		}
	};
}
