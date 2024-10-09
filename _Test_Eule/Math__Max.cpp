#include "../_TestingUtilities/Catch2.h"
#include "../Eule/Math.h"

using namespace Eule;

/** Equivalence classes:
*		-- a < b
*		-- a > b
*		-- a == b
*/

// a < b
TEST_CASE(__FILE__"/a_lt_b", "[Math][Max]")
{
    REQUIRE(Math::Max(4.0, 12.0) == 12.0);
    return;
}

// a > b
TEST_CASE(__FILE__"/a_gt_b", "[Math][Max]")
{
    REQUIRE(Math::Max(12.0, 4.0) == 12.0);
    return;
}

// a == b
TEST_CASE(__FILE__"/a_eq_b", "[Math][Max]")
{
    REQUIRE(Math::Max(9.0, 9.0) == 9.0);
    return;
}
