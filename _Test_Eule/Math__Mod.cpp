#include "../_TestingUtilities/Catch2.h"
#include "../Eule/Math.h"
#include <stdexcept>

using namespace Eule;

/** Equivalence classes:
* a -> numerator
* b -> denominator
*		-- a > 0 && b > 0
*		-- a < 0 && b > 0
*		-- a > 0 && b < 0
*		-- a < 0 && b < 0
* 
*		-- a > 0 && b = 0
*		-- a = 0 && b > 0
* *		-- a < 0 && b = 0
*		-- a = 0 && b < 0
*		-- a = 0 && b = 0
*/

// a > 0 && b > 0
TEST_CASE(__FILE__"/a_gt_0_and_b_gt_0", "[Math][Mod]")
{
    REQUIRE(Math::Mod(199, 32) == 7);
    return;
}

// a < 0 && b > 0
TEST_CASE(__FILE__"/a_lt_0_and_b_gt_0", "[Math][Mod]")
{
    REQUIRE(Math::Mod(-199, 32) == 25);
    return;
}

// a > 0 && b < 0
TEST_CASE(__FILE__"/a_gt_0_and_b_lt_0", "[Math][Mod]")
{
    REQUIRE(Math::Mod(199, -32) == -25);
    return;
}

// a > 0 && b = 0
TEST_CASE(__FILE__"/a_gt_0_and_b_eq_0", "[Math][Mod]")
{
    // Exppect divide-by-zero
    REQUIRE_THROWS_AS(Math::Mod(-199, 0), std::logic_error);
    return;
}

// a = 0 && b > 0
TEST_CASE(__FILE__"/a_eq_0_and_b_gt_0", "[Math][Mod]")
{
    REQUIRE(Math::Mod(0, 32) == 0);
    return;
}

// a < 0 && b = 0
TEST_CASE(__FILE__"/a_lt_0_and_b_eq_0", "[Math][Mod]")
{
    // Expect divide-by-zero
    REQUIRE_THROWS_AS(Math::Mod(-199, 0), std::logic_error);
    return;
}

// a = 0 && b < 0
TEST_CASE(__FILE__"/a_eq_0_and_b_lt_0", "[Math][Mod]")
{
    REQUIRE(Math::Mod(0, -32) == 0);
    return;
}
