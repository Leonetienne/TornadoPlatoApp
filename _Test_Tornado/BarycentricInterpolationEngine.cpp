
#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../Tornado/BarycentricInterpolationEngine.h"
#include "../Eule/Math.h"
#include <random>
#include <sstream>
#include <chrono>

using namespace TorGL;
using Eule::Math;

namespace {
    InterRenderTriangle GetExampleTriangle()
    {
        InterRenderTriangle ird;

        // Triangle does not need to be clipped. Is known to be all visible
        ird.a.pos_cs = { -325, 0.5, 1.1, 512 };
        ird.b.pos_cs = { 412, 0.5, 1.1, 927 };
        ird.c.pos_cs = { 0, 0.5, 1.1, 561 };

        // Perspective division
        ird.a.pos_ndc = (ird.a.pos_cs / ird.a.pos_cs.w);
        ird.b.pos_ndc = (ird.b.pos_cs / ird.b.pos_cs.w);
        ird.c.pos_ndc = (ird.c.pos_cs / ird.c.pos_cs.w);

        // Transform to screen space
        ird.a.pos_ss = ird.a.pos_ndc * 400 + Vector2d(400, 400);
        ird.b.pos_ss = ird.b.pos_ndc * 400 + Vector2d(400, 400);
        ird.c.pos_ss = ird.c.pos_ndc * 400 + Vector2d(400, 400);

        return ird;
    }
}

// Tests that interpolation to a vertex does return that vertex's value
TEST_CASE("Corners Remain Same", "[BarycentricInterpolation][Engine]")
{
    InterRenderTriangle ird = GetExampleTriangle();
    std::mt19937 rng((std::random_device())());

    // Run interpolation test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        // Create vertex values
        double a = LARGE_RAND_DOUBLE;
        double b = LARGE_RAND_DOUBLE;
        double c = LARGE_RAND_DOUBLE;

        // Interpolate value at position of a between a, b, and c
        double ret = BarycentricInterpolationEngine::PerspectiveCorrected(
            ird,
            ird.a.pos_ss,
            a,
            b,
            c
        );
        // Check the interpolated value is the exact value at a
        REQUIRE(Math::Similar(a, ret));

        // Interpolate value at position of b between a, b, and c
        ret = BarycentricInterpolationEngine::PerspectiveCorrected(
            ird,
            ird.b.pos_ss,
            a,
            b,
            c
        );
        // Check the interpolated value is the exact value at b
        REQUIRE(Math::Similar(b, ret));

        // Interpolate value at position of c between a, b, and c
        ret = BarycentricInterpolationEngine::PerspectiveCorrected(
            ird,
            ird.c.pos_ss,
            a,
            b,
            c
        );
        // Check the interpolated value is the exact value at c
        REQUIRE(Math::Similar(c, ret));
    }
}

// Tests that using the cached variant is faster when interpolating multiple attributes per pixel
TEST_CASE("Cached Values Are Faster", "[BarycentricInterpolation][Engine]")
{
    InterRenderTriangle ird = GetExampleTriangle();
    std::mt19937 rng((std::random_device())());

    // Benchmark without caching
    std::chrono::nanoseconds nocache;
    {
        auto begin = std::chrono::high_resolution_clock::now();

        // Interpolate many attributes on the same pixel
        for (std::size_t i = 0; i < 10000; i++)
        {
            // Don't optimize shit
            volatile double a = LARGE_RAND_DOUBLE;
            volatile double b = LARGE_RAND_DOUBLE;
            volatile double c = LARGE_RAND_DOUBLE;
            volatile double ret = BarycentricInterpolationEngine::PerspectiveCorrected(
                ird,
                ird.a.pos_ss,
                a,
                b,
                c
            );
        }

        auto end = std::chrono::high_resolution_clock::now();
        nocache = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    }

    // Benchmark with caching
    std::chrono::nanoseconds cached;
    {
        auto begin = std::chrono::high_resolution_clock::now();

        // Interpolate many attributes on the same pixel
        std::array<double, 5> cache { 0 };
        for (std::size_t i = 0; i < 10000; i++)
        {
            // Don't optimize shit
            volatile double a = LARGE_RAND_DOUBLE;
            volatile double b = LARGE_RAND_DOUBLE;
            volatile double c = LARGE_RAND_DOUBLE;
            volatile double ret = BarycentricInterpolationEngine::PerspectiveCorrect__CachedValues(
                ird,
                ird.a.pos_ss,
                a,
                b,
                c,
                &cache
            );
        }

        auto end = std::chrono::high_resolution_clock::now();
        cached = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    }

    std::wstringstream wss;
    wss << L"Wtf, no cached values was faster than with cached values! NoCache: " << nocache.count() << L"ns | Cached: " << cached.count() << L"ns.";
    REQUIRE(nocache > cached);
}

