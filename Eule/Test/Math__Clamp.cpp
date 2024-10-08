#include "Catch2.h"
#include <Eule/Math.h>

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

// min < v < max	-> v
TEST_CASE(__FILE__"/min_lt_v_lt_max", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(6.0, 4.0, 9.0) == 6.0);
    return;
}

// v < min < max	-> min
TEST_CASE(__FILE__"/v_lt_min_max", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(4.0, 6.0, 9.0) == 6.0);
    return;
}

// min < max < v	-> max
TEST_CASE(__FILE__"/min_lt_max_lt_v", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(12.0, 6.0, 9.0) == 9.0);
    return;
}

// v == min < max	-> min
TEST_CASE(__FILE__"/v_eq_min_lt_max", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(6.0, 6.0, 9.0) == 6.0);
    return;
}

// min < v == max	-> max
TEST_CASE(__FILE__"/min_lt_v_eq_max", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(9.0, 6.0, 9.0) == 9.0);
    return;
}

// v < max == min	-> max
TEST_CASE(__FILE__"/v_lt_max_eq_min", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(9.0, 6.0, 9.0) == 9.0);
    return;
}

// max == min < v	-> max
TEST_CASE(__FILE__"/max_eq_min_lt_v", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(15.0, 9.0, 9.0) == 9.0);
    return;
}

// min == v == max	-> max
TEST_CASE(__FILE__"/min_eq_v_eq_max", "[Math][Clamp]")
{
    REQUIRE( Math::Clamp(15.0, 15.0, 15.0) == 15.);
    return;
}

// max < v < min	-> min
TEST_CASE(__FILE__"/max_lt_v_lt_min", "[Math][Clamp]")
{
    REQUIRE(Math::Clamp(4.0, 7.0, 3.0) == 7.0);
    return;
}
