#include "CppUnitTest.h"
#include "../Eule/Math.h"
#include <random>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Eule;

/** Equivalence classes:
*		-- a < b, 0 < t < 1		-> a < result < b
*		-- a > b, 0 < t < 1		-> b < result < a
*		-- a == b, 0 < t < 1	-> result == b
* 
*		-- a < b, t < 0			-> result < a
*		-- a > b, t < 0			-> result > a
*		-- a == b, t < 0		-> result == b
* 
* 		-- a < b, t > 1			-> result > b
*		-- a > b, t > 1			-> result < b
*		-- a == b, t > 1		-> result == b
* 
*		-- a == 0
*		-- b == 0
*		-- a == b == 0
*/

namespace _Math
{
	TEST_CLASS(_Lerp)
	{
	public:
		// -- a < b, 0 < t < 1		-> a < result < b
		TEST_METHOD(a_lt_b___0_lt_t_lt_1)
		{
			Assert::AreEqual(1.75, Math::Lerp(1, 2, 0.75));
			return;
		}

		// -- a > b, 0 < t < 1		-> b < result < a
		TEST_METHOD(a_gt_b___0_lt_t_lt_1)
		{
			Assert::AreEqual(1.25, Math::Lerp(2, 1, 0.75));
			return;
		}

		// -- a == b, 0 < t < 1		-> result == b
		TEST_METHOD(a_eq_b___0_lt_t_lt_1)
		{
			Assert::AreEqual(2.0, Math::Lerp(2, 2, 0.75));
			return;
		}
		
		// -- a < b, t < 0			-> result < a
		TEST_METHOD(a_lt_b___t_lt_0)
		{
			Assert::AreEqual(0.25, Math::Lerp(1, 2, -0.75));
			return;
		}

		// -- a > b, t < 0			-> result > a
		TEST_METHOD(a_gt_b___t_lt_0)
		{
			Assert::AreEqual(2.75, Math::Lerp(2, 1, -0.75));
			return;
		}

		// -- a == b, t < 0			-> result == b
		TEST_METHOD(a_eq_b___t_lt_0)
		{
			Assert::AreEqual(2.0, Math::Lerp(2, 2, -0.75));
			return;
		}
		
		// -- a < b, t > 1			-> result > b
		TEST_METHOD(a_lt_b___t_gt_1)
		{
			Assert::AreEqual(2.5, Math::Lerp(1, 2, 1.5));
			return;
		}

		// -- a > b, t > 1			-> result < b
		TEST_METHOD(a_gt_b___t_gt_1)
		{
			Assert::AreEqual(0.5, Math::Lerp(2, 1, 1.5));
			return;
		}

		// -- a == b, t > 1			-> result == b
		TEST_METHOD(a_eq_b___t_gt_1)
		{
			Assert::AreEqual(1.0, Math::Lerp(1, 1, 1.5));
			return;
		}

		// -- a == 0
		TEST_METHOD(a_eq_0)
		{
			Assert::AreEqual(2.25, Math::Lerp(0, 3, 0.75));
			return;
		}

		// -- b == 0
		TEST_METHOD(b_eq_0)
		{
			Assert::AreEqual(0.75, Math::Lerp(3, 0, 0.75));
			return;
		}

		// -- a == b == 0
		TEST_METHOD(a_eq_b_eq_0)
		{
			Assert::AreEqual(0.0, Math::Lerp(0, 0, 0.75));
			return;
		}

	};
}
