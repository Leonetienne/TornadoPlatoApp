#include "../_TestingUtilities/Catch2.h"
#include "../Eule/Vector2.h"
#include "../Eule/Math.h"
#include "../_TestingUtilities/HandyMacros.h"
#include <random>
#include <sstream>

using namespace Eule;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests if all values are 0 after initialization via default constructor
TEST_CASE(__FILE__"/New_Vector_All_0", "[Vector][Vector2]")
{
    Vector2d v2;

    REQUIRE(0.0 == v2.x);
    REQUIRE(0.0 == v2.y);

    return;
}

// Tests if values can be set via the constructor
TEST_CASE(__FILE__"/Can_Set_Values_Constructor", "[Vector][Vector2]")
{
    Vector2d v2(69, 32);

    REQUIRE(69.0 == v2.x);
    REQUIRE(32.0 == v2.y);

    return;
}

// Tests if values can be set via letters
TEST_CASE(__FILE__"/Can_Set_Values_Letters", "[Vector][Vector2]")
{
    Vector2d v2;
    v2.x = 69;
    v2.y = 32;

    REQUIRE(69.0 == v2.x);
    REQUIRE(32.0 == v2.y);

    return;
}

// Tests if values can be set via array descriptors
TEST_CASE(__FILE__"/Can_Set_Values_ArrayDescriptor", "[Vector][Vector2]")
{
    Vector2d v2;
    v2[0] = 69;
    v2[1] = 32;

    REQUIRE(69.0 == v2.x);
    REQUIRE(32.0 == v2.y);

    return;
}

// Tests if values can be set via an initializer list
TEST_CASE(__FILE__"/Can_Set_Values_InitializerList", "[Vector][Vector2]")
{
    Vector2d v2 = {69, 32};

    REQUIRE(69.0 == v2.x);
    REQUIRE(32.0 == v2.y);

    return;
}

// Tests for vectors copied via the copy constructor to have the same values
TEST_CASE(__FILE__"/Copy_Constructor_Same_Values", "[Vector][Vector2]")
{
    Vector2d a(69, 32);
    Vector2d b(a);

    REQUIRE(a.x == b.x);
    REQUIRE(a.y == b.y);

    return;
}

// Tests for vectors copied via the equals operator to have the same values
TEST_CASE(__FILE__"/Operator_Equals_Same_Values", "[Vector][Vector2]")
{
    Vector2d a(69, 32);
    Vector2d b = a;

    REQUIRE(a.x == b.x);
    REQUIRE(a.y == b.y);

    return;
}

// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Copy_Constructor_Independent", "[Vector][Vector2]")
{
    Vector2d a(69, 32);
    Vector2d b(a);

    b.x = 169;
    b.y = 132;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);

    REQUIRE(169.0 == b.x);
    REQUIRE(132.0 == b.y);

    return;
}

// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Operator_Equals_Independent", "[Vector][Vector2]")
{
    Vector2d a(69, 32);
    Vector2d b = a;

    b.x = 169;
    b.y = 132;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);

    REQUIRE(169.0 == b.x);
    REQUIRE(132.0 == b.y);

    return;
}

// Tests if the dot product between two vectors angled 90 degrees from one another is 0. It should by definition be 0!
// Dot products are commutative, so we'll check both directions.
TEST_CASE(__FILE__"/DotProduct_90deg", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 100; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        const Vector2d a = Vector2d(1, 0) * (rng() % 6969 + 1.0);
        const Vector2d b = Vector2d(0, 1) * (rng() % 6969 + 1.0);

        INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
        REQUIRE(0.0 == a.DotProduct(b));
        REQUIRE(0.0 == b.DotProduct(a));
    }

    return;
}

// Test if the dot product is positive for two vectors angled less than 90 degrees from another
// Dot products are commutative, so we'll check both directions.
TEST_CASE(__FILE__"/DotProduct_LessThan90deg", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        const Vector2d a = Vector2d(1, 1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
        const Vector2d b = Vector2d(1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);

        INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
        REQUIRE(a.DotProduct(b) > 0);
        REQUIRE(b.DotProduct(a) > 0);
    }

    return;
}

// Test if the dot product is negative for two vectors angled greater than 90 degrees from another
// Dot products are commutative, so we'll check both directions.
TEST_CASE(__FILE__"/DotProduct_GreaterThan90deg", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        // The length of the vectors should not matter. Only the angle should.
        // Let's test that!
        const Vector2d a = Vector2d(1, -1.0 / (rng() % 100)) * (rng() % 6969 + 1.0); // Don't allow the scalar to become 0
        const Vector2d b = Vector2d(-1.0 / (rng() % 100), 1) * (rng() % 6969 + 1.0);

        INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
        REQUIRE(a.DotProduct(b) < 0);
        REQUIRE(b.DotProduct(a) < 0);
    }

    return;
}

// Tests that the dot product is correct for a known value
TEST_CASE(__FILE__"/DotProduct_Oracle", "[Vector][Vector2]")
{
    // Setup
    const Vector2d a(-99, 199);
    const Vector2d b(18, -1);

    // Exercise
    const double dot = a.DotProduct(b);

    // Verify
    REQUIRE(-1981.0 == dot);

    return;
}

// Quick and dirty check if the useless int-method is working
TEST_CASE(__FILE__"/DotProduct_Dirty_Int", "[Vector][Vector2]")
{
    Vector2i a;
    Vector2i b;

    // 90 deg
    a = {0, 10};
    b = {10, 0};
    INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
    REQUIRE(0.0 == a.DotProduct(b));
    REQUIRE(0.0 == b.DotProduct(a));

    // < 90 deg
    a = { 7, 10 };
    b = { 10, 1 };
    INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
    REQUIRE(a.DotProduct(b) > 0.0);
    REQUIRE(b.DotProduct(a) > 0.0);

    // > 90 deg
    a = { -3, 10 };
    b = { 10, -4 };
    INFO(a << L" DOT " << b << L" = " << a.DotProduct(b));
    REQUIRE(a.DotProduct(b) < 0.0);
    REQUIRE(b.DotProduct(a) < 0.0);

    return;
}

// Tests if the cross product of two vectors of the exact opposite direction is 0
TEST_CASE(__FILE__"/CrossProduct_Opposite_Direction", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE / 1000.0;
        const double y = LARGE_RAND_DOUBLE / 1000.0;

        // Vector length should not matter, so randomize it
        // In this case, they are allowed to be of length 0
        // Don't scale it up too much to avoid failure due to floating point inaccuracy
        Vector2d a = Vector2d( x,  y) * (LARGE_RAND_DOUBLE / 1000.0);
        Vector2d b = Vector2d(-x, -y) * (LARGE_RAND_DOUBLE / 1000.0);

        INFO(a << " CROSS " << b << " = " << a.CrossProduct(b) << " in iteration #" << i << ". Shared x and y are: [" << x << ", " << y << "]");
        REQUIRE(Math::Similar(a.CrossProduct(b), 0.0, 10));
    }

    return;
}

// Tests if the cross product of two vectors of the exact same direction is 0
TEST_CASE(__FILE__"/CrossProduct_Same_Direction", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE / 1000.0;
        const double y = LARGE_RAND_DOUBLE / 1000.0;

        // Vector length should not matter, so randomize it
        // In this case, they are allowed to be of length 0
        // Don't scale it up too much to avoid failure due to floating point inaccuracy
        Vector2d a = Vector2d(x, y) * (LARGE_RAND_DOUBLE / 1000.0);
        Vector2d b = Vector2d(x, y) * (LARGE_RAND_DOUBLE / 1000.0);

        INFO(a << " CROSS " << b << " = " << a.CrossProduct(b));
        REQUIRE(Math::Similar(a.CrossProduct(b), 0.0, 10));
    }

    return;
}

// Tests for the cross product to be positive, if vector b is to the left of a
TEST_CASE(__FILE__"/CrossProduct_BToTheLeft", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_POSITIVE_DOUBLE; // The dot product also flips if both points are negative in height.. let's keep it positive.
        if (x == 0) x++;
        if (y == 0) y++;

        Vector2d a = Vector2d(x, y);
        Vector2d b = Vector2d(x - (rng() % 6969ll + 1.0), y);

        INFO(a << " CROSS " << b << " = " << a.CrossProduct(b) << " in iteration #" << i );
        REQUIRE(a.CrossProduct(b) > 0);
    }

    return;
}

// Tests for the cross product to be negative, if vector b is to the left of a
TEST_CASE(__FILE__"/CrossProduct_BToTheRight", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_POSITIVE_DOUBLE; // The dot product also flips if both points are negative in height.. let's keep it positive.
        if (x == 0) x++;
        if (y == 0) y++;

        Vector2d a = Vector2d(x, y);
        Vector2d b = Vector2d(x + (rng() % 6969ll + 1.0), y);

        INFO(a << L" CROSS " << b << L" = " << a.CrossProduct(b))
        REQUIRE(a.CrossProduct(b) < 0);
    }

    return;
}

// Quick and dirty check if the useless int-method is working
TEST_CASE(__FILE__"/CrossProduct_Dirty_Int", "[Vector][Vector2]")
{
    Vector2i a;
    Vector2i b;

    // Same direction
    a = { 10, 0 };
    b = { 10, 0 };
    INFO(a << " CROSS " << b << " = " << a.CrossProduct(b));
    REQUIRE(0.0 == a.CrossProduct(b));
    REQUIRE(0.0 == b.CrossProduct(a));

    // Opposite direction
    a = { -10, 0 };
    b = { 10, 0 };
    INFO(a << " CROSS " << b << " = " << a.CrossProduct(b));
    REQUIRE(0.0 == a.CrossProduct(b));
    REQUIRE(0.0 == b.CrossProduct(a));

    // B to the left
    a = { 0, 10 };
    b = { -5, 10 };
    INFO(a << " CROSS " << b << " = " << a.CrossProduct(b));
    REQUIRE(a.CrossProduct(b) > 0.0);

    // B to the right
    a = { 0, 10 };
    b = { 17, 10 };
    INFO(a << " CROSS " << b << " = " << a.CrossProduct(b));
    REQUIRE(a.CrossProduct(b) < 0.0);

    return;
}

// Tests the SqrMagnitude method to work as expected with random numbers
TEST_CASE(__FILE__"/SqrMagnitude", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = (double)(rng() % 1000) - 500.0;
        const double y = (double)(rng() % 1000) - 500.0;
        const double expected = x*x + y*y;

        REQUIRE(expected == Vector2d(x, y).SqrMagnitude());
    }

    return;
}

// Checks if the int method is working
TEST_CASE(__FILE__"/SqrMagnitude_Int", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const int x = LARGE_RAND_INT;
        const int y = LARGE_RAND_INT;
        const int expected = x*x + y*y;

        REQUIRE(Math::Similar((double)expected, Vector2i(x, y).SqrMagnitude()));
    }

    return;
}

// Tests for the length of the vector (0,0) being 0
TEST_CASE(__FILE__"/Magnitude_Is_0_On_Vec0", "[Vector][Vector2]")
{
    REQUIRE(0.0 == Vector2d(0, 0).Magnitude());
    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_X", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = (double)(rng() % 1000) - 500.0;
        const Vector2d vec(x, 0);
        REQUIRE(Math::Similar(abs(x), vec.Magnitude()));
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_Y", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double y = (double)(rng() % 1000) - 500.0;
        const Vector2d vec(0, y);
        REQUIRE(Math::Similar(abs(y), vec.Magnitude()));
    }

    return;
}

// Tests for a known result
TEST_CASE(__FILE__"/Magnitude", "[Vector][Vector2]")
{
    // Ya'll got more of 'dem digits?
    REQUIRE(204.02205763103165736538358032703399658203125 == Vector2d(192, -69).Magnitude());
    return;
}

// Tests for expected lerp result 0.00
TEST_CASE(__FILE__"/Lerp_000", "[Vector][Vector2]")
{
    const Vector2d a(100, 1000);
    const Vector2d b(200, 4000);
    const Vector2d res = a.Lerp(b, 0.00);

    INFO(res);
    REQUIRE(a == res);
    return;
}

// Tests for expected lerp result 0.25
TEST_CASE(__FILE__"/Lerp_025", "[Vector][Vector2]")
{
    const Vector2d a(100, 1000);
    const Vector2d b(200, 4000);
    const Vector2d res = a.Lerp(b, 0.25);

    INFO(res);
    REQUIRE(Vector2d(125, 1750) == res);
    return;
}

// Tests for expected lerp result 0.50
TEST_CASE(__FILE__"/Lerp_050", "[Vector][Vector2]")
{
    const Vector2d a(100, 1000);
    const Vector2d b(200, 4000);
    const Vector2d res = a.Lerp(b, 0.50);

    INFO(res);
    REQUIRE(Vector2d(150, 2500) == res);
    return;
}

// Tests for expected lerp result 0.75
TEST_CASE(__FILE__"/Lerp_075", "[Vector][Vector2]")
{
    const Vector2d a(100, 1000);
    const Vector2d b(200, 4000);
    const Vector2d res = a.Lerp(b, 0.75);

    INFO(res);
    REQUIRE(Vector2d(175, 3250) == res);
    return;
}

// Tests for expected lerp result 1.00
TEST_CASE(__FILE__"/Lerp_100", "[Vector][Vector2]")
{
    const Vector2d a(100, 1000);
    const Vector2d b(200, 4000);
    const Vector2d res = a.Lerp(b, 1.00);

    INFO(res);
    REQUIRE(b == res);
    return;
}

// Tests lerpself
TEST_CASE(__FILE__"/LerpSelf", "[Vector][Vector2]")
{
    Vector2d a(100, 1000);
    Vector2d b(200, 4000);

    a.LerpSelf(b, 0.75);

    INFO(a);
    REQUIRE(Vector2d(175, 3250) == a);
    return;
}

// Tests if an input vector of length 0 is handled correctly by the normalize method
TEST_CASE(__FILE__"/Normalize_Length_Before_Is_0", "[Vector][Vector2]")
{
    const Vector2d vec(0, 0);
    REQUIRE(0.0 == vec.Normalize().Magnitude());
    return;
}

// Tests for any normalized vector to be of length 1
TEST_CASE(__FILE__"/Normalize_Length_Is_1", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        double x = LARGE_RAND_DOUBLE;
        double y = LARGE_RAND_DOUBLE;
        if (x == 0) x++;
        if (y == 0) y++;

        Vector2d vec(x, y);

        INFO(vec);
        REQUIRE(Math::Similar(vec.Normalize().Magnitude(), 1.0)); // Account for floating point inaccuracy
    }

    return;
}

// Tests the normalize method with known values
TEST_CASE(__FILE__"/Normalize_Oracle", "[Vector][Vector2]")
{
    // Setup
    Vector2d v(3.2, -5.3);

    // Exercise
    v.NormalizeSelf();

    // Verify
    Vector2d expected(0.51686909903, -0.85606444527);
    REQUIRE(v.Similar(expected));
}

// Tests for a normalized vector to still point in the exact same direction
TEST_CASE(__FILE__"/Normalize_Direction_Stays_Unaffected", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;

        Vector2d vec(x, y);

        // Prevent a vector of length 0 going in
        if (vec.SqrMagnitude() == 0)
            vec.x++;

        Vector2d vec_n(x, y);
        vec_n = vec_n.Normalize();

        INFO(vec << " | " << vec_n);

        // Both vectors should still point in the same direction!
        REQUIRE((
            (vec.DotProduct(vec_n) > 0) && // Roughly same direction
            (Math::Similar(vec_n.CrossProduct(vec), 0.0)) // Both vectors align
        ));
    }
    return;
}

// Kinda dumb method, but ok lol
// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
TEST_CASE(__FILE__"/Normalized_Int_Vector_Is_0", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        int x = LARGE_RAND_INT;
        int y = LARGE_RAND_INT;

        Vector2i vec(x, y);

        vec.NormalizeSelf();

        std::wstringstream wss;
        wss << vec;
        REQUIRE(0.0 == vec.Magnitude());
    }
}

// Tests that NormalizeSelf() results in the same as Normalize()
TEST_CASE(__FILE__"/NormalizeSelf_IsSameAs_Normalize", "[Vector][Vector2]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        Vector2d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);

        Vector2d nVec = vec.Normalize();
        vec.NormalizeSelf();

        REQUIRE(nVec == vec);
    }

    return;
}

// Tests for the VectorScale() method to work
TEST_CASE(__FILE__"/VectorScale", "[Vector][Vector2]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;

        const Vector2d a(ax, ay);
        const Vector2d b(bx, by);

        Vector2d target(
            ax * bx,
            ay * by
        );

        REQUIRE(a.VectorScale(b) == target);
    }

    return;
}

// Tests for operator- (unary) to work
TEST_CASE(__FILE__"/Operator_Unary_Negative", "[Vector][Vector2]")
{
    const Vector2d v(29, -5);

    REQUIRE(Vector2d(-29, 5) == -v);

    return;
}

// Tests for operator+ to work as expected
TEST_CASE(__FILE__"/Operator_Add", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;

        const Vector2d a(ax, ay);
        const Vector2d b(bx, by);

        REQUIRE(Vector2d(ax+bx, ay+by) == a+b);
    }

    return;
}

// Tests for operator+= to work as expected
TEST_CASE(__FILE__"/Operator_Add_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;

        Vector2d a(ax, ay);
        a += Vector2d(bx, by);

        REQUIRE(Vector2d(ax + bx, ay + by) == a);
    }

    return;
}

// Tests for operator- to work as expected
TEST_CASE(__FILE__"/Operator_Sub", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;

        const Vector2d a(ax, ay);
        const Vector2d b(bx, by);

        REQUIRE(Vector2d(ax - bx, ay - by) == a - b);
    }

    return;
}

// Tests for operator-= to work as expected
TEST_CASE(__FILE__"/Operator_Sub_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;

        Vector2d a(ax, ay);
        a -= Vector2d(bx, by);

        REQUIRE(Vector2d(ax - bx, ay - by) == a);
    }

    return;
}

// Tests for operator* to work as expected
TEST_CASE(__FILE__"/Operator_Mult", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector2d a(x, y);

        REQUIRE(Vector2d(x * scalar, y * scalar) == a * scalar);
    }

    return;
}

// Tests for operator*= to work as expected
TEST_CASE(__FILE__"/Operator_Mult_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector2d a(x, y);
        a *= scalar;

        REQUIRE(Vector2d(x * scalar, y * scalar) == a);
    }

    return;
}

// Tests for operator/ to work as expected
TEST_CASE(__FILE__"/Operator_Div", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector2d a(x, y);

        REQUIRE(Vector2d(x / scalar, y / scalar) == a / scalar);
    }

    return;
}

// Tests for operator/= to work as expected
TEST_CASE(__FILE__"/Operator_Div_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector2d a(x, y);
        a /= scalar;

        REQUIRE(Vector2d(x / scalar, y / scalar) == a);
    }

    return;
}

// Tests for operator== to work as expected
TEST_CASE(__FILE__"/Operator_Compare_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = (rng() % 10) - 5;
        const double ay = (rng() % 10) - 5;
        const double bx = (rng() % 10) - 5;
        const double by = (rng() % 10) - 5;

        const Vector2d a(ax, ay);
        const Vector2d b(bx, by);

        REQUIRE(
            ((ax == bx) && (ay == by)) ==
            (a == b)
        );
    }

    return;
}

// Tests for operator!= to work as expected
TEST_CASE(__FILE__"/Operator_Not_Equals", "[Vector][Vector2]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = (rng() % 10) - 5;
        const double ay = (rng() % 10) - 5;
        const double bx = (rng() % 10) - 5;
        const double by = (rng() % 10) - 5;

        const Vector2d a(ax, ay);
        const Vector2d b(bx, by);

        REQUIRE(
            ((ax != bx) || (ay != by)) ==
            (a != b)
        );
    }

    return;
}

// Tests loose comparison via Vector2d::Similar -> true
TEST_CASE(__FILE__"/Similar_True", "[Vector][Vector2]")
{
    REQUIRE(
        Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666).Similar(
            Vector2d(0, -6.666666667)
    ));
    return;
}

// Tests loose comparison via Vector2d::Similar -> false
TEST_CASE(__FILE__"/Similar_False", "[Vector][Vector2]")
{
    REQUIRE_FALSE(
        Vector2d(0.00000000000000000000001, -6.6666666666666666666666666666).Similar(
            Vector2d(0.1, -6.7)
    ));
    return;
}

// Tests that the move constructor works
TEST_CASE(__FILE__"/Move_Constructor", "[Vector][Vector2]")
{
    const Vector2d a(1, 2);
    const Vector2d b(std::move(a));

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);

    return;
}

// Tests that the move operator works
TEST_CASE(__FILE__"/Move_Operator", "[Vector][Vector2]")
{
    const Vector2d a(1, 2);
    const Vector2d b = std::move(a);

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);

    return;
}
