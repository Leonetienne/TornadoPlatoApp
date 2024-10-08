#include "Catch2.h"
#include <Eule/Math.h>

using namespace Eule;

// Checks that the similar function works with an exact comparison -> true
TEST_CASE(__FILE__"/Exact_Comparison_True", "[Math][Similar]")
{
    REQUIRE(Math::Similar(100, 100, 0));
    return;
}

// Checks that the similar function works with an exact comparison -> false
TEST_CASE(__FILE__"/Exact_Comparison_False", "[Math][Similar]")
{
    REQUIRE_FALSE(Math::Similar(100, 100.001, 0));
    return;
}

// Checks that the similar function works with an exact comparison -> false
TEST_CASE(__FILE__"/Exact_Comparison_False2", "[Math][Similar]")
{
    REQUIRE_FALSE(Math::Similar(100, 99.999, 0));
    return;
}

// Checks that the similar function works with a loose comparison -> true
TEST_CASE(__FILE__"/Loose_Comparison_True", "[Math][Similar]")
{
    REQUIRE(Math::Similar(100, 100.001, 0.01));
    return;
}

// Checks that the similar function works with a loose comparison -> true
TEST_CASE(__FILE__"/Loose_Comparison_True2", "[Math][Similar]")
{
    REQUIRE(Math::Similar(100, 99.999, 0.01));
    return;
}

// Checks that the similar function works with a loose comparison -> false
TEST_CASE(__FILE__"/Loose_Comparison_False", "[Math][Similar]")
{
    REQUIRE_FALSE(Math::Similar(100, 100.1, 0.01));
    return;
}

// Checks that the similar function works with a loose comparison -> false
TEST_CASE(__FILE__"/Loose_Comparison_False2", "[Math][Similar]")
{
    REQUIRE_FALSE(Math::Similar(100, 99.9, 0.01));
    return;
}
