#include "CppUnitTest.h"
#include "../Tornado/Vector2.h"
#include "../Tornado/Vector3.h"
#include "../Tornado/Vector4.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Vectors
{
	TEST_CLASS(_VectorConversion)
	{
	private:
		std::mt19937 rng;

	public:
		// Constructor
		_VectorConversion()
		{
			rng = std::mt19937((std::random_device())());
			return;

		}

		// Tests that conversion vector2 -> vector3 works
		TEST_METHOD(Convert_Vector2_To_Vector3)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector2d v2(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector3d v3 = v2;
		
				Assert::AreEqual(v2.x, v3.x);
				Assert::AreEqual(v2.y, v3.y);
				Assert::AreEqual(0.0, v3.z);
			}
		
			return;
		}
		
		// Tests that conversion vector2 -> vector4 works
		TEST_METHOD(Convert_Vector2_To_Vector4)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector2d v2(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector4d v4 = v2;
		
				Assert::AreEqual(v2.x, v4.x);
				Assert::AreEqual(v2.y, v4.y);
				Assert::AreEqual(0.0, v4.z);
				Assert::AreEqual(0.0, v4.w);
			}
		
			return;
		}

		// Tests that conversion vector3 -> vector2 works
		TEST_METHOD(Convert_Vector3_To_Vector2)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector3d v3(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector2d v2 = v3;
		
				Assert::AreEqual(v3.x, v2.x);
				Assert::AreEqual(v3.y, v2.y);
			}
		
			return;
		}
		
		// Tests that conversion vector3 -> vector4 works
		TEST_METHOD(Convert_Vector3_To_Vector4)
		{
			// Run test 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector3d v3(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector4d v4 = v3;
		
				Assert::AreEqual(v3.x, v4.x);
				Assert::AreEqual(v3.y, v4.y);
				Assert::AreEqual(v3.z, v4.z);
				Assert::AreEqual(0.0, v4.w);
			}
		
			return;
		}

		// Tests that conversion vector4 -> vector42 works
		TEST_METHOD(Convert_Vector4_To_Vector2)
		{
			// Run tests 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector4d v4(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector2d v2 = v4;
		
				Assert::AreEqual(v4.x, v2.x);
				Assert::AreEqual(v4.y, v2.y);
			}
		
			return;
		}
		
		// Tests that conversion vector4 -> vector3 works
		TEST_METHOD(Convert_Vector4_To_Vector3)
		{
			// Run tests 100 times
			for (std::size_t i = 0; i < 100; i++)
			{
				Vector4d v4(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
				Vector3d v3 = v4;
		
				Assert::AreEqual(v4.x, v3.x);
				Assert::AreEqual(v4.y, v3.y);
				Assert::AreEqual(v4.z, v3.z);
			}
		
			return;
		}
	};
}
