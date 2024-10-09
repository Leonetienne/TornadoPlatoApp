#include "../_TestingUtilities/Catch2.h"
#include "../Eule/Vector3.h"
#include "../Eule/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>

using namespace Eule;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests if all values are 0 after initialization via default constructor
TEST_CASE(__FILE__"/New_Vector_All_0", "[Vector][Vector3]")
{
    Vector3d v3;

    REQUIRE(0.0 == v3.x);
    REQUIRE(0.0 == v3.y);
    REQUIRE(0.0 == v3.z);

    return;
}

// Tests if values can be set via the constructor
TEST_CASE(__FILE__"/Can_Set_Values_Constructor", "[Vector][Vector3]")
{
    Vector3d v3(69, 32, 16);

    REQUIRE(69. == v3.x);
    REQUIRE(32. == v3.y);
    REQUIRE(16. == v3.z);

    return;
}

// Tests if values can be set via letters
TEST_CASE(__FILE__"/Can_Set_Values_Letters", "[Vector][Vector3]")
{
    Vector3d v3;
    v3.x = 69;
    v3.y = 32;
    v3.z = 16;

    REQUIRE(69. == v3.x);
    REQUIRE(32. == v3.y);
    REQUIRE(16. == v3.z);

    return;
}

// Tests if values can be set via array descriptors
TEST_CASE(__FILE__"/Can_Set_Values_ArrayDescriptor", "[Vector][Vector3]")
{
    Vector3d v3;
    v3[0] = 69;
    v3[1] = 32;
    v3[2] = 16;

    REQUIRE(69. == v3.x);
    REQUIRE(32. == v3.y);
    REQUIRE(16. == v3.z);

    return;
}

// Tests if values can be set via an initializer list
TEST_CASE(__FILE__"/Can_Set_Values_InitializerList", "[Vector][Vector3]")
{
    Vector3d v3 = { 69, 32, 16 };

    REQUIRE(69. == v3.x);
    REQUIRE(32. == v3.y);
    REQUIRE(16. == v3.z);

    return;
}

// Tests for vectors copied via the copy constructor to have the same values
TEST_CASE(__FILE__"/Copy_Constructor_Same_Values", "[Vector][Vector3]")
{
    Vector3d a(69, 32, 16);
    Vector3d b(a);

    REQUIRE(a.x == b.x);
    REQUIRE(a.y == b.y);
    REQUIRE(a.z == b.z);

    return;
}

// Tests for vectors copied via the equals operator to have the same values
TEST_CASE(__FILE__"/Operator_Equals_Same_Values", "[Vector][Vector3]")
{
    Vector3d a(69, 32, 16);
    Vector3d b = a;

    REQUIRE(a.x == b.x);
    REQUIRE(a.y == b.y);
    REQUIRE(a.z == b.z);

    return;
}

// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Copy_Constructor_Independent", "[Vector][Vector3]")
{
    Vector3d a(69, 32, 16);
    Vector3d b(a);

    b.x = 169;
    b.y = 132;
    b.z = 116;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);
    REQUIRE(16.0 == a.z);

    REQUIRE(169 == b.x);
    REQUIRE(132 == b.y);
    REQUIRE(116 == b.z);

    return;
}

// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Operator_Equals_Independent", "[Vector][Vector3]")
{
    Vector3d a(69, 32, 16);
    Vector3d b = a;

    b.x = 169;
    b.y = 132;
    b.z = 116;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);
    REQUIRE(16.0 == a.z);

    REQUIRE(169 == b.x);
    REQUIRE(132 == b.y);
    REQUIRE(116 == b.z);

    return;
}

// Tests if the dot product between two vectors angled 90 degrees from one another is 0. It should by definition be 0!
// Dot products are commutative, so we'll check both directions.
// This test tests all possible 90 degree setups with 1000x random lengths
TEST_CASE(__FILE__"/DotProduct_90deg", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 100; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        Vector3d a = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
        Vector3d b = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);

        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));

        a = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
        b = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));

        a = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
        b = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));

        a = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
        b = Vector3d(0, 1, 0) * (rng() % 6969 + 1.0);
        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));

        a = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
        b = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));

        a = Vector3d(0, 0, 1) * (rng() % 6969 + 1.0);
        b = Vector3d(1, 0, 0) * (rng() % 6969 + 1.0);
        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));
    }

    return;
}

// Test if the dot product is positive for two vectors angled less than 90 degrees from another
// Dot products are commutative, so we'll check both directions.
TEST_CASE(__FILE__"/DotProduct_LessThan90deg", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        Vector3d a = Vector3d(1, 1.0 / (rng() % 100), 69) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
        Vector3d b = Vector3d(1.0 / (rng() % 100), 1, 69) * (rng() % 6969 + 1.0);


        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(a.DotProduct(b) > 0);
        REQUIRE(b.DotProduct(a) > 0);
    }

    return;
}

// Test if the dot product is negative for two vectors angled greater than 90 degrees from another
// Dot products are commutative, so we'll check both directions.
TEST_CASE(__FILE__"/DotProduct_GreaterThan90deg", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        Vector3d a = Vector3d(1, -1.0 / (rng() % 100), 69) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
        Vector3d b = Vector3d(-1.0 / (rng() % 100), 1, -69) * (rng() % 6969 + 1.0);

        INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
        REQUIRE(a.DotProduct(b) < 0);
        REQUIRE(b.DotProduct(a) < 0);
    }

    return;
}

// Tests that the dot product is correct for a known value
TEST_CASE(__FILE__"/DotProduct_Oracle", "[Vector][Vector3]")
{
    // Setup
    Vector3d a(-99, 199, -32);
    Vector3d b(18, -1, -21);

    // Exercise
    const double dot = a.DotProduct(b);

    // Verify
    REQUIRE(-1309.0 == dot);

    return;
}

// Quick and dirty check if the useless int-method is working
TEST_CASE(__FILE__"/DotProduct_Dirty_Int", "[Vector][Vector3]")
{
    Vector3i a;
    Vector3i b;

    // 90 deg
    a = { 0, 10, 0 };
    b = { 10, 0, 0 };
    INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
    REQUIRE(0.0 == a.DotProduct(b));
    REQUIRE(0.0 == b.DotProduct(a));

    // < 90 deg
    a = { 7, 10, 10 };
    b = { 10, 1, 10 };
    INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
    REQUIRE(a.DotProduct(b) > 0.0);
    REQUIRE(b.DotProduct(a) > 0.0);

    // > 90 deg
    a = { -3, 10, -10 };
    b = { 10, -4,  10 };
    INFO(a << " DOT " << b << " = " << a.DotProduct(b) << '\n');
    REQUIRE(a.DotProduct(b) < 0.0);
    REQUIRE(b.DotProduct(a) < 0.0);

    return;
}

// Tests for the cross product between the same vector being 0
TEST_CASE(__FILE__"/CrossProduct_Same_Vector_Is_0", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_DOUBLE;
        double z = LARGE_RAND_DOUBLE;

        Vector3d a(x, y, z);

        INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
        REQUIRE(Vector3d(0,0,0).Similar(a.CrossProduct(a), 0.01));
    }

    return;
}

// Tests for the cross product between opposite vectors being 0
TEST_CASE(__FILE__"/CrossProduct_Opposite_Vector_Is_0", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_DOUBLE;
        double z = LARGE_RAND_DOUBLE;

        Vector3d a(x, y, z);
        Vector3d b(-x, -y, -z);

        INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
        REQUIRE(Vector3d(0,0,0).Similar(a.CrossProduct(b), 0.01));
    }

    return;
}

// Tests for known values
TEST_CASE(__FILE__"/CrossProduct_KnownValues", "[Vector][Vector3]")
{
    Vector3d a;
    Vector3d b;

    a = Vector3d(1, 0, 0);
    b = Vector3d(0, 0, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3d(-1, 0, 0);
    b = Vector3d(0, 0, -1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3d(1, 0, 0);
    b = Vector3d(0, 0, -1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, 1, 0) == a.CrossProduct(b));

    a = Vector3d(1, 0, 0);
    b = Vector3d(0, 1, 0);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, 0, 1) == a.CrossProduct(b));

    a = Vector3d(1, 0, 0);
    b = Vector3d(1, 0, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3d(1, 0, 0);
    b = Vector3d(1, 1, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 1) == a.CrossProduct(b));

    a = Vector3d(3, -1, -3);
    b = Vector3d(-1, 1, 3);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -6, 2) == a.CrossProduct(b));

    return;
}

// Tests for known values, but with int vectors
TEST_CASE(__FILE__"/CrossProduct_KnownValues_Int", "[Vector][Vector3]")
{
    Vector3i a;
    Vector3i b;

    a = Vector3i(1, 0, 0);
    b = Vector3i(0, 0, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3i(-1, 0, 0);
    b = Vector3i(0, 0, -1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3i(1, 0, 0);
    b = Vector3i(0, 0, -1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, 1, 0) == a.CrossProduct(b));

    a = Vector3i(1, 0, 0);
    b = Vector3i(0, 1, 0);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, 0, 1) == a.CrossProduct(b));

    a = Vector3i(1, 0, 0);
    b = Vector3i(1, 0, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 0) == a.CrossProduct(b));

    a = Vector3i(1, 0, 0);
    b = Vector3i(1, 1, 1);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -1, 1) == a.CrossProduct(b));

    a = Vector3i(3, -1, -3);
    b = Vector3i(-1, 1, 3);
    INFO(a << L" CROSS " << a << L" = " << a.CrossProduct(a) << '\n');
    REQUIRE(Vector3d(0, -6, 2) == a.CrossProduct(b));

    return;
}

// Tests the SqrMagnitude method to work as expected with random numbers
TEST_CASE(__FILE__"/SqrMagnitude", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = (double)(rng() % 1000) - 500.0; // Too large numbers would get inaccurate decimals when using intrinsics.
        double y = (double)(rng() % 1000) - 500.0;
        double z = (double)(rng() % 1000) - 500.0;
        double expected = x*x + y*y + z*z;

        REQUIRE(Math::Similar(expected, Vector3d(x, y, z).SqrMagnitude()));
    }

    return;
}

// Tests the SqrMagnitude method to work as expected with random numbers, but with an int-vector
TEST_CASE(__FILE__"/SqrMagnitude_Int", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const int x = LARGE_RAND_INT;
        const int y = LARGE_RAND_INT;
        const int z = LARGE_RAND_INT;
        const int expected = x*x + y*y + z*z;

        REQUIRE(expected == Vector3i(x, y, z).SqrMagnitude());
    }

    return;
}

// Tests for the length of the vector (0,0,0) being 0
TEST_CASE(__FILE__"/Magnitude_Is_0_On_Vec0", "[Vector][Vector3]")
{
    REQUIRE(0.0 == Vector3d(0, 0, 0).Magnitude());
    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_X", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = (double)(rng() % 1000) - 500.0; // Too large numbers would get inaccurate decimals when using intrinsics.
        Vector3d vec(x, 0, 0);

        INFO('\n'
            << "Actual: " << vec.Magnitude() << '\n'
            << "Expected: " << x << '\n'
        );
        REQUIRE(Math::Similar(abs(x), vec.Magnitude()));
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_Y", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double y = (double)(rng() % 1000) - 500.0; // Too large numbers would get inaccurate decimals when using intrinsics.
        Vector3d vec(0, y, 0);

        INFO('\n'
            << "Actual: " << vec.Magnitude() << '\n'
            << "Expected: " << y << '\n'
        );
        REQUIRE(Math::Similar(abs(y), vec.Magnitude()));
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_Z", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double z = (double)(rng() % 1000) - 500.0; // Too large numbers would get inaccurate decimals when using intrinsics.
        Vector3d vec(0, 0, z);

        INFO('\n'
            << "Actual: " << vec.Magnitude() << '\n'
            << "Expected: " << z << '\n'
        );
        REQUIRE(Math::Similar(abs(z), vec.Magnitude()));
    }

    return;
}

// Tests for a known result
TEST_CASE(__FILE__"/Magnitude", "[Vector][Vector3]")
{
    // Ya'll got more of 'dem digits?
    REQUIRE(426.14786166306174663986894302070140838623046875 == Vector3d(69, -420, 21).Magnitude());
    return;
}

// Tests for expected lerp result 0.00
TEST_CASE(__FILE__"/Lerp_000", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);
    Vector3d res = a.Lerp(b, 0.00);

    INFO(res);
    REQUIRE(a == res);
    return;
}

// Tests for expected lerp result 0.25
TEST_CASE(__FILE__"/Lerp_025", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);
    Vector3d res = a.Lerp(b, 0.25);

    INFO(res);
    REQUIRE(Vector3d(125, 1750, 32.5) == res);
    return;
}

// Tests for expected lerp result 0.50
TEST_CASE(__FILE__"/Lerp_050", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);
    Vector3d res = a.Lerp(b, 0.50);

    INFO(res);
    REQUIRE(Vector3d(150, 2500, 55) == res);
    return;
}

// Tests for expected lerp result 0.75
TEST_CASE(__FILE__"/Lerp_075", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);
    Vector3d res = a.Lerp(b, 0.75);

    INFO(res);
    REQUIRE(Vector3d(175, 3250, 77.5) == res);
    return;
}

// Tests for expected lerp result 1.00
TEST_CASE(__FILE__"/Lerp_100", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);
    Vector3d res = a.Lerp(b, 1.00);

    INFO(res);
    REQUIRE(b == res);
    return;
}

// Tests lerpself
TEST_CASE(__FILE__"/LerpSelf", "[Vector][Vector3]")
{
    Vector3d a(100, 1000, 10);
    Vector3d b(200, 4000, 100);

    a.LerpSelf(b, 0.75);

    INFO(a);
    REQUIRE(Vector3d(175, 3250, 77.5) == a);
    return;
}

// Tests if an input vector of length 0 is handled correctly by the normalize method
TEST_CASE(__FILE__"/Normalize_Length_Before_Is_0", "[Vector][Vector3]")
{
    Vector3d vec(0, 0, 0);
    vec.NormalizeSelf();
    REQUIRE(0.0 == vec.Magnitude());
    return;
}

// Tests for any normalized vector to be of length 1
TEST_CASE(__FILE__"/Normalize_Length_Is_1", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_DOUBLE;
        double z = LARGE_RAND_DOUBLE;

        Vector3d vec(x, y, z);

        // Prevent a vector of length 0 going in
        if (vec.SqrMagnitude() == 0)
            vec.x++;

        INFO(vec);
        REQUIRE(Math::Similar(vec.Normalize().Magnitude(), 1.0)); // Account for floating point inaccuracy
    }

    return;
}

// Tests the normalize method with known values
TEST_CASE(__FILE__"/Normalize_Oracle", "[Vector][Vector3]")
{
    // Setup
    Vector3d v(3.2, -5.3, 9.88);

    // Exercise
    v.NormalizeSelf();

    // Verify
    Vector3d expected(0.27445384355, -0.45456417839, 0.84737624198);
    REQUIRE(v.Similar(expected));
}

// Tests for a normalized vector to still point in the exact same direction
TEST_CASE(__FILE__"/Normalize_Direction_Stays_Unaffected", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_DOUBLE;
        double z = LARGE_RAND_DOUBLE;
        if (x == 0) x++;
        if (y == 0) y++;
        if (z == 0) z++;

        Vector3d vec(x, y, z);

        Vector3d vec_n(x, y, z);
        vec_n = vec_n.Normalize();

        INFO(vec << L" | " << vec_n);

        // Both vectors should still point in the same direction!
        REQUIRE((
            (vec.DotProduct(vec_n) > 0) && // Roughly same direction
            (Math::Similar(vec_n.CrossProduct(vec).Magnitude(), 0.0)) // Both vectors align
        ));
    }
    return;
}

// Kinda dumb method, but ok lol
// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
TEST_CASE(__FILE__"/Normalized_Int_Vector_Is_0", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const int x = LARGE_RAND_INT;
        const int y = LARGE_RAND_INT;
        const int z = LARGE_RAND_INT;

        Vector3i vec(x, y, z);

        vec.NormalizeSelf();

        INFO(vec);
        REQUIRE(0.0 == vec.Magnitude());
    }
}

// Tests that NormalizeSelf() results in the same as Normalize()
TEST_CASE(__FILE__"/NormalizeSelf_IsSameAs_Normalize", "[Vector][Vector3]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        Vector3d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);

        Vector3d nVec = vec.Normalize();
        vec.NormalizeSelf();

        REQUIRE(nVec == vec);
    }

    return;
}

// Tests for the VectorScale() method to work
TEST_CASE(__FILE__"/VectorScale", "[Vector][Vector3]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;

        const Vector3d a(ax, ay, az);
        const Vector3d b(bx, by, bz);

        Vector3d target(
            ax * bx,
            ay * by,
            az * bz
        );

        REQUIRE(a.VectorScale(b) == target);
    }

    return;
}

// Tests for operator- (unary) to work
TEST_CASE(__FILE__"/Operator_Unary_Negative", "[Vector][Vector3]")
{
    const Vector3d v(29, -5, 35);

    REQUIRE(Vector3d(-29, 5, -35) == -v);

    return;
}

// Tests for operator+ to work as expected
TEST_CASE(__FILE__"/Operator_Add", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;

        const Vector3d a(ax, ay, az);
        const Vector3d b(bx, by, bz);

        REQUIRE(Vector3d(ax + bx, ay + by, az + bz) == a + b);
    }

    return;
}

// Tests for operator+= to work as expected
TEST_CASE(__FILE__"/Operator_Add_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;

        Vector3d a(ax, ay, az);
        a += Vector3d(bx, by, bz);

        REQUIRE(Vector3d(ax + bx, ay + by, az + bz) == a);
    }

    return;
}

// Tests for operator- to work as expected
TEST_CASE(__FILE__"/Operator_Sub", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;

        const Vector3d a(ax, ay, az);
        const Vector3d b(bx, by, bz);

        REQUIRE(Vector3d(ax - bx, ay - by, az - bz) == a - b);
    }

    return;
}

// Tests for operator-= to work as expected
TEST_CASE(__FILE__"/Operator_Sub_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;

        Vector3d a(ax, ay, az);
        a -= Vector3d(bx, by, bz);

        REQUIRE(Vector3d(ax - bx, ay - by, az - bz) == a);
    }

    return;
}

// Tests for operator* to work as expected
TEST_CASE(__FILE__"/Operator_Mult", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector3d a(x, y, z);

        REQUIRE(Vector3d(x * scalar, y * scalar, z * scalar) == a * scalar);
    }

    return;
}

// Tests for operator*= to work as expected
TEST_CASE(__FILE__"/Operator_Mult_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector3d a(x, y, z);
        a *= scalar;

        REQUIRE(Vector3d(x * scalar, y * scalar, z * scalar) == a);
    }

    return;
}

// Tests for operator/ to work as expected
TEST_CASE(__FILE__"/Operator_Div", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector3d a(x, y, z);

        REQUIRE(Vector3d(x / scalar, y / scalar, z / scalar) == a / scalar);
    }

    return;
}

// Tests for operator/= to work as expected
TEST_CASE(__FILE__"/Operator_Div_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector3d a(x, y, z);
        a /= scalar;

        REQUIRE(Vector3d(x / scalar, y / scalar, z / scalar) == a);
    }

    return;
}

// Tests for operator== to work as expected
TEST_CASE(__FILE__"/Operator_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 10000; i++)
    {
        const double ax = (rng() % 10) - 5;
        const double ay = (rng() % 10) - 5;
        const double az = (rng() % 10) - 5;
        const double bx = (rng() % 10) - 5;
        const double by = (rng() % 10) - 5;
        const double bz = (rng() % 10) - 5;

        const Vector3d a(ax, ay, az);
        const Vector3d b(bx, by, bz);

        REQUIRE(
            ((ax == bx) && (ay == by) && (az == bz)) ==
            (a == b)
        );
    }

    return;
}

// Tests for operator!= to work as expected
TEST_CASE(__FILE__"/Operator_Not_Equals", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 10000; i++)
    {
        double ax = (rng() % 10) - 5;
        double ay = (rng() % 10) - 5;
        double az = (rng() % 10) - 5;
        double bx = (rng() % 10) - 5;
        double by = (rng() % 10) - 5;
        double bz = (rng() % 10) - 5;

        Vector3d a(ax, ay, az);
        Vector3d b(bx, by, bz);

        REQUIRE(
            ((ax != bx) || (ay != by) || (az != bz)) ==
            (a != b)
        );
    }

    return;
}

// Tests for matrix multiplication working regarding rotation
TEST_CASE(__FILE__"/MatrixMult_Rotate_Yaw", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(69, 32, 16);
    Vector3d originalVec = vec;

    // Create 90deg yaw rotation matrix (Y)
    Matrix4x4 mat;
    mat[0] = {  0, 0, 1, 0 };
    mat[1] = {  0, 1, 0, 0 };
    mat[2] = { -1, 0, 0, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO("Rot #1 " << vec);
    REQUIRE(Vector3d(16, 32, -69) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #2 " << vec);
    REQUIRE(Vector3d(-69, 32, -16) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #3 " << vec);
    REQUIRE(Vector3d(-16, 32, 69) == vec);

    // Rotate by 90 deg a last time, having completed a 360� rotation.
    vec *= mat;
    INFO("Rot #4 " << vec);
    REQUIRE(originalVec == vec);

    return;
}

// Tests for matrix multiplication working regarding rotation
TEST_CASE(__FILE__"/MatrixMult_Rotate_Roll", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(69, 32, 16);
    Vector3d originalVec = vec;

    // Create 90deg roll rotation matrix (Z)
    Matrix4x4 mat;
    mat[0] = { 0, -1, 0, 0 };
    mat[1] = { 1,  0, 0, 0 };
    mat[2] = { 0,  0, 1, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO("Rot #1 " << vec);
    REQUIRE(Vector3d(-32, 69, 16) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #2 " << vec);
    REQUIRE(Vector3d(-69, -32, 16) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #3 " << vec);
    REQUIRE(Vector3d(32, -69, 16) == vec);

    // Rotate by 90 deg a last time, having completed a 360� rotation.
    vec *= mat;
    INFO("Rot #4 " << vec);
    REQUIRE(originalVec == vec);

    return;
}

// Tests for matrix multiplication working regarding rotation
TEST_CASE(__FILE__"/MatrixMult_Rotate_Pitch", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(69, 32, 16);
    Vector3d originalVec = vec;

    // Create 90deg pitch rotation matrix (X)
    Matrix4x4 mat;
    mat[0] = { 1, 0,  0, 0 };
    mat[1] = { 0, 0, -1, 0 };
    mat[2] = { 0, 1,  0, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO("Rot #1 " << vec);
    REQUIRE(Vector3d(69, -16, 32) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #2 " << vec);
    REQUIRE(Vector3d(69, -32, -16) == vec);

    // Rotate again!
    vec *= mat;
    INFO("Rot #3 " << vec);
    REQUIRE(Vector3d(69, 16, -32) == vec);

    // Rotate by 90 deg a last time, having completed a 360� rotation.
    vec *= mat;
    INFO("Rot #4 " << vec);
    REQUIRE(originalVec == vec);

    return;
}

// Tests if rotating a vector (1,1,1) by (45,45,45) eulers works
TEST_CASE(__FILE__"/MatrixMult_Rotate_Unit_Combined", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(1, 1, 1);

    // Create rotation matrix
    Matrix4x4 mat;
    mat[0] = {     0.5, -0.1465,  0.8535, 0 };
    mat[1] = {     0.5,  0.8535, -0.1465, 0 };
    mat[2] = { -0.7072,     0.5,     0.5, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE((
        Math::Similar(vec.x, 1.207, 0.001) &&
        Math::Similar(vec.y, 1.207, 0.001) &&
        Math::Similar(vec.z, 0.2928, 0.001)
    ));

    return;
}

// Tests if rotating a vector (69,32,16) by (45,45,45) eulers works
TEST_CASE(__FILE__"/MatrixMult_Rotate_HalfUnit_Combined", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(69, 32, 16);

    // Create rotation matrix
    Matrix4x4 mat;
    mat[0] = {     0.5, -0.1465,  0.8535, 0 };
    mat[1] = {     0.5,  0.8535, -0.1465, 0 };
    mat[2] = { -0.7072,     0.5,     0.5, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE((
        Math::Similar(vec.x, 43.468, 0.001) &&
        Math::Similar(vec.y, 59.468, 0.001) &&
        Math::Similar(vec.z, -24.7968, 0.001)
    ));

    return;
}

// Tests if rotating a vector (69,32,16) by (45,45,45) eulers works
TEST_CASE(__FILE__"/MatrixMult_Rotate_Combined", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(69, 32, 16);

    // Create rotation matrix
    Matrix4x4 mat;
    mat[0] = { -0.1639, -0.9837, -0.0755, 0 };
    mat[1] = {  0.128,   -0.097,   0.987, 0 };
    mat[2] = { -0.9782,   0.152,  0.1417, 0 };

    // Rotate vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE((
        Math::Similar(vec.x, -43.9955, 0.001) &&
        Math::Similar(vec.y, 21.52, 0.001) &&
        Math::Similar(vec.z, -60.3646, 0.001)
    ));

    return;
}

// Tests if matrix scaling works ( x axis only )
TEST_CASE(__FILE__"/MatrixMult_Scale_X", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5, 6, 7);

    // Create scaling matrix
    Matrix4x4 mat;
    mat[0] = { 3, 0, 0, 0 };
    mat[1] = { 0, 1, 0, 0 };
    mat[2] = { 0, 0, 1, 0 };

    // Scale vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3d(15, 6, 7) == vec);

    return;
}

// Tests if matrix scaling works ( y axis only )
TEST_CASE(__FILE__"/MatrixMult_Scale_Y", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5, 6, 7);

    // Create scaling matrix
    Matrix4x4 mat;
    mat[0] = { 1, 0, 0, 0 };
    mat[1] = { 0, 3, 0, 0 };
    mat[2] = { 0, 0, 1, 0 };

    // Scale vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3d(5, 18, 7) == vec);

    return;
}

// Tests if matrix scaling works ( z axis only )
TEST_CASE(__FILE__"/MatrixMult_Scale_Z", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5, 6, 7);

    // Create scaling matrix
    Matrix4x4 mat;
    mat[0] = { 1, 0, 0, 0 };
    mat[1] = { 0, 1, 0, 0 };
    mat[2] = { 0, 0, 3, 0 };

    // Scale vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3d(5, 6, 21) == vec);

    return;
}

// Tests if matrix scaling works ( all axes )
TEST_CASE(__FILE__"/MatrixMult_Scale_Combined", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5, 6, 7);

    // Create scaling matrix
    Matrix4x4 mat;
    mat[0] = { 4, 0, 0, 0 };
    mat[1] = { 0, 5, 0, 0 };
    mat[2] = { 0, 0, 8, 0 };

    // Scale vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3d(20, 30, 56) == vec);

    return;
}

// Tests if translation via matrix multiplication works
TEST_CASE(__FILE__"/MatrixMult_Translation", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5, 6, 7);

    // Create scaling matrix
    Matrix4x4 mat;
    mat[0] = { 1, 0, 0, 155 };
    mat[1] = { 0, 1, 0, -23 };
    mat[2] = { 0, 0, 1, 333 };

    // Translate vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3d(160, -17, 340) == vec);

    return;
}

// Tests the multiplication operator (*) with a simple matrix. All other tests used the * operator (without the '=')
TEST_CASE(__FILE__"/MatrixMult_Not_Using_MultEqualsOperator", "[Vector][Vector3]")
{
    // Create vector
    Vector3d vec(5.1, 6.4, 7.99);

    // Create scaling and translation matrix
    Matrix4x4 mat;
    mat[0] = { 3.8, 0, 0, -5.1 };
    mat[1] = { 0, -1.5, 0, 15.2 };
    mat[2] = { 0, 0, 3.01, 19.9 };

    // Transform vector
    vec = vec * mat;

    // Did we succeed?
    Vector3d expected(
        5.1 * 3.8 - 5.1,
        6.4 * -1.5 + 15.2,
        7.99 * 3.01 + 19.9
    );

    INFO('\n'
        << "Expected: " << expected << '\n'
        << "Actual: " << vec
    );

    REQUIRE(expected == vec);

    return;
}

// A simple matrix multiplication tested on an int vector
TEST_CASE(__FILE__"/MatrixMult_Dirty_Int_Check", "[Vector][Vector3]")
{
    // Create vector
    Vector3i vec(5, 6, 7);

    // Create scaling and translation matrix
    Matrix4x4 mat;
    mat[0] = { 3,  0, 0, -5 };
    mat[1] = { 0, -1, 0, 15 };
    mat[2] = { 0,  0, 3, 20 };

    // Transform vector
    vec *= mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3i(
        5*3 + -5,
        6*-1 + 15,
        7*3 + 20
    ) == vec);

    return;
}

// Tests the multiplication operator (*) with a simple matrix. All other tests used the * operator (without the '=')
TEST_CASE(__FILE__"/MatrixMult_Dirty_Int_Check_Not_Using_MultEqualsOperator", "[Vector][Vector3]")
{
    // Create vector
    Vector3i vec(5, 6, 7);

    // Create scaling and translation matrix
    Matrix4x4 mat;
    mat[0] = { 3,  0, 0, -5 };
    mat[1] = { 0, -1, 0, 15 };
    mat[2] = { 0,  0, 3, 20 };

    // Scale vector
    vec = vec * mat;

    // Did we succeed?
    INFO(vec);
    REQUIRE(Vector3i(
        5 * 3 + -5,
        6 * -1 + 15,
        7 * 3 + 20
    ) == vec);

    return;
}

//This tests the multiplication equals operator (*=) procedurally (but without rotation)
TEST_CASE(__FILE__"/MatrixMult_Equals_Procedural", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Generate parameters
        const double initialX = LARGE_RAND_DOUBLE;
        const double initialY = LARGE_RAND_DOUBLE;
        const double initialZ = LARGE_RAND_DOUBLE;
        const double scaleX   = LARGE_RAND_DOUBLE;
        const double scaleY   = LARGE_RAND_DOUBLE;
        const double scaleZ   = LARGE_RAND_DOUBLE;
        const double transX   = LARGE_RAND_DOUBLE;
        const double transY   = LARGE_RAND_DOUBLE;
        const double transZ   = LARGE_RAND_DOUBLE;

        // Create vector
        Vector3d vec(initialX, initialY, initialZ);

        // Create matrix
        Matrix4x4 mat;
        mat[0] = { scaleX,      0,      0, transX };
        mat[1] = {      0, scaleY,      0, transY };
        mat[2] = {      0,      0, scaleZ, transZ };
        mat[3] = {      0,      0,      0,      0 };

        // Create expected vector
        Vector3d expected(
            initialX * scaleX + transX,
            initialY * scaleY + transY,
            initialZ * scaleZ + transZ
        );

        // Transform vector
        vec *= mat;

        // Compare
        REQUIRE(vec.Similar(expected, 0.01));
    }

    return;
}

// This tests the matrix multiplication operator (*) procedurally (but without rotation)
TEST_CASE(__FILE__"/MatrixMult_Procedural", "[Vector][Vector3]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // Generate parameters
        const double initialX = LARGE_RAND_DOUBLE;
        const double initialY = LARGE_RAND_DOUBLE;
        const double initialZ = LARGE_RAND_DOUBLE;
        const double scaleX   = LARGE_RAND_DOUBLE;
        const double scaleY   = LARGE_RAND_DOUBLE;
        const double scaleZ   = LARGE_RAND_DOUBLE;
        const double transX   = LARGE_RAND_DOUBLE;
        const double transY   = LARGE_RAND_DOUBLE;
        const double transZ   = LARGE_RAND_DOUBLE;

        // Create vector
        Vector3d vec(initialX, initialY, initialZ);

        // Create matrix
        Matrix4x4 mat;
        mat[0] = { scaleX,      0,      0, transX };
        mat[1] = {      0, scaleY,      0, transY };
        mat[2] = {      0,      0, scaleZ, transZ };
        mat[3] = {      0,      0,      0,      0 };

        // Create expected vector
        Vector3d expected(
            initialX * scaleX + transX,
            initialY * scaleY + transY,
            initialZ * scaleZ + transZ
        );

        // Transform vector
        vec = vec * mat;

        // Compare
        REQUIRE(vec.Similar(expected, 0.01));
    }

    return;
}

// Tests loose comparison via Vector3d::Similar -> true
TEST_CASE(__FILE__"/Similar_True", "[Vector][Vector3]")
{
    REQUIRE(
        Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999).Similar(
            Vector3d(0, -6.666666667, 10)
    ));
    return;
}

// Tests loose comparison via Vector3d::Similar -> false
TEST_CASE(__FILE__"/Similar_False", "[Vector][Vector3]")
{
    REQUIRE_FALSE(
        Vector3d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999).Similar(
            Vector3d(0.1, -6.7, 10.1)
    ));
    return;
}

// Tests that the move constructor works
TEST_CASE(__FILE__"/Move_Constructor", "[Vector][Vector3]")
{
    Vector3d a(1,2,3);
    Vector3d b(std::move(a));

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);
    REQUIRE(b.z == 3.0);

    return;
}

// Tests that the move operator works
TEST_CASE(__FILE__"/Move_Operator", "[Vector][Vector3]")
{
    Vector3d a(1, 2, 3);
    Vector3d b = std::move(a);

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);
    REQUIRE(b.z == 3.0);

    return;
}
