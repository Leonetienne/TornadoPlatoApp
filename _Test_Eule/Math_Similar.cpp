#include "CppUnitTest.h"
#include "../Eule/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

namespace _Math
{
	TEST_CLASS(_Similar)
	{
	public:
		// Checks that the similar function works with an exact comparison -> true
		TEST_METHOD(Exact_Comparison_True)
		{
			Assert::IsTrue(Math::Similar(100, 100, 0));
			return;
		}

		// Checks that the similar function works with an exact comparison -> false
		TEST_METHOD(Exact_Comparison_False)
		{
			Assert::IsFalse(Math::Similar(100, 100.001, 0));
			return;
		}

		// Checks that the similar function works with an exact comparison -> false
		TEST_METHOD(Exact_Comparison_False2)
		{
			Assert::IsFalse(Math::Similar(100, 99.999, 0));
			return;
		}

		// Checks that the similar function works with a loose comparison -> true
		TEST_METHOD(Loose_Comparison_True)
		{
			Assert::IsTrue(Math::Similar(100, 100.001, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> true
		TEST_METHOD(Loose_Comparison_True2)
		{
			Assert::IsTrue(Math::Similar(100, 99.999, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> false
		TEST_METHOD(Loose_Comparison_False)
		{
			Assert::IsFalse(Math::Similar(100, 100.1, 0.01));
			return;
		}

		// Checks that the similar function works with a loose comparison -> false
		TEST_METHOD(Loose_Comparison_False2)
		{
			Assert::IsFalse(Math::Similar(100, 99.9, 0.01));
			return;
		}
	};
}
