#include "Catch2.h"
#include <Eule/Math.h>

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

// -- a < b, 0 < t < 1		-> a < result < b
TEST_CASE(__FILE__"/a_lt_b___0_lt_t_lt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(1, 2, 0.75) == 1.75);
    return;
}

// -- a > b, 0 < t < 1		-> b < result < a
TEST_CASE(__FILE__"/a_gt_b___0_lt_t_lt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(2, 1, 0.75) == 1.25);
    return;
}

// -- a == b, 0 < t < 1		-> result == b
TEST_CASE(__FILE__"/a_eq_b___0_lt_t_lt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(2, 2, 0.75) == 2.0);
    return;
}

// -- a < b, t < 0			-> result < a
TEST_CASE(__FILE__"/a_lt_b___t_lt_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(1, 2, -0.75) == 0.25);
    return;
}

// -- a > b, t < 0			-> result > a
TEST_CASE(__FILE__"/a_gt_b___t_lt_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(2, 1, -0.75) == 2.75);
    return;
}

// -- a == b, t < 0			-> result == b
TEST_CASE(__FILE__"/a_eq_b___t_lt_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(2, 2, -0.75) == 2.0);
    return;
}

// -- a < b, t > 1			-> result > b
TEST_CASE(__FILE__"/a_lt_b___t_gt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(1, 2, 1.5) == 2.5);
    return;
}

// -- a > b, t > 1			-> result < b
TEST_CASE(__FILE__"/a_gt_b___t_gt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(2, 1, 1.5) == 0.5);
    return;
}

// -- a == b, t > 1			-> result == b
TEST_CASE(__FILE__"/a_eq_b___t_gt_1", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(1, 1, 1.5) == 1.0);
    return;
}

// -- a == 0
TEST_CASE(__FILE__"/a_eq_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(0, 3, 0.75) == 2.25);
    return;
}

// -- b == 0
TEST_CASE(__FILE__"/b_eq_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(3, 0, 0.75) == 0.75);
    return;
}

// -- a == b == 0
TEST_CASE(__FILE__"/a_eq_b_eq_0", "[Math][Lerp]")
{
    REQUIRE(Math::Lerp(0, 0, 0.75) == 0.0);
    return;
}
