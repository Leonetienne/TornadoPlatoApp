#include "CppUnitTest.h"
#include "../Eule/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

/** Equivalence classes:
*		-- min < v < max
*		-- v < min < max
*		-- min < max < v
*		-- v == min < max
*		-- min < v == max
*		-- v < max == min
*		-- max == min < v
*		-- max == min == v
*		-- max < v < min
*/

/** Equivalence classes:
*		-- a < b
*		-- a > b
*		-- a == b
*/

namespace _Math
{
	TEST_CLASS(_Min)
	{
	public:
		// a < b
		TEST_METHOD(a_lt_b)
		{
			Assert::AreEqual(4.0, Math::Min(4.0, 9.0));
			return;
		}

		// a > b
		TEST_METHOD(a_gt_b)
		{
			Assert::AreEqual(4.0, Math::Min(9.0, 4.0));
			return;
		}

		// a == b
		TEST_METHOD(a_eq_b)
		{
			Assert::AreEqual(9.0, Math::Min(9.0, 9.0));
			return;
		}

	};
}
