#include "CppUnitTest.h"
#include "Similar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestingUtilities
{
	TEST_CLASS(_Similar)
	{
	public:
		
		// Checks that the similar function works with an exact comparison -> true
		TEST_METHOD(Exact_Comparison_True)
		{
			Assert::IsTrue(Similar(100, 100, 0));
			return;
		}

		// Checks that the similar function works with an exact comparison -> false
		TEST_METHOD(Exact_Comparison_False)
		{
			Assert::IsFalse(Similar(100, 100.001, 0));
			return;
		}

		// Checks that the similar function works with an exact comparison -> false
		TEST_METHOD(Exact_Comparison_False2)
		{
			Assert::IsFalse(Similar(100, 99.999, 0));
			return;
		}

		// Checks that the similar function works with a loose comparison -> true
		TEST_METHOD(Loose_Comparison_True)
		{
			Assert::IsTrue(Similar(100, 100.001, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> true
		TEST_METHOD(Loose_Comparison_True2)
		{
			Assert::IsTrue(Similar(100, 99.999, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> false
		TEST_METHOD(Loose_Comparison_False)
		{
			Assert::IsFalse(Similar(100, 100.1, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> false
		TEST_METHOD(Loose_Comparison_False2)
		{
			Assert::IsFalse(Similar(100, 99.9, 0.01));
			return;
		}

		#include "../Tornado/Vector2.h"
		// Tests loose comparison with a Vector2d -> true
		TEST_METHOD(Loose_Comparison_True_Vector2d)
		{
			Assert::IsTrue(Similar(
				Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666),
				Vector2d(0, -6.666666667)
			));
			return;
		}

		// Tests loose comparison with a Vector2d -> false
		TEST_METHOD(Loose_Comparison_False_Vector2d)
		{
			Assert::IsFalse(Similar(
				Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666),
				Vector2d(0.1, -6.7)
			));
			return;
		}

		#include "../Tornado/Vector3.h"
		// Tests loose comparison with a Vector3d -> true
		TEST_METHOD(Loose_Comparison_True_Vector3d)
		{
			Assert::IsTrue(Similar(
				Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999),
				Vector3d(0, -6.666666667, 10)
			));
			return;
		}

		// Tests loose comparison with a Vector3d -> false
		TEST_METHOD(Loose_Comparison_False_Vector3d)
		{
			Assert::IsFalse(Similar(
				Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999),
				Vector3d(0.1, -6.7, 10.1)
			));
			return;
		}

		#include "../Tornado/Vector4.h"
		// Tests loose comparison with a Vector4d -> true
		TEST_METHOD(Loose_Comparison_True_Vector4d)
		{
			Assert::IsTrue(Similar(
				Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333),
				Vector4d(0, -6.666666667, 10, -3.33333333333333)
			));
			return;
		}

		// Tests loose comparison with a Vector4d -> false
		TEST_METHOD(Loose_Comparison_False_Vector4d)
		{
			Assert::IsFalse(Similar(
				Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333),
				Vector4d(0.1, -6.7, 10.1, -3.333)
			));
			return;
		}
	};
}
