#include "CppUnitTest.h"
#include "../Eule/Math.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

/** Equivalence classes:
*		-- value > 0
*		-- value < 0
*		-- value == 0
*/

namespace _Math
{
	TEST_CLASS(_Abs)
	{
	public:
		// Checks with a positive input
		TEST_METHOD(Positive_Value)
		{
			Assert::AreEqual(45.0, Math::Abs(45.0));
			return;
		}

		// Checks with a negative input
		TEST_METHOD(Negative_Value)
		{
			Assert::AreEqual(45.0, Math::Abs(-45.0));
			return;
		}

		// Checks with a zero input
		TEST_METHOD(Zero_Value)
		{
			Assert::AreEqual(0.0, Math::Abs(0.0));
			return;
		}
	};
}
