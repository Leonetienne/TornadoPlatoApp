#include "../_TestingUtilities/Catch2.h"
#include "../Tornado/Color.h"
#include <sstream>

using namespace TorGL;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

TEST_CASE(__FILE__"/Default_Constructor_All_0", "[Color]")
{
    // Test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        Color color;
        REQUIRE(color.r == 0);
        REQUIRE(color.g == 0);
        REQUIRE(color.b == 0);
        REQUIRE(color.a == 0);
    }
}

TEST_CASE("Color_Does_Match_Constructor_Without_Alpha", "[Color]")
{
    // Test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const double r = rng() % 255;
        const double g = rng() % 255;
        const double b = rng() % 255;

        Color col(r, g, b);
        REQUIRE(col.r == r);
        REQUIRE(col.g == g);
        REQUIRE(col.b == b);
        REQUIRE(col.a == 255);
    }
}

TEST_CASE("Color_Does_Match_Constructor", "[Color]")
{
    // Test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const double r = rng() % 255;
        const double g = rng() % 255;
        const double b = rng() % 255;
        const double a = rng() % 255;

        Color col(r, g, b, a);
        REQUIRE(col.r == r);
        REQUIRE(col.g == g);
        REQUIRE(col.b == b);
        REQUIRE(col.a == a);
    }
}

TEST_CASE("Lerp_Expected_000", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);
    REQUIRE(a == Color::Lerp(a, b, 0.00));
}

TEST_CASE("Lerp_Expected_025", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);

    Color res = Color::Lerp(a, b, 0.25);
    std::ostringstream oss;
    oss << res;
    REQUIRE(Color(25, 175, 62.5, 50) == res);
}

TEST_CASE("Lerp_Expected_050", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);

    Color res = Color::Lerp(a, b, 0.50);
    std::ostringstream oss;
    oss << res;
    REQUIRE(Color(50, 150, 75, 100) == res);
}

TEST_CASE("Lerp_Expected_075", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);

    Color res = Color::Lerp(a, b, 0.75);
    std::ostringstream oss;
    oss << res;
    REQUIRE(Color(75, 125, 87.5, 150) == res);
}

TEST_CASE("Lerp_Expected_100", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);
    REQUIRE(b == Color::Lerp(a, b, 1.00));
}

TEST_CASE("Lerp_Memberfunction", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);
    REQUIRE(Color(75, 125, 87.5, 150) == a.Lerp(b, 0.75));
}

TEST_CASE("LerpSelf", "[Color]")
{
    Color a(0, 200, 50, 0);
    Color b(100, 100, 100, 200);
    a.LerpSelf(b, 0.75);
    REQUIRE(Color(75, 125, 87.5, 150) == a);
}

TEST_CASE("Operator_Equals", "[Color]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ar = rng() % 5;
        const double ag = rng() % 5;
        const double ab = rng() % 5;
        const double aa = rng() % 5;
        const double br = rng() % 5;
        const double bg = rng() % 5;
        const double bb = rng() % 5;
        const double ba = rng() % 5;

        const Color a(ar, ag, ab, aa);
        const Color b(br, bg, bb, ba);

        // If all color values are equal, the color objects should be equal aswell
        if ((ar == br) && (ag == bg) && (ab == bb) && (aa == ba)) {
            REQUIRE(a == b);
        }
    }
}

TEST_CASE("Operator_NotEquals", "[Color]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ar = rng() % 5;
        const double ag = rng() % 5;
        const double ab = rng() % 5;
        const double aa = rng() % 5;
        const double br = rng() % 5;
        const double bg = rng() % 5;
        const double bb = rng() % 5;
        const double ba = rng() % 5;

        const Color a(ar, ag, ab, aa);
        const Color b(br, bg, bb, ba);

        // If any color value is uneuqal, the color bject should also be unequal
        if ((ar != br) || (ag != bg) || (ab != bb) || (aa != ba)) {
            REQUIRE(a != b);
        }
    }
}

TEST_CASE("Constants", "[Color]")
{
    REQUIRE(Color(0, 0, 0, 255) == Color::black);
    REQUIRE(Color(128, 128, 128, 255) == Color::gray);
    REQUIRE(Color(255, 0, 0, 255) == Color::red);
    REQUIRE(Color(0, 255, 0, 255) == Color::green);
    REQUIRE(Color(0, 0, 255, 255) == Color::blue);
    REQUIRE(Color(255, 255, 0, 255) == Color::yellow);
    REQUIRE(Color(0, 255, 255, 255) == Color::cyan);
    REQUIRE(Color(255, 0, 255, 255) == Color::pink);
    REQUIRE(Color(255, 255, 255, 255) == Color::white);
}

