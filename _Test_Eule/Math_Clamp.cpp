#include "CppUnitTest.h"
#include "../Eule/Math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

/** Equivalence classes:
*		-- min < v < max	-> v
*		-- v < min < max	-> min
*		-- min < max < v	-> max
*		-- v == min < max	-> min
*		-- min < v == max	-> max
*		-- v < max == min	-> max
*		-- max == min < v	-> max
*		-- min == v == max	-> max
*		-- max < v < min	-> min
*/

namespace _Math
{
	TEST_CLASS(_Clamp)
	{
	public:
		// min < v < max	-> v
		TEST_METHOD(min_lt_v_lt_max)
		{
			Assert::AreEqual(6.0, Math::Clamp(6.0, 4.0, 9.0));
			return;
		}

		// v < min < max	-> min
		TEST_METHOD(v_lt_min_max)
		{
			Assert::AreEqual(6.0, Math::Clamp(4.0, 6.0, 9.0));
			return;
		}

		// min < max < v	-> max
		TEST_METHOD(min_lt_max_lt_v)
		{
			Assert::AreEqual(9.0, Math::Clamp(12.0, 6.0, 9.0));
			return;
		}

		// v == min < max	-> min
		TEST_METHOD(v_eq_min_lt_max)
		{
			Assert::AreEqual(6.0, Math::Clamp(6.0, 6.0, 9.0));
			return;
		}

		// min < v == max	-> max
		TEST_METHOD(min_lt_v_eq_max)
		{
			Assert::AreEqual(9.0, Math::Clamp(9.0, 6.0, 9.0));
			return;
		}

		// v < max == min	-> max
		TEST_METHOD(v_lt_max_eq_min)
		{
			Assert::AreEqual(9.0, Math::Clamp(9.0, 6.0, 9.0));
			return;
		}

		// max == min < v	-> max
		TEST_METHOD(max_eq_min_lt_v)
		{
			Assert::AreEqual(9.0, Math::Clamp(15.0, 9.0, 9.0));
			return;
		}

		// min == v == max	-> max
		TEST_METHOD(min_eq_v_eq_max)
		{
			Assert::AreEqual(15.0, Math::Clamp(15.0, 15.0, 15.0));
			return;
		}

		// max < v < min	-> min
		TEST_METHOD(max_lt_v_lt_min)
		{
			Assert::AreEqual(7.0, Math::Clamp(4.0, 7.0, 3.0));
			return;
		}
	};
}
