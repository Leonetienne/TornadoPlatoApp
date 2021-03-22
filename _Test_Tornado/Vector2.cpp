#include "CppUnitTest.h"
#include "../Tornado/Vector2.h"
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

				// Vector length should not matter, so randomize it
				Vector2d a = Vector2d(x, y) * (rng() % 6969);
				Vector2d b = Vector2d(x - (rng() % 6969 + 1.0), y) * (rng() % 6969);

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

				// Vector length should not matter, so randomize it
				Vector2d a = Vector2d(x, y) * (rng() % 6969);
				Vector2d b = Vector2d(x + (rng() % 6969 + 1.0), y) * (rng() % 6969);

				std::wstringstream wss;
				wss << a << L" CROSS " << b << L" = " << a.CrossProduct(b) << std::endl;
				Assert::IsTrue(a.CrossProduct(b) < 0, wss.str().c_str());
			}

			return;
		}
	};
}
