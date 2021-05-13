#include "CppUnitTest.h"
#include "../Tornado/Vector2.h"
#include "../Tornado/Similar.h"
#include "../_TestingUtilities/HandyMacros.h"

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

			b.x = 169;
			b.y = 132;

			Assert::AreEqual(69.0, a.x);
			Assert::AreEqual(32.0, a.y);

			Assert::AreEqual(169.0, b.x);
			Assert::AreEqual(132.0, b.y);

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

		// Tests that the dot product is correct for a known value
		TEST_METHOD(DotProduct_Oracle)
		{
			// Setup
			Vector2d a(-99, 199);
			Vector2d b(18, -1);

			// Exercise
			const double dot = a.DotProduct(b);

			// Verify
			Assert::AreEqual(-1981.0, dot);

			return;
		}

		// Quick and dirty check if the useless int-method is working
		TEST_METHOD(DotProduct_Dirty_Int)
		{
			Vector2i a;
			Vector2i b;
			std::wstringstream wss;

			// 90 deg
			a = {0, 10};
			b = {10, 0};
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::AreEqual(0.0, a.DotProduct(b), wss.str().c_str());
			Assert::AreEqual(0.0, b.DotProduct(a), wss.str().c_str());

			// < 90 deg
			a = { 7, 10 };
			b = { 10, 1 };
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::IsTrue(a.DotProduct(b) > 0.0, wss.str().c_str());
			Assert::IsTrue(b.DotProduct(a) > 0.0, wss.str().c_str());

			// > 90 deg
			a = { -3, 10 };
			b = { 10, -4 };
			wss.str(L"");
			wss << a << L" DOT " << b << L" = " << a.DotProduct(b) << std::endl;
			Assert::IsTrue(a.DotProduct(b) < 0.0, wss.str().c_str());
			Assert::IsTrue(b.DotProduct(a) < 0.0, wss.str().c_str());

			return;
		}

		// Tests if the cross product of two vectors of the exact opposite direction is 0
		TEST_METHOD(CrossProduct_Opposite_Direction)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE / 1000.0;
				double y = LARGE_RAND_DOUBLE / 1000.0;

				// Vector length should not matter, so randomize it
				// In this case, they are allowed to be of length 0
				// Don't scale it up too much to avoid failure due to floating point inaccuracy
				Vector2d a = Vector2d( x,  y) * (LARGE_RAND_DOUBLE / 1000.0);
				Vector2d b = Vector2d(-x, -y) * (LARGE_RAND_DOUBLE / 1000.0);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(Similar(a.CrossProduct(b), 0.0, 10), wss.str().c_str());
			}

			return;
		}

		// Tests if the cross product of two vectors of the exact same direction is 0
		TEST_METHOD(CrossProduct_Same_Direction)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE / 1000.0;
				double y = LARGE_RAND_DOUBLE / 1000.0;

				// Vector length should not matter, so randomize it
				// In this case, they are allowed to be of length 0
				// Don't scale it up too much to avoid failure due to floating point inaccuracy
				Vector2d a = Vector2d(x, y) * (LARGE_RAND_DOUBLE / 1000.0);
				Vector2d b = Vector2d(x, y) * (LARGE_RAND_DOUBLE / 1000.0);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(Similar(a.CrossProduct(b), 0.0, 10), wss.str().c_str());
			}

			return;
		}

		// Tests for the cross product to be positive, if vector b is to the left of a
		TEST_METHOD(CrossProduct_BToTheLeft)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
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
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
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

		// Quick and dirty check if the useless int-method is working
		TEST_METHOD(CrossProduct_Dirty_Int)
		{
			Vector2i a;
			Vector2i b;
			std::wstringstream wss;

			// Same direction
			a = { 10, 0 };
			b = { 10, 0 };
			wss.str(L"");
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::AreEqual(0.0, a.CrossProduct(b), wss.str().c_str());
			Assert::AreEqual(0.0, b.CrossProduct(a), wss.str().c_str());

			// Opposite direction
			a = { -10, 0 };
			b = { 10, 0 };
			wss.str(L"");
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::AreEqual(0.0, a.CrossProduct(b), wss.str().c_str());
			Assert::AreEqual(0.0, b.CrossProduct(a), wss.str().c_str());

			// B to the left
			a = { 0, 10 };
			b = { -5, 10 };
			wss.str(L"");
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(a.CrossProduct(b) > 0.0, wss.str().c_str());

			// B to the right
			a = { 0, 10 };
			b = { 17, 10 };
			wss.str(L"");
			wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
			Assert::IsTrue(a.CrossProduct(b) < 0.0, wss.str().c_str());

			return;
		}

		// Tests the SqrMagnitude method to work as expected with random numbers
		TEST_METHOD(SqrMagnitude)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = (double)(rng() % 1000) - 500.0;
				double y = (double)(rng() % 1000) - 500.0;
				double expected = x*x + y*y;

				Assert::AreEqual(expected, Vector2d(x, y).SqrMagnitude());
			}

			return;
		}

		// Checks if the int method is working
		TEST_METHOD(SqrMagnitude_Int)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				int x = LARGE_RAND_INT;
				int y = LARGE_RAND_INT;
				int expected = x*x + y*y;

				Assert::IsTrue(Similar((double)expected, Vector2i(x, y).SqrMagnitude()));
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
				double x = (double)(rng() % 1000) - 500.0;
				Vector2d vec(x, 0);
				Assert::IsTrue(Similar(abs(x), vec.Magnitude()));
			}

			return;
		}

		// Tests for a vector of a known length to actually return that
		TEST_METHOD(Magnitude_One_Axis_Y)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double y = (double)(rng() % 1000) - 500.0;
				Vector2d vec(0, y);
				Assert::IsTrue(Similar(abs(y), vec.Magnitude()));
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

		// Tests lerp as a static function
		TEST_METHOD(Lerp_Static)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);
			Vector2d res = Vector2d::Lerp(a, b, 0.75);

			std::wstringstream wss;
			wss << res;
			Assert::IsTrue(Vector2d(175, 3250) == res, wss.str().c_str());
			return;
		}

		// Tests lerpself
		TEST_METHOD(LerpSelf)
		{
			Vector2d a(100, 1000);
			Vector2d b(200, 4000);

			a.LerpSelf(b, 0.75);

			std::wstringstream wss;
			wss << a;
			Assert::IsTrue(Vector2d(175, 3250) == a, wss.str().c_str());
			return;
		}

		// Tests if an input vector of length 0 is handled correctly by the normalize method
		TEST_METHOD(Normalize_Length_Before_Is_0)
		{
			Vector2d vec(0, 0);
			Assert::AreEqual(0.0, vec.Normalize().Magnitude());
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
				if (x == 0) x++;
				if (y == 0) y++;

				Vector2d vec(x, y);

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
			Vector2d v(3.2, -5.3);

			// Exercise
			v.NormalizeSelf();

			// Verify
			Vector2d expected(0.51686909903, -0.85606444527);
			Assert::IsTrue(v.Similar(expected));
		}

		// Tests for a normalized vector to still point in the exact same direction
		TEST_METHOD(Normalize_Direction_Stays_Unaffected)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;

				Vector2d vec(x, y);

				// Prevent a vector of length 0 going in
				if (vec.SqrMagnitude() == 0)
					vec.x++;

				Vector2d vec_n(x, y);
				vec_n = vec_n.Normalize();

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

		// Kinda dumb method, but ok lol
		// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
		TEST_METHOD(Normalized_Int_Vector_Is_0)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				int x = LARGE_RAND_INT;
				int y = LARGE_RAND_INT;

				Vector2i vec(x, y);

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
				Vector2d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				
				Vector2d nVec = vec.Normalize();
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
				const double bx = LARGE_RAND_DOUBLE;
				const double by = LARGE_RAND_DOUBLE;
			
				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Vector2d target(
					ax * bx,
					ay * by
				);

				Assert::IsTrue(a.VectorScale(b) == target);
			}

			return;
		}

		// Tests for operator- (unary) to work
		TEST_METHOD(Operator_Unary_Negative)
		{
			Vector2d v(29, -5);

			Assert::IsTrue(Vector2d(-29, 5) == -v);

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
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;

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
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;

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
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;

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
				double ax = LARGE_RAND_DOUBLE;
				double ay = LARGE_RAND_DOUBLE;
				double bx = LARGE_RAND_DOUBLE;
				double by = LARGE_RAND_DOUBLE;

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
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

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
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

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
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

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
				double x = LARGE_RAND_DOUBLE;
				double y = LARGE_RAND_DOUBLE;
				double scalar = LARGE_RAND_DOUBLE;

				Vector2d a(x, y);
				a /= scalar;

				Assert::IsTrue(Vector2d(x / scalar, y / scalar) == a);
			}

			return;
		}

		// Tests for operator== to work as expected
		TEST_METHOD(Operator_Compare_Equals)
		{
			// Test 1000 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				double ax = (rng() % 10) - 5;
				double ay = (rng() % 10) - 5;
				double bx = (rng() % 10) - 5;
				double by = (rng() % 10) - 5;

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
				double ax = (rng() % 10) - 5;
				double ay = (rng() % 10) - 5;
				double bx = (rng() % 10) - 5;
				double by = (rng() % 10) - 5;

				Vector2d a(ax, ay);
				Vector2d b(bx, by);

				Assert::IsTrue(
					((ax != bx) || (ay != by)) ==
					(a != b)
				);
			}

			return;
		}

		// Tests loose comparison via Vector2d::Similar -> true
		TEST_METHOD(Similar_True)
		{
			Assert::IsTrue(
				Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666).Similar(
					Vector2d(0, -6.666666667)
			));
			return;
		}

		// Tests loose comparison via Vector2d::Similar -> false
		TEST_METHOD(Similar_False)
		{
			Assert::IsFalse(
				Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666).Similar(
					Vector2d(0.1, -6.7)
			));
			return;
		}
	};
}
