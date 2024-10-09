#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/Testutil.h"
#include "../Eule/Random.h"
#include <array>
#include <sstream>

using namespace Eule;

// Checks that a random integer is never outside the specification, two positive values
TEST_CASE(__FILE__"/Never_Outside_Specification__pos__pos", "[Random][RandomIntRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        int rnd = Random::RandomIntRange(49, 99);

        INFO("rnd too small");
        REQUIRE(rnd >= 49);
        INFO("rnd too big");
        REQUIRE(rnd <= 99);
    }

    return;
}

// Checks that a random integer is never outside the specification, negative minimum
TEST_CASE(__FILE__"/Never_Outside_Specification__neg__pos", "[Random][RandomIntRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        int rnd = Random::RandomIntRange(-39, 99);

        INFO("rnd too small");
        REQUIRE(rnd >= -39);
        INFO("rnd too big");
        REQUIRE(rnd <= 99);
    }

    return;
}

// Checks that a random integer is never outside the specification, two negative values
TEST_CASE(__FILE__"/Never_Outside_Specification__neg__neg", "[Random][RandomIntRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        int rnd = Random::RandomIntRange(-39, -10);

        INFO("rnd too small");
        REQUIRE(rnd >= -39);
        INFO("rnd too big");
        REQUIRE(rnd <= -10);
    }

    return;
}

// Checks that the random intrange method also returns the supplied limits
TEST_CASE(__FILE__"/Inclusivity", "[Random][RandomIntRange]")
{
    // Test 1000 random integers
    // The chance that any number [0,9] will not drop at least once is basically 0

    std::array<bool, 10> foundDigits;
    foundDigits.fill(false);

    for (std::size_t i = 0; i < 1000; i++)
    {
        int randomVal = Random::RandomIntRange(0, 9);
        foundDigits[randomVal] = true;
    }

    // Check that each value has been rolled at least once
    for (const bool& b : foundDigits)
        REQUIRE(b);

    return;
}

// Checks that the produced integer distribution shows a big standard deviation
TEST_CASE(__FILE__"/Big_Standard_Deviation", "[Random][RandomIntRange]")
{
    // Setup
    std::vector<int> rands;
    rands.resize(1000);

    // Exercise
    // Create 1000 random values
    std::generate_n(rands.data(), rands.size(), []()->int { return Random::RandomIntRange(100, (int)4e8); });

    // Verify
    std::stringstream ss;
    ss << "[";
    for (int i : rands)
        ss << i << ", ";
    ss << "]";

    const double stddev = Testutil::Stddev(rands);
    INFO(ss.str());
    REQUIRE(stddev >= 1000000);

    return;
}
