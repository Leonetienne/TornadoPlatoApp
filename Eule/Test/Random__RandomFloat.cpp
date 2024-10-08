#include "Catch2.h"
#include <Eule/Random.h>

using namespace Eule;

// Checks that all values are always 0 <= v <= 1
TEST_CASE(__FILE__"/Always_Satisfies_0_lt_v_lt_1", "[Random][RandomFloat]")
{
    // Test 1000 random values
    for (std::size_t i = 0; i < 1e3; i++)
    {
        const double rnd = Random::RandomFloat();
        INFO("rnd < 0");
        REQUIRE(rnd >= 0.0);

        INFO("rnd > 1");
        REQUIRE(rnd <= 1.0);
    }

    return;
}
