#include "Catch2.h"
#include "../_TestingUtilities/Testutil.h"
#include <Eule/Random.h>
#include <array>

using namespace Eule;

// Checks that the produced unsigned-integer distribution shows a big standard deviation
TEST_CASE(__FILE__"/Uint_Big_Standard_Deviation", "[Random][RandomInt]")
{
    // Setup
    std::vector<unsigned int> rands;
    rands.resize(1000);

    // Exercise
    // Create 1000 random values
    std::generate_n(rands.data(), rands.size(), []()->unsigned int { return Random::RandomUint(); });

    // Verify
    const double stddev = Testutil::Stddev<unsigned int>(rands);
    REQUIRE(stddev >= 1000000);

    return;
}

// Checks that the produced integer distribution shows a big standard deviation
TEST_CASE(__FILE__"/Int_Big_Standard_Deviation", "[Random][RandomInt]")
{
    // Setup
    std::vector<int> rands;
    rands.resize(1000);

    // Exercise
    // Create 1000 random values
    std::generate_n(rands.data(), rands.size(), []()->int { return Random::RandomInt(); });

    // Verify
    const double stddev = Testutil::Stddev<int>(rands);
    REQUIRE(stddev >= 1000000);

    return;
}
