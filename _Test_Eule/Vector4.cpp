#include "../_TestingUtilities/Catch2.h"
#include "../_TestingUtilities/HandyMacros.h"
#include "../Eule/Math.h"
#include "../Eule/Vector4.h"
#include "../Eule/Matrix4x4.h"
#include <random>

using namespace Eule;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests if all values are 0 after initialization via default constructor
TEST_CASE(__FILE__"/New_Vector_All_0", "[Vector][Vector4]")
{
    const Vector4d v4;

    REQUIRE(0.0 == v4.x);
    REQUIRE(0.0 == v4.y);
    REQUIRE(0.0 == v4.z);
    REQUIRE(0.0 == v4.w);

    return;
}

// Tests if values can be set via the constructor
TEST_CASE(__FILE__"/Can_Set_Values_Constructor", "[Vector][Vector4]")
{
    const Vector4d v4(69, 32, 16, 10);

    REQUIRE(69.0 == v4.x);
    REQUIRE(32.0 == v4.y);
    REQUIRE(16.0 == v4.z);
    REQUIRE(10.0 == v4.w);

    return;
}

// Tests if values can be set via letters
TEST_CASE(__FILE__"/Can_Set_Values_Letters", "[Vector][Vector4]")
{
    Vector4d v4;
    v4.x = 69;
    v4.y = 32;
    v4.z = 16;
    v4.w = 10;

    REQUIRE(69.0 == v4.x);
    REQUIRE(32.0 == v4.y);
    REQUIRE(16.0 == v4.z);
    REQUIRE(10.0 == v4.w);

    return;
}

// Tests if values can be set via array descriptors
TEST_CASE(__FILE__"/Can_Set_Values_ArrayDescriptor", "[Vector][Vector4]")
{
    Vector4d v4;
    v4[0] = 69;
    v4[1] = 32;
    v4[2] = 16;
    v4[3] = 10;

    REQUIRE(69.0 == v4.x);
    REQUIRE(32.0 == v4.y);
    REQUIRE(16.0 == v4.z);
    REQUIRE(10.0 == v4.w);

    return;
}

// Tests if values can be set via an initializer list
TEST_CASE(__FILE__"/Can_Set_Values_InitializerList", "[Vector][Vector4]")
{
    const Vector4d v4 = { 69, 32, 16, 10 };

    REQUIRE(69.0 == v4.x);
    REQUIRE(32.0 == v4.y);
    REQUIRE(16.0 == v4.z);
    REQUIRE(10.0 == v4.w);

    return;
}

// Tests for vectors copied via the copy constructor to have the same values
TEST_CASE(__FILE__"/Copy_Constructor_Same_Values", "[Vector][Vector4]")
{
    const Vector4d a(69, 32, 16, 10);
    Vector4d b(a);

    REQUIRE(69.0 == b.x);
    REQUIRE(32.0 == b.y);
    REQUIRE(16.0 == b.z);
    REQUIRE(10.0 == b.w);

    return;
}

// Tests for vectors copied via the equals operator to have the same values
TEST_CASE(__FILE__"/Operator_Equals_Same_Values", "[Vector][Vector4]")
{
    const Vector4d a(69, 32, 16, 10);
    Vector4d b = a;

    REQUIRE(69.0 == b.x);
    REQUIRE(32.0 == b.y);
    REQUIRE(16.0 == b.z);
    REQUIRE(10.0 == b.w);

    return;
}

// Tests for vectors copied via the copy constructor to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Copy_Constructor_Independent", "[Vector][Vector4]")
{
    const Vector4d a(69, 32, 16, 10);
    Vector4d b(a);

    b.x = 169;
    b.y = 132;
    b.z = 116;
    b.w = 110;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);
    REQUIRE(16.0 == a.z);
    REQUIRE(10.0 == a.w);

    REQUIRE(169.0 == b.x);
    REQUIRE(132.0 == b.y);
    REQUIRE(116.0 == b.z);
    REQUIRE(110.0 == b.w);

    return;
}

// Tests for vectors copied via the equals operator to be modifyable without modifying the original object
TEST_CASE(__FILE__"/Operator_Equals_Independent", "[Vector][Vector4]")
{
    const Vector4d a(69, 32, 16, 10);
    Vector4d b = a;

    b.x = 169;
    b.y = 132;
    b.z = 116;
    b.w = 110;

    REQUIRE(69.0 == a.x);
    REQUIRE(32.0 == a.y);
    REQUIRE(16.0 == a.z);
    REQUIRE(10.0 == a.w);

    REQUIRE(169.0 == b.x);
    REQUIRE(132.0 == b.y);
    REQUIRE(116.0 == b.z);
    REQUIRE(110.0 == b.w);

    return;
}

// Tests the SqrMagnitude method to work as expected with random numbers
TEST_CASE(__FILE__"/SqrMagnitude", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;
        const double expected = x*x + y*y + z*z + w*w;

        REQUIRE(expected == Vector4d(x, y, z, w).SqrMagnitude());
    }

    return;
}

// Tests for the length of the vector (0,0,0,0) being 0
TEST_CASE(__FILE__"/Magnitude_Is_0_On_Vec0", "[Vector][Vector4]")
{
    REQUIRE(0.0 == Vector4d(0, 0, 0, 0).Magnitude());
    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_X", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const Vector4d vec(x, 0, 0, 0);
        REQUIRE(abs(x) == vec.Magnitude());
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_Y", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double y = LARGE_RAND_DOUBLE;
        const Vector4d vec(0, y, 0, 0);
        REQUIRE(abs(y) == vec.Magnitude());
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_Z", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double z = LARGE_RAND_DOUBLE;
        const Vector4d vec(0, 0, z, 0);
        REQUIRE(abs(z) == vec.Magnitude());
    }

    return;
}

// Tests for a vector of a known length to actually return that
TEST_CASE(__FILE__"/Magnitude_One_Axis_W", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double w = LARGE_RAND_DOUBLE;
        const Vector4d vec(0, 0, 0, w);
        REQUIRE(abs(w) == vec.Magnitude());
    }

    return;
}

// Tests for a known result
TEST_CASE(__FILE__"/Magnitude", "[Vector][Vector4]")
{
    REQUIRE(Math::Similar(78.574653, Vector4d(-23.76, 15.82, 66.75, 30.06).Magnitude(), 0.0001));
    return;
}

// Tests for expected lerp result 0.00
TEST_CASE(__FILE__"/Lerp_000", "[Vector][Vector4]")
{
    const Vector4d a(100, 1000, 10, -200);
    const Vector4d b(200, 4000, 100, 200);
    const Vector4d res = a.Lerp(b, 0.00);

    INFO(res);
    REQUIRE(a == res);
    return;
}

// Tests for expected lerp result 0.25
TEST_CASE(__FILE__"/Lerp_025", "[Vector][Vector4]")
{
    const Vector4d a(100, 1000, 10, -200);
    const Vector4d b(200, 4000, 100, 200);
    const Vector4d res = a.Lerp(b, 0.25);

    INFO(res);
    REQUIRE(Vector4d(125, 1750, 32.5, -100) == res);
    return;
}

// Tests for expected lerp result 0.50
TEST_CASE(__FILE__"/Lerp_050", "[Vector][Vector4]")
{
    const Vector4d a(100, 1000, 10, -200);
    const Vector4d b(200, 4000, 100, 200);
    const Vector4d res = a.Lerp(b, 0.50);

    INFO(res);
    REQUIRE(Vector4d(150, 2500, 55, 0) == res);
    return;
}

// Tests for expected lerp result 0.75
TEST_CASE(__FILE__"/Lerp_075", "[Vector][Vector4]")
{
    const Vector4d a(100, 1000, 10, -200);
    const Vector4d b(200, 4000, 100, 200);
    const Vector4d res = a.Lerp(b, 0.75);

    INFO(res);
    REQUIRE(Vector4d(175, 3250, 77.5, 100) == res);
    return;
}

// Tests for expected lerp result 1.00
TEST_CASE(__FILE__"/Lerp_100", "[Vector][Vector4]")
{
    const Vector4d a(100, 1000, 10, -200);
    const Vector4d b(200, 4000, 100, 200);
    const Vector4d res = a.Lerp(b, 1.00);

    INFO(res);
    REQUIRE(b == res);
    return;
}

// Tests lerpself
TEST_CASE(__FILE__"/LerpSelf", "[Vector][Vector4]")
{
    Vector4d a(100, 1000, 10, -200);
    Vector4d b(200, 4000, 100, 200);

    a.LerpSelf(b, 0.75);

    INFO(a);
    REQUIRE(Vector4d(175, 3250, 77.5, 100) == a);
    return;
}

// Tests if an input vector of length 0 is handled correctly by the normalize method
TEST_CASE(__FILE__"/Normalize_Length_Before_Is_0", "[Vector][Vector4]")
{
    Vector4d vec(0, 0, 0, 0);
    vec.NormalizeSelf();
    REQUIRE(0.0 == vec.Magnitude());
    return;
}

// Tests for any normalized vector to be of length 1
TEST_CASE(__FILE__"/Normalize_Length_Is_1", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;

        Vector4d vec(x, y, z, w);

        // Prevent a vector of length 0 going in
        if (vec.SqrMagnitude() == 0)
            vec.x++;

        INFO(vec);
        REQUIRE(Math::Similar(vec.Normalize().Magnitude(), 1.0)); // Account for floating point inaccuracy
    }

    return;
}

// Tests the normalize method with known values
TEST_CASE(__FILE__"/Normalize_Oracle", "[Vector][Vector4]")
{
    // Setup
    Vector4d v(3.2, -5.3, 9.88, 69.420);

    // Exercise
    v.NormalizeSelf();

    // Verify
    Vector4d expected(0.0454594951, -0.07529228877, 0.14035619114, 0.98618692201);
    REQUIRE(v.Similar(expected));
}

// Kinda dumb method, but ok lol
// DON'T NORMALIZE INT-VECTORS WHAT IS WRONG WITH YOU
TEST_CASE(__FILE__"/Normalized_Int_Vector_Is_0", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const int x = LARGE_RAND_INT;
        const int y = LARGE_RAND_INT;
        const int z = LARGE_RAND_INT;
        const int w = LARGE_RAND_INT;

        Vector4i vec(x, y, z, w);

        vec.NormalizeSelf();

        INFO(vec);
        REQUIRE(0.0 == vec.Magnitude());
    }
}

// Tests that NormalizeSelf() results in the same as Normalize()
TEST_CASE(__FILE__"/NormalizeSelf_IsSameAs_Normalize", "[Vector][Vector4]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        Vector4d vec(LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE, LARGE_RAND_DOUBLE);

        Vector4d nVec = vec.Normalize();
        vec.NormalizeSelf();

        REQUIRE(nVec == vec);
    }

    return;
}

// Tests for the VectorScale() method to work
TEST_CASE(__FILE__"/VectorScale", "[Vector][Vector4]")
{
    // Run test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        const Vector4d a(ax, ay, az, aw);
        const Vector4d b(bx, by, bz, bw);

        const Vector4d target(
            ax * bx,
            ay * by,
            az * bz,
            aw * bw
        );

        REQUIRE(a.VectorScale(b) == target);
    }

    return;
}

// Tests for operator- (unary) to work
TEST_CASE(__FILE__"/Operator_Unary_Negative", "[Vector][Vector4]")
{
    const Vector4d v(29, -5, 35, -69);

    REQUIRE(Vector4d(-29, 5, -35, 69) == -v);

    return;
}

// Tests for operator+ to work as expected
TEST_CASE(__FILE__"/Operator_Add", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        const Vector4d a(ax, ay, az, aw);
        const Vector4d b(bx, by, bz, bw);

        REQUIRE(Vector4d(ax + bx, ay + by, az + bz, aw + bw) == a + b);
    }

    return;
}

// Tests for operator+= to work as expected
TEST_CASE(__FILE__"/Operator_Add_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        Vector4d a(ax, ay, az, aw);
        a += Vector4d(bx, by, bz, bw);

        REQUIRE(Vector4d(ax + bx, ay + by, az + bz, aw + bw) == a);
    }

    return;
}

// Tests for operator- to work as expected
TEST_CASE(__FILE__"/Operator_Sub", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        const Vector4d a(ax, ay, az, aw);
        const Vector4d b(bx, by, bz, bw);

        REQUIRE(Vector4d(ax - bx, ay - by, az - bz, aw - bw) == a - b);
    }

    return;
}

// Tests for operator-= to work as expected
TEST_CASE(__FILE__"/Operator_Sub_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        Vector4d a(ax, ay, az, aw);
        a -= Vector4d(bx, by, bz, bw);

        REQUIRE(Vector4d(ax - bx, ay - by, az - bz, aw - bw) == a);
    }

    return;
}

// Tests for operator* to work as expected
TEST_CASE(__FILE__"/Operator_Mult", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector4d a(x, y, z, w);

        REQUIRE(Vector4d(x * scalar, y * scalar, z * scalar, w * scalar) == a * scalar);
    }

    return;
}

// Tests for operator*= to work as expected
TEST_CASE(__FILE__"/Operator_Mult_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector4d a(x, y, z, w);
        a *= scalar;

        REQUIRE(Vector4d(x * scalar, y * scalar, z * scalar, w * scalar) == a);
    }

    return;
}

// Tests for operator/ to work as expected
TEST_CASE(__FILE__"/Operator_Div", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        const Vector4d a(x, y, z, w);

        REQUIRE(Vector4d(x / scalar, y / scalar, z / scalar, w / scalar) == a / scalar);
    }

    return;
}

// Tests for operator/= to work as expected
TEST_CASE(__FILE__"/Operator_Div_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double x = LARGE_RAND_DOUBLE;
        const double y = LARGE_RAND_DOUBLE;
        const double z = LARGE_RAND_DOUBLE;
        const double w = LARGE_RAND_DOUBLE;
        const double scalar = LARGE_RAND_DOUBLE;

        Vector4d a(x, y, z, w);
        a /= scalar;

        REQUIRE(Vector4d(x / scalar, y / scalar, z / scalar, w / scalar) == a);
    }

    return;
}

// Tests for operator== to work as expected
TEST_CASE(__FILE__"/Operator_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        const Vector4d a(ax, ay, az, aw);
        const Vector4d b(bx, by, bz, bw);

        REQUIRE(
            ((ax == bx) && (ay == by) && (az == bz) && (aw == bw)) ==
            (a == b)
        );
    }

    return;
}

// Tests for operator!= to work as expected
TEST_CASE(__FILE__"/Operator_Not_Equals", "[Vector][Vector4]")
{
    // Test 1000 times
    for (std::size_t i = 0; i < 1000; i++)
    {
        const double ax = LARGE_RAND_DOUBLE;
        const double ay = LARGE_RAND_DOUBLE;
        const double az = LARGE_RAND_DOUBLE;
        const double aw = LARGE_RAND_DOUBLE;
        const double bx = LARGE_RAND_DOUBLE;
        const double by = LARGE_RAND_DOUBLE;
        const double bz = LARGE_RAND_DOUBLE;
        const double bw = LARGE_RAND_DOUBLE;

        const Vector4d a(ax, ay, az, aw);
        const Vector4d b(bx, by, bz, bw);

        REQUIRE(
            ((ax != bx) || (ay != by) || (az != bz) || (aw != bw)) ==
            (a != b)
        );
    }

    return;
}

// Tests matrix multiplication with the multiplication operator (*) with a known result
TEST_CASE(__FILE__"/MatrixMult", "[Vector][Vector4]")
{
    Vector4d vec(117, 12, -36, 500);

    Matrix4x4 mat;
    mat[0] = { -43.7,   83,   96,     86 };
    mat[1] = {    12, 34.3,   43,    -47 };
    mat[2] = {    36,   67, 48.9,    -32 };
    mat[3] = {   -69,   47,   21,  89.01 };

    vec = vec * mat;

    REQUIRE(Vector4d(35427.1, -23232.4, -12744.4, 36240) == vec);

    return;
}

// Tests matrix multiplication with the multiplication equals operator (*=) with a known result
TEST_CASE(__FILE__"/MatrixMult_Equals", "[Vector][Vector4]")
{
    Vector4d vec(117, 12, -36, 500);

    Matrix4x4 mat;
    mat[0] = { -43.7,   83,   96,     86 };
    mat[1] = {    12, 34.3,   43,    -47 };
    mat[2] = {    36,   67, 48.9,    -32 };
    mat[3] = {   -69,   47,   21,  89.01 };

    vec *= mat;

    REQUIRE(Vector4d(35427.1, -23232.4, -12744.4, 36240) == vec);

    return;
}

// Tests matrix multiplication with the multiplication operator (*) with a known result, but with an int-vector
TEST_CASE(__FILE__"/MatrixMult_Int", "[Vector][Vector4]")
{
    Vector4i vec(112, -420, 80085, 1);

    Matrix4x4 mat;
    mat[0] = {  12,  83, 96,  86 };
    mat[1] = {  12, -57, 43, -47 };
    mat[2] = {  36,  67, 61, -32 };
    mat[3] = { -69,  47, 21,  99 };

    vec = vec * mat;

    REQUIRE(Vector4i(7654730, 3468892, 4861045, 1654416) == vec);

    return;
}

// Tests matrix multiplication with the multiplication equals operator (*=) with a known result, but with an int - vector
TEST_CASE(__FILE__"/MatrixMult_Equals_Int", "[Vector][Vector4]")
{
    Vector4i vec(112, -420, 80085, 1);

    Matrix4x4 mat;
    mat[0] = {  12,  83, 96,  86 };
    mat[1] = {  12, -57, 43, -47 };
    mat[2] = {  36,  67, 61, -32 };
    mat[3] = { -69,  47, 21,  99 };

    vec *= mat;

    REQUIRE(Vector4i(7654730, 3468892, 4861045, 1654416) == vec);

    return;
}

// Tests loose comparison via Vector4d::Similar -> true
TEST_CASE(__FILE__"/Loose_Comparison_True_Vector4d", "[Vector][Vector4]")
{
    REQUIRE(
        Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333).Similar(
            Vector4d(0, -6.666666667, 10, -3.33333333333333)
    ));
    return;
}

// Tests loose comparison via Vector4d::Similar -> false
TEST_CASE(__FILE__"/Loose_Comparison_False_Vector4d", "[Vector][Vector4]")
{
    REQUIRE_FALSE(
        Vector4d(0.00000000000000000000001, -6.6666666666666666666666666666, 9.9999999999999999999999999999, -3.3333333333333333333333333333333333333).Similar(
            Vector4d(0.1, -6.7, 10.1, -3.333)
    ));
    return;
}

// Tests that the move constructor works
TEST_CASE(__FILE__"/Move_Constructor", "[Vector][Vector4]")
{
    Vector4d a(1, 2, 3, 4);
    Vector4d b(std::move(a));

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);
    REQUIRE(b.z == 3.0);
    REQUIRE(b.w == 4.0);

    return;
}

// Tests that the move operator works
TEST_CASE(__FILE__"/Move_Operator", "[Vector][Vector4]")
{
    Vector4d a(1, 2, 3, 4);
    Vector4d b = std::move(a);

    REQUIRE(b.x == 1.0);
    REQUIRE(b.y == 2.0);
    REQUIRE(b.z == 3.0);
    REQUIRE(b.w == 4.0);

    return;
}
