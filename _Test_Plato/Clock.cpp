#include "../_TestingUtilities/Catch2.h"
#include "../Plato/Clock.h"
#include "../Plato/Math.h"
#include <random>
#include <thread>
#include <chrono>

using namespace Plato;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
    typedef std::chrono::milliseconds ms;
}

// Tests that a new clock has as elapsed time of ~0
TEST_CASE(__FILE__"/New_Clock_Elapsed_Time_0", "[Clock]")
{
    // Setup
    Clock clock;

    // Excersise
    const double se = clock.GetElapsedTime().AsSeconds();

    // Verify
    REQUIRE(Math::Similar(se, 0));
    return;
}

// Tests that a clock started before a Sleep() matches the sleep duration
TEST_CASE(__FILE__"/Elapsed_Time_Matches_Sleep", "[Clock]")
{
    // Setup
    Clock clock;

    // Excersise
    std::this_thread::sleep_for(ms(269));
    const double se = clock.GetElapsedTime().AsSeconds();

    // Verify
    REQUIRE(Math::Similar(se, 269e-3, 10));
    return;
}

// Tests that the elapsed time is 0 directly after resetting the clock
TEST_CASE(__FILE__"/Elapsed_Time_Is_0_After_Reset", "[Clock]")
{
    // Setup
    Clock clock;

    // Excersise
    std::this_thread::sleep_for(ms(269));
    clock.Reset();
    const double se = clock.GetElapsedTime().AsSeconds();

    // Verify
    REQUIRE(Math::Similar(se, 0));

    return;
}

// Tests that the measurements between seconds, milliseconds and microseconds are accurate
TEST_CASE(__FILE__"/Unit_Conversion_Works", "[Clock]")
{
    // Setup
    Clock clock;

    // Excersise
    std::this_thread::sleep_for(ms(2069)); // Sleep relatively long because we are going to compare SECONDS to NANOSECONDS! Yikes! We're going to need some big values for this!
    Clock::Duration dur = clock.GetElapsedTime(); // We have to cache the duration object, because fetching the individual durations would cost time itself, influencing the test result...

    const double se = dur.AsSeconds();
    const double ms = dur.AsMilliseconds();
    const double us = dur.AsMicroseconds();
    const double ns = dur.AsNanoseconds();

    // Verify
    REQUIRE(Math::Similar(2069e-3, se, 10e-3));
    REQUIRE(Math::Similar(2069e+0, ms, 10e+0));
    REQUIRE(Math::Similar(2069e+3, us, 10e+3));
    REQUIRE(Math::Similar(2069e+6, ns, 10e+6));
    return;
}
