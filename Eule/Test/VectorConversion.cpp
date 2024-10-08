#include "Catch2.h"
#include <Eule/Vector2.h>
#include <Eule/Vector3.h>
#include <Eule/Vector4.h>
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Eule;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that conversion vector2 -> vector3 works
TEST_CASE(__FILE__"/Convert_Vector2_To_Vector3", "[Vector][VectorConversion]")
{
    // Run test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector2d v2(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector3d v3 = v2;

        REQUIRE(v2.x == v3.x);
        REQUIRE(v2.y == v3.y);
        REQUIRE(0.0  == v3.z);
    }

    return;
}

// Tests that conversion vector2 -> vector4 works
TEST_CASE(__FILE__"/Convert_Vector2_To_Vector4", "[Vector][VectorConversion]")
{
    // Run test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector2d v2(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector4d v4 = v2;

        REQUIRE(v2.x == v4.x);
        REQUIRE(v2.y == v4.y);
        REQUIRE(0.0  == v4.z);
        REQUIRE(0.0  == v4.w);
    }

    return;
}

// Tests that conversion vector3 -> vector2 works
TEST_CASE(__FILE__"/Convert_Vector3_To_Vector2", "[Vector][VectorConversion]")
{
    // Run test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector3d v3(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector2d v2 = v3;

        REQUIRE(v3.x == v2.x);
        REQUIRE(v3.y == v2.y);
    }

    return;
}

// Tests that conversion vector3 -> vector4 works
TEST_CASE(__FILE__"/Convert_Vector3_To_Vector4", "[Vector][VectorConversion]")
{
    // Run test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector3d v3(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector4d v4 = v3;

        REQUIRE(v3.x == v4.x);
        REQUIRE(v3.y == v4.y);
        REQUIRE(v3.z == v4.z);
        REQUIRE(0.0  == v4.w);
    }

    return;
}

// Tests that conversion vector4 -> vector42 works
TEST_CASE(__FILE__"/Convert_Vector4_To_Vector2", "[Vector][VectorConversion]")
{
    // Run tests 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector4d v4(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector2d v2 = v4;

        REQUIRE(v4.x == v2.x);
        REQUIRE(v4.y == v2.y);
    }

    return;
}

// Tests that conversion vector4 -> vector3 works
TEST_CASE(__FILE__"/Convert_Vector4_To_Vector3", "[Vector][VectorConversion]")
{
    // Run tests 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Vector4d v4(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);
        const Vector3d v3 = v4;

        REQUIRE(v4.x == v3.x);
        REQUIRE(v4.y == v3.y);
        REQUIRE(v4.z == v3.z);
    }

    return;
}

// Tests Vector2i -> Vector2d
TEST_CASE(__FILE__"/Convert_Vector2i_To_Vector2d", "[Vector][VectorConversion]")
{
    // Setup
    const Vector2i vi(69, 70);

    // Exercise
    const Vector2d vd = vi.ToDouble();

    // Verify
    REQUIRE(Vector2d(69, 70) == vd);

    return;
}

// Tests Vector2d -> Vector2i
TEST_CASE(__FILE__"/Convert_Vector2d_To_Vector2i", "[Vector][VectorConversion]")
{
    // Setup
    const Vector2d vd(69.2, 70.8);

    // Exercise
    const Vector2i vi = vd.ToInt();

    // Verify
    REQUIRE(Vector2i(69, 70) == vi);

    return;
}

// Tests Vector3i -> Vector3d
TEST_CASE(__FILE__"/Convert_Vector3i_To_Vector3d", "[Vector][VectorConversion]")
{
    // Setup
    const Vector3i vi(69, 70, 122);

    // Exercise
    const Vector3d vd = vi.ToDouble();

    // Verify
    REQUIRE(Vector3d(69, 70, 122) == vd);

    return;
}

// Tests Vector3d -> Vector3i
TEST_CASE(__FILE__"/Convert_Vector3d_To_Vector3i", "[Vector][VectorConversion]")
{
    // Setup
    const Vector3d vd(69.2, 70.8, 122);

    // Exercise
    const Vector3i vi = vd.ToInt();

    // Verify
    REQUIRE(Vector3i(69, 70, 122) == vi);

    return;
}

// Tests Vector4i -> Vector4d
TEST_CASE(__FILE__"/Convert_Vector4i_To_Vector4d", "[Vector][VectorConversion]")
{
    // Setup
    const Vector4i vi(69, 70, 122, 199);

    // Exercise
    const Vector4d vd = vi.ToDouble();

    // Verify
    REQUIRE(Vector4d(69, 70, 122, 199) == vd);

    return;
}

// Tests Vector4d -> Vector4i
TEST_CASE(__FILE__"/Convert_Vector4d_To_Vector4i", "[Vector][VectorConversion]")
{
    // Setup
    const Vector4d vd(69.2, 70.8, 122, 199.501);

    // Exercise
    const Vector4i vi = vd.ToInt();

    // Verify
    REQUIRE(Vector4i(69, 70, 122, 199) == vi);

    return;
}
