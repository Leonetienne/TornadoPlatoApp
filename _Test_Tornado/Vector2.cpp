#include "CppUnitTest.h"
#include "../Tornado/Vector2.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TornadoTest
{
	TEST_CLASS(_Vector2)
	{
	public:

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
			std::mt19937 rng((std::random_device())());

			// Test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector2d a = Vector2d(1, 0) * (rng() % 6969);
				Vector2d b = Vector2d(0, 1) * (rng() % 6969);

				Assert::AreEqual(0.0, a.DotProduct(b));
				Assert::AreEqual(0.0, b.DotProduct(a));
			}

			return;
		}

		// Test if the dot product is positive for two vectors angled less than 90 degrees from another
		// Dot products are commutative, so we'll check both directions.
		TEST_METHOD(DotProduct_LessThan90deg)
		{
			std::mt19937 rng((std::random_device())());

			// Test 100 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector2d a = Vector2d(1, 1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector2d b = Vector2d(1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);


				std::wstringstream wss;
				wss << "(" << a.x << ", " << a.y << ") DOT (" << b.x << ", " << b.y << ")" << std::endl;
				Assert::IsTrue(a.DotProduct(b) > 0, wss.str().c_str());
				Assert::IsTrue(b.DotProduct(a) > 0, wss.str().c_str());
			}

			return;
		}

		// Test if the dot product is negative for two vectors angled greater than 90 degrees from another
		// Dot products are commutative, so we'll check both directions.
		TEST_METHOD(DotProduct_GreaterThan90deg)
		{
			std::mt19937 rng((std::random_device())());

			// Test 100 times
			for (std::size_t i = 0; i < 1000; i++)
			{
				// The length of the vectors should not matter. Only the angle should.
				// Let's test that!
				Vector2d a = Vector2d(1, -1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
				Vector2d b = Vector2d(-1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);

				std::wstringstream wss;
				wss << "(" << a.x << ", " << a.y << ") DOT (" << b.x << ", " << b.y << ")" << std::endl;
				Assert::IsTrue(a.DotProduct(b) < 0, wss.str().c_str());
				Assert::IsTrue(b.DotProduct(a) < 0, wss.str().c_str());
			}

			return;
		}
	};
}
