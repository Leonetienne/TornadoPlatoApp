#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/Testutil.h"
#include "../Eule/Random.h"

using namespace Eule;

// Checks that a random double is never outside the specification, two positive values
TEST_CASE(__FILE__"/Random_Doublerange_Never_Outside_Specification__pos__pos", "[Random][RandomRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        double rnd = Random::RandomRange(49.0, 99.0);

        INFO("rnd too small");
        REQUIRE(rnd >= 49.0);
        INFO("rnd too big");
        REQUIRE(rnd <= 99.0);
    }

    return;
}

// Checks that a random double is never outside the specification, negative minimum
TEST_CASE(__FILE__"/Random_Doublerange_Never_Outside_Specification__neg__pos", "[Random][RandomRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        double rnd = Random::RandomRange(-39.0, 99.0);

        INFO("rnd too small");
        REQUIRE(rnd >= -39.0);
        INFO("rnd too big");
        REQUIRE(rnd <= 99.0);
    }

    return;
}

// Checks that a random double is never outside the specification, two negative values
TEST_CASE(__FILE__"/Random_Doublerange_Never_Outside_Specification__neg__neg", "[Random][RandomRange]")
{
    // Test 1000 random integers
    for (std::size_t i = 0; i < 1000; i++)
    {
        double rnd = Random::RandomRange(-39.0, -10.0);

        INFO("rnd too small");
        REQUIRE(rnd >= -39.0);
        INFO("rnd too big");
        REQUIRE(rnd <= -10.0);
    }

    return;
}

// Checks that the produced double-precision floating point distribution shows a big standard deviation
TEST_CASE(__FILE__"/Double_Big_Standard_Deviation", "[Random][RandomRange]")
{
    // Setup
    std::vector<double> rands;
    rands.resize(100);

    // Exercise
    // Create 1000 random values
    std::generate_n(rands.data(), rands.size(), []()->double { return Random::RandomRange(100, 4e9); });

    // Verify
    const double stddev = Testutil::Stddev(rands);
    REQUIRE(stddev >= 1000000);

    return;
}
