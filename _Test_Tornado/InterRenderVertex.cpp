#include "../Tornado/InterRenderVertex.h"
#include "../_TestingUtilities/Catch2.h"
#include <random>

using namespace TorGL;

namespace {
    void InitA(InterRenderVertex& a)
    {
        a.pos_ws = { 0,0,0 };
        a.pos_wsmx = { 0,0,0 };
        a.pos_cs = { 0,0,0,0 };
        a.pos_ndc = { 0,0,0 };
        a.pos_ss = { 0,0,0 };
        a.pos_uv = { 0,0 };
        a.normal = { 0,0,0 };

        return;
    }

    void InitB(InterRenderVertex& b)
    {
        b.pos_ws = { 100,100,100 };
        b.pos_wsmx = { 100,100,100 };
        b.pos_cs = { 100,100,100,100 };
        b.pos_ndc = { 100,100,100 };
        b.pos_ss = { 100,100, 100 };
        b.pos_uv = { 100,100 };
        b.normal = { 100,100,100 };

        return;
    }

    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that nothing gets interpolated by default
TEST_CASE(__FILE__"Interpolates_Nothing_By_Default", "[InterRenderVertex]")
{
    InterRenderVertex a;
    InterRenderVertex b;
    InitA(a);
    InitB(b);

    InterRenderVertex aold = a;

    a.Interpolate(b, 0.25);

    REQUIRE(aold.pos_ws == a.pos_ws);
    REQUIRE(aold.pos_cs == a.pos_cs);
    REQUIRE(aold.pos_ndc == a.pos_ndc);
    REQUIRE(aold.pos_ss == a.pos_ss);
    REQUIRE(aold.pos_uv == a.pos_uv);
    REQUIRE(aold.normal == a.normal);

    return;
}

// Tests that only the correct attributes get interpolated
TEST_CASE(__FILE__"Interpolates_Correct_Attributes", "[InterRenderVertex]")
{
    // Run test 100 times

    for (std::size_t i = 0; i < 100; i++)
    {
        // Decide that to interpolate
        const bool lerp_ws = rng() % 2;
        const bool lerp_wsmx = rng() % 2;
        const bool lerp_cs = rng() % 2;
        const bool lerp_nd = rng() % 2;
        const bool lerp_ss = rng() % 2;
        const bool lerp_uv = rng() % 2;
        const bool lerp_nm = rng() % 2;

        // Generate interpolation mask
        long long mask = 0;
        if (lerp_ws)
            mask |= IRV_LERP_POS_WS;
        if (lerp_wsmx)
            mask |= IRV_LERP_POS_WSMX;
        if (lerp_cs)
            mask |= IRV_LERP_POS_CS;
        if (lerp_uv)
            mask |= IRV_LERP_POS_UV;
        if (lerp_nd)
            mask |= IRV_LERP_POS_NDC;
        if (lerp_ss)
            mask |= IRV_LERP_POS_SS;
        if (lerp_nm)
            mask |= IRV_LERP_NORMAL;

        // Create vertices
        InterRenderVertex a;
        InterRenderVertex b;
        InitA(a);
        InitB(b);
        InterRenderVertex aold = a;

        // Set interpolation mask
        a.SetInterpolationMask(mask);

        // Interpolate
        a.Interpolate(b, 0.25);

        // Check, if only the corresponding vertices have been modified
        if (lerp_ws)
            REQUIRE_FALSE(aold.pos_ws == a.pos_ws);
        else
            REQUIRE(aold.pos_ws == a.pos_ws);

        if (lerp_wsmx)
            REQUIRE_FALSE(aold.pos_wsmx == a.pos_wsmx);
        else
            REQUIRE(aold.pos_wsmx == a.pos_wsmx);

        if (lerp_cs)
            REQUIRE_FALSE(aold.pos_cs == a.pos_cs);
        else
            REQUIRE(aold.pos_cs == a.pos_cs);

        if (lerp_nd)
            REQUIRE_FALSE(aold.pos_ndc == a.pos_ndc);
        else
            REQUIRE(aold.pos_ndc == a.pos_ndc);

        if (lerp_ss)
            REQUIRE_FALSE(aold.pos_ss == a.pos_ss);
        else
            REQUIRE(aold.pos_ss == a.pos_ss);

        if (lerp_uv)
            REQUIRE_FALSE(aold.pos_uv == a.pos_uv);
        else
            REQUIRE(aold.pos_uv == a.pos_uv);

        if (lerp_nm)
            REQUIRE_FALSE(aold.normal == a.normal);
        else
            REQUIRE(aold.normal == a.normal);
    }

    return;
}

