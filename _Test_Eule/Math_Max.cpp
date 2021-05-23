#include "CppUnitTest.h"
#include "../Eule/Math.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/** Equivalence classes:
*		-- a < b
*		-- a > b
*		-- a == b
*/

namespace _Math
{
	TEST_CLASS(_Max)
	{
	public:
		// a < b
		TEST_METHOD(a_lt_b)
		{
			Assert::AreEqual(12.0, Math::Max(4.0, 12.0));
			return;
		}

		// a > b
		TEST_METHOD(a_gt_b)
		{
			Assert::AreEqual(12.0, Math::Max(12.0, 4.0));
			return;
		}

		// a == b
		TEST_METHOD(a_eq_b)
		{
			Assert::AreEqual(9.0, Math::Max(9.0, 9.0));
			return;
		}

	};
}
