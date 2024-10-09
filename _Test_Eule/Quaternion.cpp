#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../Eule/Quaternion.h"
#include <random>

using namespace Eule;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that if constructed with the default constructor, that all values are 0 (but w should be 1)
TEST_CASE(__FILE__"/Default_Constructor_All_0", "[Quaternion]")
{
    const Quaternion q;
    REQUIRE(Vector4d(0, 0, 0, 1) == q.GetRawValues());

    return;
}

// Tests that getting and setting raw values works
TEST_CASE(__FILE__"/Can_Set_Get_Raw_Values", "[Quaternion]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const Vector4d v(
            rng() % 90,
            rng() % 90,
            rng() % 90,
            rng() % 90
        );

        Quaternion q(Vector4d(0, 0, 0, 0)); // Garbage values

        q.SetRawValues(v);
        REQUIRE(v.Similar(q.GetRawValues()));
    }

    return;
}

// Tests that retrieving euler angles (without gimbal lock) results in the same values as put in
TEST_CASE(__FILE__"/To_Euler_From_Euler", "[Quaternion]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Create vector
        const Vector3d eul(
            rng() % 90,
            rng() % 90,
            rng() % 90
        );

        // Create quaternion from vector
        const Quaternion q(eul);

        // Create debug output
        INFO('\n'
            << "Actual vals: " << q.ToEulerAngles() << '\n'
            << "Target vals: " << eul << '\n'
        );

        // Assertion
        REQUIRE(eul.Similar(q.ToEulerAngles()));
    }

    return;
}

// Tests that adding angles (0,0,0) does not modify the quaternion
TEST_CASE(__FILE__"/Add_Angles_0_Does_Nothing", "[Quaternion]")
{
    const Quaternion a(Vector3d(0, -45, 45));
    const Quaternion b(Vector3d(0, 0, 0));

    REQUIRE(Vector3d(0, -45, 45).Similar((a * b).ToEulerAngles()));

    return;
}

// Tests that subtracting angles (0,0,0) does not modify the quaternion
TEST_CASE(__FILE__"/Sub_Angles_0_Does_Nothing", "[Quaternion]")
{
    const Quaternion a(Vector3d(0, -45, 45));
    const Quaternion b(Vector3d(0, 0, 0));

    REQUIRE(Vector3d(0, -45, 45).Similar((a / b).ToEulerAngles()));

    return;
}

// Tests that subtracting by itself always returns (0,0,0)
TEST_CASE(__FILE__"/Sub_Itself_Is_0", "[Quaternion]")
{
    // Run test 100 times
    for (std::size_t i = 0; i < 100; i++)
    {
        const Quaternion a(Vector3d(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE));
        REQUIRE(Vector3d(0,0,0).Similar((a / a).ToEulerAngles()));
    }

    return;
}

// Tests that rotating a vector is equal to multiplying it with the inverted rotation matrix
TEST_CASE(__FILE__"/RotateVector_Equal_to_RotationMatrix", "[Quaternion]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const Quaternion a(Vector3d(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE));

        const Vector3d point(32, 19, -14);

        // Generate debug output
        INFO((point * a.ToRotationMatrix()) << '\n' << "===" << (a * point) << '\n');

        REQUIRE((point * a.ToRotationMatrix()).Similar(a * point));
    }

    return;
}

// Tests that a *= b will result in the exact same outcome as a = a * b
TEST_CASE(__FILE__"/MultiplyEquals_Operator_Same_Result_As_Multiply_Operator", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        Quaternion a(Vector3d(rng() % 360, rng() % 360, rng() % 360));
        const Quaternion b(Vector3d(rng() % 360, rng() % 360, rng() % 360));

        // Exercise
        Quaternion ref = a * b;
        a *= b;

        // Verify
        REQUIRE(a.GetRawValues().Similar(ref.GetRawValues()));
    }

    return;
}

// Tests that a /= b will result in the exact same outcome as a = a / b
TEST_CASE(__FILE__"/DivideEquals_Operator_Same_Result_As_Divide_Operator", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        Quaternion a(Vector3d(rng() % 360, rng() % 360, rng() % 360));
        const Quaternion b(Vector3d(rng() % 360, rng() % 360, rng() % 360));

        // Exercise
        Quaternion ref = a / b;
        a /= b;

        // Verify
        REQUIRE(a.GetRawValues().Similar(ref.GetRawValues()));
    }

    return;
}

// Tests basic equals comparison -> true
TEST_CASE(__FILE__"/Basic_EqualsComparison_True", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        const Vector3d e(rng() % 360, rng() % 360, rng() % 360);
        const Quaternion a(e);
        const Quaternion b(e);

        // Exercise and verify
        REQUIRE(a == b);
    }

    return;
}

// Tests basic equals comparison -> true
TEST_CASE(__FILE__"/Basic_EqualsComparison_False", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        const Vector3d ae(rng() % 360, rng() % 360, rng() % 360);
        const Vector3d be(rng() % 360, rng() % 360, rng() % 360);

        // Abort if both vectors are equal
        if (ae == be)
        {
            i--;
            continue;
        }

        const Quaternion a(ae);
        const Quaternion b(be);

        // Exercise and verify
        REQUIRE_FALSE(a == b);
    }

    return;
}

// Tests that different euler angles return true, if the angle is the same.
// Like [30, -10, 59] == [390, 350, 419]
TEST_CASE(__FILE__"/Equals_Comparison_Same_Rotation_Different_EulerAngles", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        //    Create random rotation
        const Vector3d ae(rng() % 360, rng() % 360, rng() % 360);

        // add or subtract a random multiple of 360
        #define keep_rot_change_values (360.0 * (double)(rng() % 20) * ((rng()%2) ? 1.0 : -1.0))
        const Vector3d be(ae.x + keep_rot_change_values, ae.y + keep_rot_change_values, ae.z + keep_rot_change_values);
        #undef keep_rot_change_values

        // Create quaternions
        const Quaternion a(ae);
        const Quaternion b(be);

        // Exercise & Verify
        //    Create debug output

        INFO("ae: " << ae << '\n'
            << "be: " << be << '\n'
            << "a: " << a << '\n'
            << "b: " << b << '\n'
        );

        //    Assertion
        REQUIRE(a == b);
    }

    return;
}

// Tests basic not-equals comparison -> false
TEST_CASE(__FILE__"/Basic_NotEqualsComparison_False", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        const Vector3d e(rng() % 360, rng() % 360, rng() % 360);
        const Quaternion a(e);
        const Quaternion b(e);

        // Exercise and verify
        REQUIRE_FALSE(a != b);
    }

    return;
}

// Tests basic not-equals comparison -> true
TEST_CASE(__FILE__"/Basic_NotEqualsComparison_True", "[Quaternion]")
{
    // Run tests 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Setup
        const Vector3d ae(rng() % 360, rng() % 360, rng() % 360);
        const Vector3d be(rng() % 360, rng() % 360, rng() % 360);

        // Abort if both vectors are equal
        if (ae == be)
        {
            i--;
            continue;
        }

        const Quaternion a(ae);
        const Quaternion b(be);

        // Exercise and verify
        REQUIRE(a != b);
    }

    return;
}
