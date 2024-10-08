#include "Catch2.h"
#include <Eule/Math.h>

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

// a < b
TEST_CASE(__FILE__"/a_lt_b", "[Math][Min]")
{
    REQUIRE(Math::Min(4.0, 9.0) == 4.0);
    return;
}

// a > b
TEST_CASE(__FILE__"/a_gt_b", "[Math][Min]")
{
    REQUIRE(Math::Min(9.0, 4.0) == 4.0);
    return;
}

// a == b
TEST_CASE(__FILE__"/a_eq_b", "[Math][Min]")
{
    REQUIRE(Math::Min(9.0, 9.0) == 9.0);
    return;
}
