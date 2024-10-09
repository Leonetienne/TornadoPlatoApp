#include "../_TestingUtilities/Catch2.h"
#include "../Eule/TrapazoidalPrismCollider.h"
#include "../Eule/Quaternion.h"
#include <random>
#include <array>

using namespace Eule;
using TPC = TrapazoidalPrismCollider;

namespace {
    static std::mt19937 rng = std::mt19937((std::random_device())());
}

// Tests that all vertices can be set individually, and at once
TEST_CASE(__FILE__"/Can_Set_Each_Vertex", "[TrapazoidalPrismCollider][Collider]")
{
    // All vertex values are unique
    TPC tpc;
    tpc.SetVertex(TPC::FRONT	| TPC::LEFT		| TPC::BOTTOM,	Vector3d(-1, -1, 1)  * 1);
    tpc.SetVertex(TPC::FRONT	| TPC::LEFT		| TPC::TOP,		Vector3d(-1, 1, 1)   * 2);
    tpc.SetVertex(TPC::BACK	| TPC::LEFT		| TPC::BOTTOM,	Vector3d(-1, -1, -1) * 3);
    tpc.SetVertex(TPC::BACK	| TPC::LEFT		| TPC::TOP,		Vector3d(-1, 1, -1)  * 4);
    tpc.SetVertex(TPC::FRONT	| TPC::RIGHT	| TPC::BOTTOM,	Vector3d(1, -1, 1)   * 5);
    tpc.SetVertex(TPC::FRONT	| TPC::RIGHT	| TPC::TOP,		Vector3d(1, 1, 1)    * 6);
    tpc.SetVertex(TPC::BACK	| TPC::RIGHT	| TPC::BOTTOM,	Vector3d(1, -1, -1)  * 7);
    tpc.SetVertex(TPC::BACK	| TPC::RIGHT	| TPC::TOP,		Vector3d(1, 1, -1)   * 8);

    INFO("FRONT|LEFT|BOTTOM");
    REQUIRE(tpc.GetVertex(TPC::FRONT    | TPC::LEFT	| TPC::BOTTOM)	== (Vector3d(-1, -1, 1) * 1));

    INFO("FRONT|LEFT|TOP");
    REQUIRE(tpc.GetVertex(TPC::FRONT    | TPC::LEFT	| TPC::TOP)		== (Vector3d(-1, 1, 1) * 2));

    INFO("BACK|LEFT|BOTTOM");
    REQUIRE(tpc.GetVertex(TPC::BACK     | TPC::LEFT	| TPC::BOTTOM)	== (Vector3d(-1, -1, -1) * 3));

    INFO("BACK|LEFT|TOP");
    REQUIRE(tpc.GetVertex(TPC::BACK     | TPC::LEFT	| TPC::TOP)		== (Vector3d(-1, 1, -1) * 4));

    INFO("FRONT|RIGHT|BOTTOM");
    REQUIRE(tpc.GetVertex(TPC::FRONT    | TPC::RIGHT	| TPC::BOTTOM)	== (Vector3d(1, -1, 1) * 5));

    INFO("FRONT|RIGHT|TOP");
    REQUIRE(tpc.GetVertex(TPC::FRONT    | TPC::RIGHT	| TPC::TOP)		== (Vector3d(1, 1, 1) * 6));

    INFO("BACK|RIGHT|BOTTOM");
    REQUIRE(tpc.GetVertex(TPC::BACK     | TPC::RIGHT	| TPC::BOTTOM)	== (Vector3d(1, -1, -1) * 7));

    INFO("BACK|RIGHT|TOP");
    REQUIRE(tpc.GetVertex(TPC::BACK     | TPC::RIGHT	| TPC::TOP)		== (Vector3d(1, 1, -1) * 8));

    return;
}

// Tests that points inside work.
// For this, we define a few points around [0,0,0] and check if they are contained.
// We then rotate the collider, and check again
// Gets repeated for every possible rotation with a min-distance per axis of 2 deg
TEST_CASE(__FILE__"/Points_Inside", "[TrapazoidalPrismCollider][Collider]")
{
    // Setup
    // Define known-inside points
    std::array<Vector3d, 9> knownInsides = {
        Vector3d( 1,-1, 1),
        Vector3d(-1,-1, 1),
        Vector3d( 1, 1, 1),
        Vector3d(-1, 1, 1),
        Vector3d( 1,-1,-1),
        Vector3d(-1,-1,-1),
        Vector3d( 1, 1,-1),
        Vector3d(-1, 1,-1),
        Vector3d( 0, 0, 0),
    };

    // Create collider, a cube of size 10^3 around the center
    TPC tpc;

    // Exercise
    // Now check that these points are inside for all these possible angles
    #ifndef _DEBUG
    constexpr double stepSize = 40;
    #else
    constexpr double stepSize = 32;
    #endif
    for (double theta = 0; theta < 360.01; theta += stepSize) {
        for (double phi = 0; phi < 360.01; phi += 2) {
            for (double alpha = 0; alpha < 360.01; alpha += stepSize)
            {
                // Rotate box
                tpc.SetVertex(TPC::FRONT	| TPC::LEFT		| TPC::BOTTOM,	Quaternion({theta, phi, alpha}) * (Vector3d(-1, -1, 1)  * 10));
                tpc.SetVertex(TPC::FRONT	| TPC::LEFT		| TPC::TOP,		Quaternion({theta, phi, alpha}) * (Vector3d(-1, 1, 1)   * 10));
                tpc.SetVertex(TPC::BACK	| TPC::LEFT		| TPC::BOTTOM,	Quaternion({theta, phi, alpha}) * (Vector3d(-1, -1, -1) * 10));
                tpc.SetVertex(TPC::BACK	| TPC::LEFT		| TPC::TOP,		Quaternion({theta, phi, alpha}) * (Vector3d(-1, 1, -1)  * 10));
                tpc.SetVertex(TPC::FRONT	| TPC::RIGHT	| TPC::BOTTOM,	Quaternion({theta, phi, alpha}) * (Vector3d(1, -1, 1)   * 10));
                tpc.SetVertex(TPC::FRONT	| TPC::RIGHT	| TPC::TOP,		Quaternion({theta, phi, alpha}) * (Vector3d(1, 1, 1)    * 10));
                tpc.SetVertex(TPC::BACK	| TPC::RIGHT	| TPC::BOTTOM,	Quaternion({theta, phi, alpha}) * (Vector3d(1, -1, -1)  * 10));
                tpc.SetVertex(TPC::BACK	| TPC::RIGHT	| TPC::TOP,		Quaternion({theta, phi, alpha}) * (Vector3d(1, 1, -1)   * 10));

                // Verify
                // Verify that all are inside
                for (const Vector3d& v : knownInsides) {
                    REQUIRE(tpc.Contains(v));
                }
            }
        }
    }

    return;
}

// Tests that points outside work.
// For this, we define a few points that are definitely outside for various reasons and check if they are not contained.
// We then rotate the collider, and check again
// Gets repeated for every possible rotation with a min-distance per axis of 2 deg
TEST_CASE(__FILE__"/Points_Outside", "[TrapazoidalPrismCollider][Collider]")
{
    // Setup
    // Define known-inside points
    std::array<Vector3d, 14> knownOutsides = {
        Vector3d(-199, 0, 0),
        Vector3d(0, -199, 0),
        Vector3d(0, 0, -199),
        Vector3d(199, 0, 0),
        Vector3d(0, 199, 0),
        Vector3d(0, 0, 199),
        Vector3d( 20, -20, 0),
        Vector3d(50, 50, 50),
        Vector3d(50, -50, 0),
        Vector3d( 0, 0, 29),
        Vector3d( 2, 1, -18),
        Vector3d( -1, 29, -1),
        Vector3d( 0, -50, -50),
        Vector3d( -50, -50, -50)
    };

    // Create collider, a cube of size 10^3 around the center
    TPC tpc;

    // Exercise
    // Now check that these points are inside for all these possible angles
    #ifndef _DEBUG
    constexpr double stepSize = 40;
    #else
    constexpr double stepSize = 32;
    #endif
    for (double theta = 0; theta < 360.01; theta += stepSize) {
        for (double phi = 0; phi < 360.01; phi += 2) {
            for (double alpha = 0; alpha < 360.01; alpha += stepSize)
            {
                // Rotate box
                tpc.SetVertex(TPC::FRONT | TPC::LEFT  | TPC::BOTTOM, Quaternion({ theta, phi, alpha }) * (Vector3d(-1, -1, 1) * 10));
                tpc.SetVertex(TPC::FRONT | TPC::LEFT  | TPC::TOP, Quaternion({ theta, phi, alpha }) * (Vector3d(-1, 1, 1) * 10));
                tpc.SetVertex(TPC::BACK  | TPC::LEFT  | TPC::BOTTOM, Quaternion({ theta, phi, alpha }) * (Vector3d(-1, -1, -1) * 10));
                tpc.SetVertex(TPC::BACK  | TPC::LEFT  | TPC::TOP, Quaternion({ theta, phi, alpha }) * (Vector3d(-1, 1, -1) * 10));
                tpc.SetVertex(TPC::FRONT | TPC::RIGHT | TPC::BOTTOM, Quaternion({ theta, phi, alpha }) * (Vector3d(1, -1, 1) * 10));
                tpc.SetVertex(TPC::FRONT | TPC::RIGHT | TPC::TOP, Quaternion({ theta, phi, alpha }) * (Vector3d(1, 1, 1) * 10));
                tpc.SetVertex(TPC::BACK  | TPC::RIGHT | TPC::BOTTOM, Quaternion({ theta, phi, alpha }) * (Vector3d(1, -1, -1) * 10));
                tpc.SetVertex(TPC::BACK  | TPC::RIGHT | TPC::TOP, Quaternion({ theta, phi, alpha }) * (Vector3d(1, 1, -1) * 10));

                // Verify
                // Verify that all are outside
                for (const Vector3d& v : knownOutsides) {
                    REQUIRE_FALSE(tpc.Contains(v));
                }
            }
        }
    }

    return;
}
